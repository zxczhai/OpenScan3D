#include "cereal/archives/json.hpp"

#include "openMVG/features/akaze/image_describer_akaze_io.hpp"
#include "openMVG/features/sift/SIFT_Anatomy_Image_Describer_io.hpp"
#include "openMVG/image/image_io.hpp"
#include "openMVG/features/regions_factory_io.hpp"
#include "openMVG/sfm/sfm_data.hpp"
#include "openMVG/sfm/sfm_data_io.hpp"
#include "openMVG/system/logger.hpp"
#include "openMVG/system/loggerprogress.hpp"
#include "openMVG/system/timer.hpp"
#include "third_party/cmdLine/cmdLine.h"
#include "third_party/stlplus3/filesystemSimplified/file_system.hpp"
#include "nonFree/sift/SIFT_describer_io.hpp"
#include "cereal/details/helpers.hpp"

#include <atomic>
#include <cstdlib>
#include <fstream>
#include <string>


#ifdef OPENMVG_USE_OPENMP
#include <omp.h>
#endif
#include "ComputeFeatures.hpp"

using namespace openMVG;
using namespace openMVG::image;
using namespace openMVG::features;
using namespace openMVG::sfm;

features::EDESCRIBER_PRESET stringToEnum(const std::string &sPreset)
{
    features::EDESCRIBER_PRESET preset;
    if (sPreset == "NORMAL")
        preset = features::NORMAL_PRESET;
    else if (sPreset == "HIGH")
        preset = features::HIGH_PRESET;
    else if (sPreset == "ULTRA")
        preset = features::ULTRA_PRESET;
    else
        preset = features::EDESCRIBER_PRESET(-1);
    return preset;
}

/// - Compute view image description (feature & descriptor extraction)
/// - Export computed data
int ComputeFeatures(std::string sSfM_Data_Filename, std::string sOutDir, std::string sImage_Describer_Method = "SIFT", std::string sFeaturePreset = "NORMAL", bool bUpRight = false, bool bForce = false)
{

#ifdef OPENMVG_USE_OPENMP
    int iNumThreads = 0;//default use MAX threads
    const unsigned int nb_max_thread = omp_get_max_threads();
#endif

    OPENMVG_LOG_INFO
        << " You called : "
        << "ComputeFeatures Task" << "\n"
        << "--input_file " << sSfM_Data_Filename << "\n"
        << "--outdir " << sOutDir << "\n"
        << "--describerMethod " << sImage_Describer_Method << "\n"
        << "--upright " << bUpRight << "\n"
        << "--describerPreset " << (sFeaturePreset.empty() ? "NORMAL" : sFeaturePreset) << "\n"
        << "--force " << bForce << "\n"
#ifdef OPENMVG_USE_OPENMP
        << "--numThreads " << nb_max_thread << "\n"
#endif
        ;

    if (sOutDir.empty())
    {
        OPENMVG_LOG_ERROR << "\nIt is an invalid output directory";
        return EXIT_FAILURE;
    }

    // Create output dir
    if (!stlplus::folder_exists(sOutDir))
    {
        if (!stlplus::folder_create(sOutDir))
        {
            OPENMVG_LOG_ERROR << "Cannot create output directory";
            return EXIT_FAILURE;
        }
    }

    //---------------------------------------
    // a. Load input scene
    //---------------------------------------
    SfM_Data sfm_data;
    if (!Load(sfm_data, sSfM_Data_Filename, ESfM_Data(VIEWS | INTRINSICS)))
    {
        OPENMVG_LOG_ERROR
            << "The input file \"" << sSfM_Data_Filename << "\" cannot be read";
        return EXIT_FAILURE;
    }

    // b. Init the image_describer
    // - retrieve the used one in case of pre-computed features
    // - else create the desired one

    using namespace openMVG::features;
    std::unique_ptr<Image_describer> image_describer;

    const std::string sImage_describer = stlplus::create_filespec(sOutDir, "image_describer", "json");
    if (!bForce && stlplus::is_file(sImage_describer))
    {
        // Dynamically load the image_describer from the file (will restore old used settings)
        std::ifstream stream(sImage_describer.c_str());
        if (!stream)
            return EXIT_FAILURE;

        try
        {
            cereal::JSONInputArchive archive(stream);
            archive(cereal::make_nvp("image_describer", image_describer));
        }
        catch (const cereal::Exception &e)
        {
            OPENMVG_LOG_ERROR << e.what() << '\n'
                              << "Cannot dynamically allocate the Image_describer interface.";
            return EXIT_FAILURE;
        }
    }
    else
    {
        // Create the desired Image_describer method.
        // Don't use a factory, perform direct allocation
        if (sImage_Describer_Method == "SIFT")
        {
            image_describer.reset(new SIFT_Image_describer(SIFT_Image_describer::Params(), !bUpRight));
        }
        else if (sImage_Describer_Method == "SIFT_ANATOMY")
        {
            image_describer.reset(
                new SIFT_Anatomy_Image_describer(SIFT_Anatomy_Image_describer::Params()));
        }
        else if (sImage_Describer_Method == "AKAZE_FLOAT")
        {
            image_describer = AKAZE_Image_describer::create(AKAZE_Image_describer::Params(AKAZE::Params(), AKAZE_MSURF), !bUpRight);
        }
        else if (sImage_Describer_Method == "AKAZE_MLDB")
        {
            image_describer = AKAZE_Image_describer::create(AKAZE_Image_describer::Params(AKAZE::Params(), AKAZE_MLDB), !bUpRight);
        }
        if (!image_describer)
        {
            OPENMVG_LOG_ERROR << "Cannot create the designed Image_describer:"
                              << sImage_Describer_Method << ".";
            return EXIT_FAILURE;
        }
        else
        {
            if (!sFeaturePreset.empty())
                if (!image_describer->Set_configuration_preset(stringToEnum(sFeaturePreset)))
                {
                    OPENMVG_LOG_ERROR << "Preset configuration failed.";
                    return EXIT_FAILURE;
                }
        }

        // Export the used Image_describer and region type for:
        // - dynamic future regions computation and/or loading
        {
            std::ofstream stream(sImage_describer.c_str());
            if (!stream)
                return EXIT_FAILURE;

            cereal::JSONOutputArchive archive(stream);
            archive(cereal::make_nvp("image_describer", image_describer));
            auto regionsType = image_describer->Allocate();
            archive(cereal::make_nvp("regions_type", regionsType));
        }
    }

    // Feature extraction routines
    // For each View of the SfM_Data container:
    // - if regions file exists continue,
    // - if no file, compute features
    {
        system::Timer timer;
        Image<unsigned char> imageGray;

        system::LoggerProgress my_progress_bar(sfm_data.GetViews().size(), "- EXTRACT FEATURES -");

        // Use a boolean to track if we must stop feature extraction
        std::atomic<bool> preemptive_exit(false);
#ifdef OPENMVG_USE_OPENMP
        

        if (iNumThreads > 0)
        {
            omp_set_num_threads(iNumThreads);
        }
        else
        {
            omp_set_num_threads(nb_max_thread);
        }

#pragma omp parallel for schedule(dynamic) if (iNumThreads > 0) private(imageGray)
#endif
        for (int i = 0; i < static_cast<int>(sfm_data.views.size()); ++i)
        {
            Views::const_iterator iterViews = sfm_data.views.begin();
            std::advance(iterViews, i);
            const View *view = iterViews->second.get();
            const std::string
                sView_filename = stlplus::create_filespec(sfm_data.s_root_path, view->s_Img_path),
                sFeat = stlplus::create_filespec(sOutDir, stlplus::basename_part(sView_filename), "feat"),
                sDesc = stlplus::create_filespec(sOutDir, stlplus::basename_part(sView_filename), "desc");
                OPENMVG_LOG_INFO<<"Creating: "<<sFeat<<"&& "<<sDesc;
            // If features or descriptors file are missing, compute them
            if (!preemptive_exit && (bForce || !stlplus::file_exists(sFeat) || !stlplus::file_exists(sDesc)))
            {
                if (!ReadImage(sView_filename.c_str(), &imageGray))
                    continue;

                //
                // Look if there is an occlusion feature mask
                //
                Image<unsigned char> *mask = nullptr; // The mask is null by default

                const std::string
                    mask_filename_local =
                        stlplus::create_filespec(sfm_data.s_root_path,
                                                 stlplus::basename_part(sView_filename) + "_mask", "png"),
                    mask_filename_global =
                        stlplus::create_filespec(sfm_data.s_root_path, "mask", "png");

                Image<unsigned char> imageMask;
                // Try to read the local mask
                if (stlplus::file_exists(mask_filename_local))
                {
                    if (!ReadImage(mask_filename_local.c_str(), &imageMask))
                    {
                        OPENMVG_LOG_ERROR
                            << "Invalid mask: " << mask_filename_local << ';'
                            << "Stopping feature extraction.";
                        preemptive_exit = true;
                        continue;
                    }
                    // Use the local mask only if it fits the current image size
                    if (imageMask.Width() == imageGray.Width() && imageMask.Height() == imageGray.Height())
                        mask = &imageMask;
                }
                else
                {
                    // Try to read the global mask
                    if (stlplus::file_exists(mask_filename_global))
                    {
                        if (!ReadImage(mask_filename_global.c_str(), &imageMask))
                        {
                            OPENMVG_LOG_ERROR
                                << "Invalid mask: " << mask_filename_global << ';'
                                << "Stopping feature extraction.";
                            preemptive_exit = true;
                            continue;
                        }
                        // Use the global mask only if it fits the current image size
                        if (imageMask.Width() == imageGray.Width() && imageMask.Height() == imageGray.Height())
                            mask = &imageMask;
                    }
                }

                // Compute features and descriptors and export them to files
                auto regions = image_describer->Describe(imageGray, mask);
                if (regions && !image_describer->Save(regions.get(), sFeat, sDesc))
                {
                    OPENMVG_LOG_ERROR
                        << "Cannot save regions for image: " << sView_filename << ';'
                        << "Stopping feature extraction.";
                    preemptive_exit = true;
                    continue;
                }
            }
            ++my_progress_bar;
        }
        OPENMVG_LOG_INFO << "Task done in (s): " << timer.elapsed();
    }
    return EXIT_SUCCESS;
}

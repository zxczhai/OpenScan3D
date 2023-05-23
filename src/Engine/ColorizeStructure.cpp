#include "openMVG/sfm/sfm_data.hpp"
#include "openMVG/sfm/sfm_data_io.hpp"
#include "openMVG/sfm/sfm_data_colorization.hpp"
#include "openMVG/system/logger.hpp"
#include "openMVG/types.hpp"

#include "software/SfM/SfMPlyHelper.hpp"
#include "ColorizeStructure.hpp"
#include "Utils.hpp"
using namespace openMVG;
using namespace openMVG::sfm;



// Convert from a SfM_Data format to another
int ColorizeStructure(std::string sfmDataDir, std::string outputDir)

{

    std::string
        sSfM_Data_Filename_In = sfmDataDir + "/sfm_data.bin",
        sOutputPLY_Out = outputDir + "/colorized.ply";

    if (sOutputPLY_Out.empty())
    {
        OPENMVG_LOG_ERROR << "No output PLY filename specified.";
        return EXIT_FAILURE;
    }

    // Load input SfM_Data scene
    SfM_Data sfm_data;
    if (!Load(sfm_data, sSfM_Data_Filename_In, ESfM_Data(ALL)))
    {
        OPENMVG_LOG_ERROR << "The input SfM_Data file \"" << sSfM_Data_Filename_In << "\" cannot be read.";
        return EXIT_FAILURE;
    }

    // Compute the scene structure color
    std::vector<Vec3> vec_3dPoints, vec_tracksColor, vec_camPosition;
    if (ColorizeTracks(sfm_data, vec_3dPoints, vec_tracksColor))
    {
        GetCameraPositions(sfm_data, vec_camPosition);

        // Export the SfM_Data scene in the expected format
        if (plyHelper::exportToPly(vec_3dPoints, vec_camPosition, sOutputPLY_Out, &vec_tracksColor))
        {
            return EXIT_SUCCESS;
        }
    }

    return EXIT_FAILURE;
}

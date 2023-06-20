#include <iostream>
#include "Global.h"
#include <string>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "LinuxMessage.hpp"
#include "IntrinsicsAnalysis.hpp"
#include "ComputeFeatures.hpp"
#include "ComputeMatches.hpp"
#include "PairGenerator.hpp"
#include "FilterMatches.hpp"
#include "SFM.hpp"
#include "ColorizeStructure.hpp"
#include "StructureFromKnownPoses.hpp"
#include "ColorizedRobustTriangulation.hpp"
#include "ExportToOpenMVS.hpp"
#include "DensifyPointCloud.hpp"
#include "ReconstructMesh.hpp"
#include "RefineMesh.hpp"
#include "TextureTheMesh.hpp"
#include "OBJ2STL.hpp"
#include "Camera.hpp"
#include "MVS.h"
#include "UtilCUDA.h"
#include <unistd.h>

uint8_t STATE_RETURN;

void write_to_file(const std::string &content)
{
    std::ofstream file("/tmp/.OpenScan3D/progress_bar_ctrl.tmp", std::ios::trunc);
    if (file.is_open())
    {
        file << content;
        file.close();
        std::cout << "Successfully written to file." << std::endl;
    }
    else
    {
        std::cout << "Failed to create file!" << std::endl;
    }
}

void checkGPUExist()
{
    int deviceCount = 0;
#ifdef _USE_CUDA
    cudaError_t error_id = cudaGetDeviceCount(&deviceCount);
    if (error_id != cudaSuccess)
    {
        printf("cudaGetDeviceCount returned %d\n-> %s\n", (int)error_id, cudaGetErrorString(error_id));
        printf("No available CUDA device(s),will use CPU Mode\n");
    }
    else if (deviceCount == 0)
    {
        printf("No available CUDA device(s),Please Check&will use CPU Mode\n");
    }
    else
    {
        printf("%d available CUDA device(s),will use GPU Mode\n", deviceCount);
    }
#else
    printf("No available CUDA device(s),will use CPU Mode\n");
#endif
}

bool checkDirExist(std::string pathToDir)
{
    DIR *dir;
    if ((dir = opendir(pathToDir.c_str())) == NULL)
        return false;
    return true;
}
bool savePid()
{
    int tid = getpid();
    if (checkDirExist("/tmp/.OpenScan3D") == false)
        mkdir("/tmp/.OpenScan3D", S_IRWXU | S_IRWXG | S_IRWXO);
    ofstream file;
    file.open("/tmp/.OpenScan3D/ProgramCache.tmp", ios::out | ios::trunc);
    if (!file.is_open())
        return false;
    file << std::to_string(tid) << endl;
    file.close();
    return true;
}
/**
 * @brief 分析并提取相机内参#1
 */
// int IntrinsicsAnalysis(std::string inputImageDir, std::string outputDir, std::string cameraDBDir)
// {
//     // test Dir Exist is useful
//     //  if(checkDirExist(inputImageDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR || checkDirExist(cameraDBDir)==PROCESSERROR)
//     //  {
//     //      ::system("echo directory not exist");
//     //      return EXIT_FAILURE;
//     //  }
//     ::system("echo IntrinsicsAnalysis working.........");
//     std::string cmd = "";
//     cmd.append("openMVG_main_SfMInit_ImageListing");
//     cmd.append(" ");
//     cmd.append("-i");
//     cmd.append(" ");
//     cmd.append(inputImageDir);
//     cmd.append(" ");
//     cmd.append("-o");
//     cmd.append(" ");
//     cmd.append(outputDir);
//     cmd.append(" ");
//     cmd.append("-d");
//     cmd.append(" ");
//     cmd.append(cameraDBDir);
//     if (::system(cmd.c_str()) != 0)
//     {
//         return EXIT_FAILURE;
//     }
//     else
//         ::system("echo IntrinsicsAnalysis Successfully.........");
//     return EXIT_SUCCESS;
// }

/**
 * @brief 提取图像特征点#2
 *
 */
// int ComputeFeatures(std::string sfmDataDir, std::string outputDir, std::string describerMethod = "SIFT", std::string describerPreset = "NORMAL", std::string upRight = "0", std::string forceCompute = "0")
// {
//     // if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
//     // {
//     //     ::system("echo directory not exist");
//     //     return PROCESSERROR;
//     // }
//     ::system("echo ComputeFeatures working.........");
//     std::string cmd = "";
//     cmd.append("openMVG_main_ComputeFeatures");
//     cmd.append(" ");
//     cmd.append("-i");
//     cmd.append(" ");
//     cmd.append(sfmDataDir);
//     cmd.append("/");
//     cmd.append("sfm_data.json");
//     cmd.append(" ");
//     cmd.append("-o");
//     cmd.append(" ");
//     cmd.append(outputDir);
//     cmd.append(" ");
//     cmd.append("-m");
//     cmd.append(" ");
//     cmd.append(describerMethod);
//     cmd.append(" ");
//     cmd.append("-p");
//     cmd.append(" ");
//     cmd.append(describerPreset);
//     cmd.append(" ");
//     cmd.append("-u");
//     cmd.append(" ");
//     cmd.append(upRight);
//     cmd.append(" ");
//     cmd.append("-f");
//     cmd.append(" ");
//     cmd.append(forceCompute);
//     if (::system(cmd.c_str()) != 0)
//     {
//         return EXIT_FAILURE;
//     }
//     else
//         ::system("echo ComputeFeatures Successfully.........");
//     return EXIT_SUCCESS;
// }
/**
 * @brief 构建图像匹配对#3
//  */
// int ComputePairs(std::string sfmDataDir, std::string outputDir)
// {
//     // if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
//     // {
//     //     ::system("echo directory not exist");
//     //     return PROCESSERROR;
//     // }
//     ::system("echo ComputePairs working.........");
//     std::string cmd = "";
//     cmd.append("openMVG_main_PairGenerator");
//     cmd.append(" ");
//     cmd.append("-i");
//     cmd.append(" ");
//     cmd.append(sfmDataDir);
//     cmd.append("/");
//     cmd.append("sfm_data.json");
//     cmd.append(" ");
//     cmd.append("-o");
//     cmd.append(" ");
//     cmd.append(outputDir);
//     cmd.append("/");
//     cmd.append("pairs.bin");
//     if (::system(cmd.c_str()) != 0)
//     {
//         return EXIT_FAILURE;
//     }
//     else
//         ::system("echo ComputePairs Successfully.........");
//     return EXIT_SUCCESS;
// }
// /**
//  * @brief 计算图像间的特征点匹配#4
//  */
// // int ComputeMatches(std::string sfmDataDir, std::string outputDir, std::string nearest_matching_method = "AUTO", std::string radio = "0.8")
// // {
// //     // if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
// //     // {
// //     //     ::system("echo directory not exist");
// //     //     return PROCESSERROR;
// //     // }
// //     ::system("echo ComputeMatches working.........");
// //     std::string cmd = "";
// //     cmd.append("openMVG_main_ComputeMatches");
// //     cmd.append(" ");
// //     cmd.append("-i");
// //     cmd.append(" ");
// //     cmd.append(sfmDataDir);
// //     cmd.append("/");
// //     cmd.append("sfm_data.json");
// //     cmd.append(" ");
// //     cmd.append("-o");
// //     cmd.append(" ");
// //     cmd.append(outputDir);
// //     cmd.append("/");
// //     cmd.append("featurePointMatch.bin");
// //     cmd.append(" ");
// //     cmd.append("-n");
// //     cmd.append(" ");
// //     cmd.append(nearest_matching_method);
// //     cmd.append(" ");
// //     cmd.append("-r");
// //     cmd.append(" ");
// //     cmd.append(radio);
// //     if (::system(cmd.c_str()) != 0)
// //     {
// //         return EXIT_FAILURE;
// //     }
// //     else
// //         ::system("echo ComputeMatches Successfully.........");
// //     return EXIT_SUCCESS;
// // }

// /**
//  * @brief 使用几何模型过滤不正确的匹配#5
//  */
// // int FilterMatches(std::string sfmDataDir, std::string outputDir, std::string geometric_mode)
// // {
// //     // if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
// //     // {
// //     //     ::system("echo directory not exist");
// //     //     return PROCESSERROR;
// //     // }
// //     ::system("echo FilterMatches working.........");
// //     std::string cmd = "";
// //     cmd.append("openMVG_main_GeometricFilter");
// //     cmd.append(" ");
// //     cmd.append("-i");
// //     cmd.append(" ");
// //     cmd.append(sfmDataDir);
// //     cmd.append("/");
// //     cmd.append("sfm_data.json");
// //     cmd.append(" ");
// //     cmd.append("-m");
// //     cmd.append(" ");
// //     cmd.append(sfmDataDir);
// //     cmd.append("/");
// //     cmd.append("featurePointMatch.bin");
// //     cmd.append(" ");
// //     cmd.append("-o");
// //     cmd.append(" ");
// //     cmd.append(outputDir);
// //     cmd.append("/");
// //     cmd.append("matches_filtered.bin");
// //     // cmd.append(" ");
// //     // cmd.append("-p");
// //     // cmd.append(" ");
// //     // cmd.append(outputDir);
// //     // cmd.append("/");
// //     // cmd.append("pairs.bin");
// //     // cmd.append(" ");
// //     // cmd.append("-s");
// //     // cmd.append(" ");
// //     // cmd.append(outputDir);
// //     // cmd.append("/");
// //     // cmd.append("filtered_pairs.bin");
// //     cmd.append(" ");
// //     cmd.append("-g");
// //     cmd.append(" ");
// //     cmd.append(geometric_mode);
// //     if (::system(cmd.c_str()) != 0)
// //     {
// //         return EXIT_FAILURE;
// //     }
// //     else
// //         ::system("echo Filter matches Successfully.........");
// //     return EXIT_SUCCESS;
// // }

// /**
//  * @brief 进行增量式的三维重建#6
//  */
// // int IncrementalReconstruction(std::string sfmDataDir, std::string outputDir, std::string sfmEngine)
// // {
// //     // if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
// //     // {
// //     //     ::system("echo directory not exist");
// //     //     return PROCESSERROR;
// //     // }
// //     ::system("echo Incremental reconstruction working.........");
// //     std::string cmd = "";
// //     cmd.append("openMVG_main_SfM");
// //     cmd.append(" ");
// //     cmd.append("-i");
// //     cmd.append(" ");
// //     cmd.append(sfmDataDir);
// //     cmd.append("/");
// //     cmd.append("sfm_data.json");
// //     cmd.append(" ");
// //     cmd.append("-m");
// //     cmd.append(" ");
// //     cmd.append(sfmDataDir);
// //     cmd.append(" ");
// //     cmd.append("-o");
// //     cmd.append(" ");
// //     cmd.append(outputDir);
// //     cmd.append(" ");
// //     cmd.append("-s");
// //     cmd.append(" ");
// //     cmd.append(sfmEngine);
// //     cmd.append(" ");
// //     cmd.append("-M");
// //     cmd.append(" ");
// //     cmd.append("matches_filtered.bin");

// //     if (::system(cmd.c_str()) != 0)
// //     {
// //         return EXIT_FAILURE;
// //     }
// //     else
// //         ::system("echo Incremental reconstruction Successfully.........");
// //     return EXIT_SUCCESS;
// // }

// // /**
// //  * @brief 进行全局式的三维重建#6
// //  */
// // int GlobalReconstruction(std::string sfmDataDir, std::string outputDir, std::string sfmEngine)
// // {
// //     // 判断需要的文件是否存在
// //     //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
// //     //  {
// //     //      system("echo directory not exist");
// //     //      return PROCESSERROR;
// //     //  }
// //     system("echo GlobalReconstruction working.........");
// //     std::string cmd = "";
// //     cmd.append("openMVG_main_SfM");
// //     cmd.append(" ");
// //     cmd.append("-i");
// //     cmd.append(" ");
// //     cmd.append(sfmDataDir);
// //     cmd.append("/");
// //     cmd.append("sfm_data.json");

// //     cmd.append(" ");
// //     cmd.append("-m");
// //     cmd.append(" ");
// //     cmd.append(sfmDataDir);

// //     cmd.append(" ");
// //     cmd.append("-M");
// //     cmd.append(" ");
// //     cmd.append("matches_filtered.bin");

// //     cmd.append(" ");
// //     cmd.append("-o");
// //     cmd.append(" ");
// //     cmd.append(outputDir);
// //     // cmd.append("/globalSFM ");

// //     cmd.append(" ");
// //     cmd.append("-s");
// //     cmd.append(" ");
// //     cmd.append(sfmEngine);

// //     if (::system(cmd.c_str()) != 0)
// //     {
// //         return EXIT_FAILURE;
// //     }
// //     else
// //         ::system("echo Incremental reconstruction Successfully.........");
// //     return EXIT_SUCCESS;
// // }
// /**
//  * @brief 为三维模型添加颜色信息#8
//  */
// // int ColorizeStructure(std::string sfmDataDir, std::string outputDir)
// // {
// //     // 判断需要的文件是否存在
// //     //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
// //     //  {
// //     //      system("echo directory not exist");
// //     //      return PROCESSERROR;
// //     //  }
// //     system("echo ColorizeStructure working.........");
// //     std::string cmd = "";
// //     cmd.append("openMVG_main_ComputeSfM_DataColor");

// //     cmd.append(" ");
// //     cmd.append("-i");
// //     cmd.append(" ");
// //     cmd.append(sfmDataDir);
// //     cmd.append("/sfm_data.bin");

// //     cmd.append(" ");
// //     cmd.append("-o");
// //     cmd.append(" ");
// //     cmd.append(outputDir);
// //     cmd.append("/");
// //     cmd.append("colorized.ply");

// //     if (::system(cmd.c_str()) != 0)
// //     {
// //         return EXIT_FAILURE;
// //     }
// //     else
// //         ::system("echo ColorizeStructure Successfully.........");
// //     return EXIT_SUCCESS;
// // }

// /**
//  * @brief 利用已知位姿的情况下，提取更多的三维结构信息#9
//  */

// // int StructureFromKnownPoses(std::string sfmDataDir, std::string feature_describeDir, std::string outputrobustDir, std::string matches_filteredDir, std::string on_off)
// // {
// //     // 判断需要的文件是否存在
// //     //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
// //     //  {
// //     //      system("echo directory not exist");
// //     //      return PROCESSERROR;
// //     //  }
// //     system("echo StructureFromKnownPoses working.........");
// //     std::string cmd = "";
// //     cmd.append("openMVG_main_ComputeStructureFromKnownPoses");
// //     cmd.append(" ");
// //     cmd.append("-i");
// //     cmd.append(" ");
// //     cmd.append(sfmDataDir);
// //     cmd.append("/");
// //     cmd.append("sfm_data.bin");

// //     cmd.append(" ");
// //     cmd.append("-m");
// //     cmd.append(" ");
// //     cmd.append(feature_describeDir);

// //     cmd.append(" ");
// //     cmd.append("-o");
// //     cmd.append(" ");
// //     cmd.append(outputrobustDir);
// //     cmd.append("/");
// //     cmd.append("robust.bin");

// //     cmd.append(" ");
// //     cmd.append("-f");
// //     cmd.append(" ");
// //     cmd.append(matches_filteredDir);
// //     cmd.append("/");
// //     cmd.append("matches_filtered.bin");

// //     cmd.append(" ");
// //     cmd.append("-d");
// //     cmd.append(" ");
// //     cmd.append(on_off);

// //     if (::system(cmd.c_str()) != 0)
// //     {
// //         return EXIT_FAILURE;
// //     }
// //     else
// //         ::system("echo StructureFromKnownPoses Successfully.........");
// //     return EXIT_SUCCESS;
// // }

// /**
//  * @brief 增加三维重建结果的稳健性和可靠性#10
//  */
// // int ColorizedRobustTriangulation(std::string robustDir, std::string colorizedDir)
// // {
// //     // 判断需要的文件是否存在
// //     //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
// //     //  {
// //     //      system("echo directory not exist");
// //     //      return PROCESSERROR;
// //     //  }
// //     system("echo ColorizedRobustTriangulation working.........");
// //     std::string cmd = "";
// //     cmd.append("openMVG_main_ComputeSfM_DataColor");

// //     cmd.append(" ");
// //     cmd.append("-i");
// //     cmd.append(" ");
// //     cmd.append(robustDir);
// //     cmd.append("/");
// //     cmd.append("robust.bin");

// //     cmd.append(" ");
// //     cmd.append("-o");
// //     cmd.append(" ");
// //     cmd.append(colorizedDir);
// //     cmd.append("/");
// //     cmd.append("robust_colorized.ply");

// //     if (::system(cmd.c_str()) != 0)
// //     {
// //         return EXIT_FAILURE;
// //     }
// //     else
// //         ::system("echo ColorizedRobustTriangulation Successfully.........");
// //     return EXIT_SUCCESS;
// // }

// /**
//  * @brief 进行控制点的注册，以便在不同数据集之间进行对比和匹配#11
//  */

// // int ControlPointsRegistration(std::string sfmDataDir)
// // {
// //     // 判断需要的文件是否存在
// //     //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
// //     //  {
// //     //      system("echo directory not exist");
// //     //      return PROCESSERROR;
// //     //  }
// //     system("echo ControlPointsRegistration working.........");
// //     std::string cmd = "";
// //     cmd.append("ui_openMVG_control_points_registration");

// //     cmd.append(" ");
// //     cmd.append("-i");
// //     cmd.append(" ");
// //     cmd.append(sfmDataDir);
// //     cmd.append("/");
// //     cmd.append("sfm_data.bin");

// //     if (::system(cmd.c_str()) != 0)
// //     {
// //         return EXIT_FAILURE;
// //     }
// //     else
// //         ::system("echo ControlPointsRegistration Successfully.........");
// //     return EXIT_SUCCESS;
// // }

// /**
//  * @brief 输出为 openMVS 格式，以便进行后续的密集重建#12
//  */

// // int ExportToOpenMVS(std::string sfmDataDir, std::string output_sceneDir, std::string output_imageDir)
// // {
// //     // 判断需要的文件是否存在
// //     //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
// //     //  {
// //     //      system("echo directory not exist");
// //     //      return PROCESSERROR;
// //     //  }
// //     system("echo ExportToOpenMVS working.........");
// //     std::string cmd = "";
// //     cmd.append("openMVG_main_openMVG2openMVS");
// //     cmd.append(" ");
// //     cmd.append("-i");
// //     cmd.append(" ");
// //     cmd.append(sfmDataDir);
// //     cmd.append("/");
// //     cmd.append("sfm_data.bin");

// //     cmd.append(" ");
// //     cmd.append("-o");
// //     cmd.append(" ");
// //     cmd.append(output_sceneDir);
// //     cmd.append("/");
// //     cmd.append("scene.mvs");

// //     cmd.append(" ");
// //     cmd.append("-d");
// //     cmd.append(" ");
// //     cmd.append(output_imageDir);
// //     cmd.append("/");
// //     cmd.append("images");

// //     if (::system(cmd.c_str()) != 0)
// //     {
// //         return EXIT_FAILURE;
// //     }
// //     else
// //         ::system("echo ExportToOpenMVS Successfully.........");
// //     return EXIT_SUCCESS;
// // }
// /**
//  * @brief 密集化点云，以得到更精细的三维模型#13
//  */

// // int DensifyPointCloud(std::string sceneDir, std::string outputDir)
// // {
// //     // 判断需要的文件是否存在
// //     //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
// //     //  {
// //     //      system("echo directory not exist");
// //     //      return PROCESSERROR;
// //     //  }
// //     std::string cmd = "";
// //     cmd.append(sceneDir);
// //     cmd.append("/");
// //     cmd.append("scene.mvs");

// //     cmd.append(" ");
// //     cmd.append("--dense-config-file");
// //     cmd.append(" ");
// //     cmd.append("Densify.ini");

// //     cmd.append(" ");
// //     cmd.append("--resolution-level");
// //     cmd.append(" ");
// //     cmd.append("1");

// //     cmd.append(" ");
// //     cmd.append("--number-views");
// //     cmd.append(" ");
// //     cmd.append("-8");

// //     cmd.append(" ");
// //     cmd.append("-w");
// //     cmd.append(" ");
// //     cmd.append(outputDir);

// //     if (system(cmd.c_str()) != 0)
// //     {
// //         return EXIT_FAILURE;
// //     }
// //     else
// //         system("echo DensifyPointCloud Successfully.........");
// //     return EXIT_SUCCESS;
// // }
// /**
//  * @brief  基于点云重建网格模型#14
//  */

// // int ReconstructTheMesh(std::string scene_denseDir, std::string outputDir)
// // {
// //     // 判断需要的文件是否存在
// //     //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
// //     //  {
// //     //      system("echo directory not exist");
// //     //      return PROCESSERROR;
// //     //  }
// //     system("echo ReconstructMesh working.........");
// //     std::string cmd = "";
// //     cmd.append("../../../bin/OpenMVS/ReconstructMesh");
// //     cmd.append(" ");
// //     cmd.append(scene_denseDir);
// //     cmd.append("/");
// //     cmd.append("scene_dense.mvs");

// //     cmd.append(" ");
// //     cmd.append("-w");
// //     cmd.append(" ");
// //     cmd.append(outputDir);

// //     if (system(cmd.c_str()) != 0)
// //     {
// //         return EXIT_FAILURE;
// //     }
// //     else
// //         system("echo ReconstructMesh Successfully.........");
// //     return EXIT_SUCCESS;
// // }

// /**
//  * @brief   对网格模型进行细化，使其更加精细#15
//  */

// // int RefineTheMesh(std::string scene_dense_meshDir, std::string outputDir)
// // {
// //     // 判断需要的文件是否存在
// //     //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
// //     //  {
// //     //      system("echo directory not exist");
// //     //      return PROCESSERROR;
// //     //  }
// //     system("echo RefineTheMesh working.........");
// //     std::string cmd = "";
// //     cmd.append("../../../bin/OpenMVS/RefineMesh");

// //     cmd.append(" ");
// //     cmd.append(scene_dense_meshDir);
// //     cmd.append("/");
// //     cmd.append("scene_dense_mesh.mvs");

// //     cmd.append(" ");
// //     cmd.append("--scales");
// //     cmd.append(" ");
// //     cmd.append("1");

// //     cmd.append(" ");
// //     cmd.append("--gradient-step");
// //     cmd.append(" ");
// //     cmd.append("25.05");

// //     cmd.append(" ");
// //     cmd.append("-w");
// //     cmd.append(" ");
// //     cmd.append(outputDir);

// //     if (system(cmd.c_str()) != 0)
// //     {
// //         return EXIT_FAILURE;
// //     }
// //     else
// //         system("echo RefineTheMesh Successfully.........");
// //     return EXIT_SUCCESS;
// // }

// /**
//  * @brief  为网格模型添加纹理信息，以提高视觉效果#16
//  */
// // int TextureTheMesh(std::string scene_dense_mesh_refineDir, std::string outputDir)
// // {
// //     // 判断需要的文件是否存在
// //     //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
// //     //  {
// //     //      system("echo directory not exist");
// //     //      return PROCESSERROR;
// //     //  }
// //     system("echo TextureTheMesh working.........");
// //     std::string cmd = "";
// //     cmd.append("../../bin/OpenMVS/TextureMesh");

// //     cmd.append(" ");
// //     cmd.append(scene_dense_mesh_refineDir);
// //     cmd.append("/");
// //     cmd.append("scene_dense_mesh_refine.mvs");

// //     cmd.append(" ");
// //     cmd.append("--decimate");
// //     cmd.append(" ");
// //     cmd.append("0.5");

// //     cmd.append(" ");
// //     cmd.append("-w");
// //     cmd.append(" ");
// //     cmd.append(outputDir);

// //     if (system(cmd.c_str()) != 0)
// //     {
// //         return EXIT_FAILURE;
// //     }
// //     else
// //         system("echo TextureTheMesh Successfully.........");
// //     return EXIT_SUCCESS;
// // }

// /**
//  * @brief   估计视差图，以便进行后续的深度图融合#17
//  */
// int EstimateDisparityMaps(std::string mvsDataDir, std::string outputDir)
// {
//     // 判断需要的文件是否存在
//     //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
//     //  {
//     //      ::system("echo directory not exist");
//     //      return PROCESSERROR;
//     //  }
//     ::system("echo EstimateDisparityMaps working.........");
//     std::string cmd = "";
//     cmd.append("../Binary/OpenMVS/DensifyPointCloud");
//     cmd.append(" ");
//     cmd.append(mvsDataDir);
//     cmd.append("/");
//     cmd.append("scene.mvs");
//     cmd.append(" ");
//     cmd.append("--dense-config-file");
//     cmd.append(" ");
//     cmd.append("Densify.ini");
//     cmd.append(" ");
//     cmd.append("--fusion-mode");
//     cmd.append(" ");
//     cmd.append("-1");
//     cmd.append(" ");
//     cmd.append("-w");
//     cmd.append(" ");
//     cmd.append(outputDir);
//     if (::system(cmd.c_str()) != 0)
//     {
//         return EXIT_FAILURE;
//     }
//     else
//         ::system("echo EstimateDisparityMaps Successfully.........");
//     return EXIT_SUCCESS;
// }

// /**
//  * @brief   将多个视差图融合，得到最终的深度图#18
//  */
// int FuseDisparityMaps(std::string mvsDataDir, std::string outputDir)
// {
//     // 判断需要的文件是否存在
//     //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
//     //  {
//     //      ::system("echo directory not exist");
//     //      return PROCESSERROR;
//     //  }
//     ::system("echo FuseDisparityMaps working.........");
//     std::string cmd = "";
//     cmd.append("../../../bin/OpenMVS/DensifyPointCloud");
//     cmd.append(" ");
//     cmd.append(mvsDataDir);
//     cmd.append("/");
//     cmd.append("scene.mvs");
//     cmd.append(" ");
//     cmd.append("--dense-config-file");
//     cmd.append(" ");
//     cmd.append("Densify.ini");
//     cmd.append(" ");
//     cmd.append("--fusion-mode");
//     cmd.append(" ");
//     cmd.append("-2");
//     cmd.append(" ");
//     cmd.append("-w");
//     cmd.append(" ");
//     cmd.append(outputDir);
//     cmd.append(" ");
//     if (::system(cmd.c_str()) != 0)
//     {
//         return EXIT_FAILURE;
//     }
//     else
//         ::system("echo FuseDisparityMaps Successfully.........");
//     return EXIT_SUCCESS;
// }

// // /**
// //  * @brief images2mvs Test
// //  */
// // int AutoR3d(std::string inputImageDir, std::string outputDir)
// // {
// //     IntrinsicsAnalysis(inputImageDir, outputDir, "../sensor_width_camera_database.txt");
// //     ComputeFeatures(outputDir, outputDir);
// //     ComputePairs(outputDir, outputDir);
// //     ComputeMatches(outputDir, outputDir);
// //     // FilterMatches(outputDir, outputDir);
// //     // IncrementalReconstruction(outputDir, outputDir);
// //     GlobalReconstruction(outputDir, outputDir);
// //     ColorizeStructure(outputDir, outputDir);
// //     StructureFromKnownPoses(outputDir, outputDir);
// //     ColorizedRobustTriangulation(outputDir, outputDir);
// //     // ControlPointsRegistration(outputDir, outputDir);
// //     ExportToOpenMVS(outputDir, outputDir);
// //     DensifyPointCloud(outputDir, outputDir);
// //     ReconstructTheMesh(outputDir, outputDir);
// //     RefineTheMesh(outputDir, outputDir);
// //  TextureTheMesh(scene_dense_mesh_refineDir,outputDir)
// //     EstimateDisparityMaps(outputDir, outputDir);
// //     FuseDisparityMaps(outputDir, outputDir);
// //     return EXIT_SUCCESS;
// // }

int getCustomCamera(CustomCamera &customCamera)
{
    ifstream cameraCache;
    cameraCache.open(("/tmp/.OpenScan3D/cameraCache.tmp"), ios::in);
    if (!cameraCache)
    {
        printf("Task failed,can't get more parameters,please check camera setting or use root user\n");
        Global::process = PROCESSERROR;
        return EXIT_FAILURE;
    }
    std::string model, sensorSize, focal;
    getline(cameraCache, model);
    getline(cameraCache, sensorSize);
    getline(cameraCache, focal);
    cameraCache.close();
    std::cout << model << "\n"
              << sensorSize << "\n"
              << focal << std::endl;
    customCamera.setModelName(model);
    customCamera.setSensorSize(std::stod(sensorSize));
    customCamera.setFocal(std::stod(focal));
    return EXIT_SUCCESS;
}

/**
 * @brief 任务处理
 */
void MsgProc(uint8_t msg)
{
    switch (msg)
    {
    case CMD_MATCHFEATURES:
    {

        write_to_file("MFS");

        Global::process = PROCESSWORKING;
        Global::saveProcess();
        std::string imagesInputDir;
        std::string sensorWidthDataBaseDir;
        std::string matchesOutputDir;
        std::string EigenMatrix; // EigenMatrixFormat"f;0;ppx;0;f;ppy;0;0;1"

        std::string describerMethod; // #1_ComputeFeatures
        std::string featureQuality;  // #1_ComputeFeatures
        bool upRight;
        bool forceCompute;

        std::string nearest_matching_method;
        std::string geometricModel;
        float distanceRatio;
        bool forceMatch;
        CustomCamera customCamera;
        if (getCustomCamera(customCamera) == EXIT_FAILURE)
            break;

        printf("\nTask Called: MATCHFEATURES \n\n");
        ifstream cmdCache;
        cmdCache.open(("/tmp/.OpenScan3D/cmdCache.tmp"), ios::in);
        if (!cmdCache)
        {
            printf("Task failed,can't get more parameters,no pointed directory\n");
            Global::process = PROCESSERROR;
            break;
        }

        std::string temp;
        getline(cmdCache, temp);
        if (temp != "matchfeature")
        {
            printf("Task failed,can't get more parameters\n");
            Global::process = PROCESSERROR;
            break;
        }

        getline(cmdCache, imagesInputDir);
        getline(cmdCache, sensorWidthDataBaseDir);
        getline(cmdCache, matchesOutputDir);
        getline(cmdCache, temp);

        EigenMatrix = temp;
        if (EigenMatrix == "NULL")
        {
            EigenMatrix = "";
        }

        getline(cmdCache, describerMethod);
        getline(cmdCache, featureQuality);
        getline(cmdCache, temp);

        if (temp == "0")
            upRight = false;
        else
            upRight = true;

        getline(cmdCache, temp);
        if (temp == "0")
            forceCompute = false;
        else
            forceCompute = true;
        getline(cmdCache, temp);
        distanceRatio = atof(temp.c_str());
        getline(cmdCache, temp);
        if (temp == "0")
            forceMatch = false;
        else
            forceMatch = true;
        getline(cmdCache, nearest_matching_method);
        getline(cmdCache, geometricModel);
        cmdCache.close();

        STATE_RETURN = IntrinsicsAnalysis(imagesInputDir, matchesOutputDir, sensorWidthDataBaseDir, customCamera);
        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("Load images failed\n");
            Global::process = PROCESSERROR;
            break;
        }
        sleep(2);
        STATE_RETURN = ComputeFeatures(matchesOutputDir + "/sfm_data.json", matchesOutputDir, describerMethod, featureQuality, upRight, forceCompute);
        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("Get feature info error\n");
            Global::process = PROCESSERROR;
            break;
        }
        STATE_RETURN = ComputePairs(matchesOutputDir, matchesOutputDir); // if use video mode switch "CONTIGUOUS"
        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("Compute Pairs error\n");
            Global::process = PROCESSERROR;
            break;
        }
        printf("Obtaining feature points is complete, ready to start matching feature points\n");
        sleep(2);
        STATE_RETURN = ComputeMatches(matchesOutputDir, matchesOutputDir, nearest_matching_method, distanceRatio);
        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("Get matched points info error\n");
            Global::process = PROCESSERROR;
            break;
        }

        STATE_RETURN = FilterMatches(matchesOutputDir, matchesOutputDir, geometricModel);
        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("Filter matches failed \n");
            Global::process = PROCESSERROR;
            break;
        }
        congmsgbuf mymsg;
        mymsg.mtype = 1;
        mymsg.data[0] = PROCESSCLOSE;
        sendMessage(mymsg);

        write_to_file("MFE");

        printf("==========Task Finished, Please Do The Next Step==========\n");

        break;
    }
    case CMD_SFMANDSFP:
    {
        write_to_file("SFMS");

        Global::process = PROCESSWORKING;
        Global::saveProcess();
        std::string inputDir;
        std::string outputDir;
        std::string sfmEngine;
        std::string on_off;

        // int triangulationMethod, resectionMethod;

        printf("\nTask called: SFM&SFP \n\n");
        ifstream cmdCache;
        cmdCache.open(("/tmp/.OpenScan3D/cmdCache.tmp"), ios::in);

        if (!cmdCache)
        {
            printf("Tasks Failed: Can't get more parameters\n");
            Global::process = PROCESSERROR;
            break;
        }

        std::string temp;
        getline(cmdCache, temp);

        if (temp != "sfm&sfp")
        {
            printf("Tasks Failed: Can't get more parameters\n");
            Global::process = PROCESSERROR;
            break;
        }
        getline(cmdCache, inputDir);
        getline(cmdCache, outputDir);
        getline(cmdCache, sfmEngine);
        getline(cmdCache, on_off);

        cmdCache.close();
        printf("=============Reconstruction Starting===========\n");
        if (sfmEngine == "GLOBAL")
        {
            STATE_RETURN = Reconstruction(inputDir, outputDir, sfmEngine);
            if (STATE_RETURN == EXIT_FAILURE)
            {
                printf("GlobalReconstruction failed \n");
                // Global::process = PROCESSERROR;
                break;
            }
        }
        else
        {
            STATE_RETURN = Reconstruction(inputDir, outputDir, sfmEngine);
            if (STATE_RETURN == EXIT_FAILURE)
            {
                printf("IncrementalReconstruction failed \n");
                // Global::process = PROCESSERROR;
                break;
            }
        }

        printf("=============Reconstruction Successfully===========\n");
        printf("=============ColorizeStructure Starting===========\n");
        STATE_RETURN = ColorizeStructure(outputDir, outputDir);
        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("ColorizeStructure failed \n");
            Global::process = PROCESSERROR;
            break;
        }

        printf("=============ColorizeStructure Successfully===========\n");
        printf("=============StructureFromKnownPoses Starting===========\n");
        STATE_RETURN = StructureFromKnownPoses(outputDir, outputDir, outputDir, outputDir, on_off);
        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("StructureFromKnownPoses failed \n");
            Global::process = PROCESSERROR;
            break;
        }

        printf("=============StructureFromKnownPoses Successfully===========\n");
        printf("=============ColorizedRobustTriangulation Starting===========\n");
        STATE_RETURN = ColorizedRobustTriangulation(outputDir, outputDir);
        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("ColorizedRobustTriangulation failed \n");
            Global::process = PROCESSERROR;
            break;
        }

        printf("=============ColorizedRobustTriangulation Successfully===========\n");
        printf("=============ExportToOpenMVS Starting===========\n");
        STATE_RETURN = ExportToOpenMVS(outputDir, outputDir, outputDir);
        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("ExportToOpenMVS failed \n");
            Global::process = PROCESSERROR;
            break;
        }

        printf("=============ExportToOpenMVS Successfully===========\n");

        write_to_file("SFME");

        printf("==========Task Finished, Please Do The Next Step==========\n");

        break;
    }
    case CMD_EXPORTDENSECLOUD:
    {

        write_to_file("DPS");

        Global::process = PROCESSWORKING;
        Global::saveProcess();
        std::string sceneDir;
        std::string outputDir;

        printf("\nTask called DENSIFYPOINTCLOUD \n\n");
        ifstream cmdCache;
        cmdCache.open(("/tmp/.OpenScan3D/cmdCache.tmp"), ios::in);
        if (!cmdCache)
        {
            printf("Tasks Failed: Can't get more parameters\n");
            Global::process = PROCESSERROR;
            break;
        }
        std::string temp;
        getline(cmdCache, temp);

        if (temp != "DENSIFYPOINTCLOUD")
        {
            printf("Tasks Failed: Can't get more parameters\n");
            Global::process = PROCESSERROR;
            break;
        }
        getline(cmdCache, sceneDir);
        getline(cmdCache, outputDir);
        cmdCache.close();
        sceneDir.append("/scene.mvs");

        char *cmd[10];
        cmd[0] = "DensifyPointCloud";
        cmd[1] = (char *)sceneDir.data();
        cmd[2] = "--dense-config-file";
        cmd[3] = "Densify.ini";
        cmd[4] = "--resolution-level";
        cmd[5] = "1";
        cmd[6] = "--number-views";
        cmd[7] = "-8";
        cmd[8] = "-w";
        cmd[9] = (char *)outputDir.data();

        std::cout << cmd[0] << endl;
        STATE_RETURN = DensifyPointCloud(10, cmd);
        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("DensifyPointCloud failed \n");
            Global::process = PROCESSERROR;
            break;
        }
        std::string sceneDenseDir = outputDir;

        write_to_file("DPE");

        printf("==========Task Finished, Please Do The Next Step==========\n");

        break;
    }
    case CMD_RECONSTRUCTMESH:
    {

        write_to_file("TRS");

        Global::process = PROCESSWORKING;
        Global::saveProcess();
        std::string scene_dense_meshDir;
        std::string outputDir;

        printf("\nTask called RECONSTRUCTMESH \n\n");
        ifstream cmdCache;

        cmdCache.open(("/tmp/.OpenScan3D/cmdCache.tmp"), ios::in);
        if (!cmdCache)
        {
            printf("Tasks Failed: Can't get more parameters\n");
            Global::process = PROCESSERROR;
            break;
        }

        std::string temp;
        getline(cmdCache, temp);

        if (temp != "RECONSTRUCTMESH")
        {
            printf("Tasks Failed: Can't get more parameters\n");
            Global::process = PROCESSERROR;
            break;
        }
        getline(cmdCache, scene_dense_meshDir);
        getline(cmdCache, outputDir);
        cmdCache.close();

        scene_dense_meshDir.append("/scene_dense.mvs");
        char *cmd[8];
        cmd[0] = "ReconstructMesh";
        cmd[1] = (char *)scene_dense_meshDir.data();
        cmd[2] = "-w";
        cmd[3] = (char *)outputDir.data();
        STATE_RETURN = ReconstructMesh(4, cmd);

        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("ReconstructTheMesh failed \n");
            Global::process = PROCESSERROR;
            break;
        }
        std::string scene_dense_mesh_dir = outputDir;
        scene_dense_mesh_dir.append("/scene_dense_mesh.mvs");

        cmd[0] = "RefineMesh";
        cmd[1] = (char *)scene_dense_mesh_dir.data();
        cmd[2] = "--scales";
        cmd[3] = "1";
        cmd[4] = "--gradient-step";
        cmd[5] = "25.05";
        cmd[6] = "-w";
        cmd[7] = (char *)outputDir.data();
        STATE_RETURN = RefineMesh(8, cmd);
        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("RefineTheMesh failed \n");
            Global::process = PROCESSERROR;
            break;
        }

        write_to_file("TRE");

        printf("==========Task Finished, Please Do The Next Step==========\n");

        break;
    }

    case CMD_TEXTUREMESH:
    {

        write_to_file("TMS");

        Global::process = PROCESSWORKING;
        Global::saveProcess();
        std::string scene_dense_mesh_refineDir;
        std::string outputDir;

        printf("\nTask called TEXTUREMESH \n\n");
        ifstream cmdCache;
        cmdCache.open(("/tmp/.OpenScan3D/cmdCache.tmp"), ios::in);

        if (!cmdCache)
        {
            printf("Tasks Failed: Can't get more parameters\n");
            Global::process = PROCESSERROR;
            break;
        }

        std::string temp;
        getline(cmdCache, temp);
        if (temp != "TEXTUREMESH")
        {
            printf("Tasks Failed: Can't get more parameters\n");
            Global::process = PROCESSERROR;
            break;
        }

        getline(cmdCache, scene_dense_mesh_refineDir);
        getline(cmdCache, outputDir);
        cmdCache.close();
        char *cmd[8];
        scene_dense_mesh_refineDir.append("/scene_dense_mesh_refine.mvs");

        cmd[0] = "TextureMesh";
        cmd[1] = (char *)scene_dense_mesh_refineDir.data();
        cmd[2] = "--decimate";
        cmd[3] = "0.5";
        cmd[4] = "-w";
        cmd[5] = (char *)outputDir.data();
        STATE_RETURN = TextureTheMesh(6, cmd);
        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("TextureTheMesh failed \n");
            Global::process = PROCESSERROR;
            break;
        }

        write_to_file("TME");

        printf("==========Task Finished, You Can View The Model Now==========\n");

        break;
    }

    case CMD_FULLAUTO:
    {

        write_to_file("MFS");

        Global::process = PROCESSWORKING;
        Global::saveProcess();
        std::string imagesInputDir;
        std::string sensorWidthDataBaseDir;
        std::string matchesOutputDir;
        std::string EigenMatrix;     // EigenMatrixFormat"f;0;ppx;0;f;ppy;0;0;1"
        std::string describerMethod; // #1_ComputeFeatures
        std::string featureQuality;  // #1_ComputeFeatures
        bool upRight;
        bool forceCompute;
        std::string nearest_matching_method;
        std::string geometricModel;
        float distanceRatio;
        bool forceMatch;

        std::string inputDir;
        std::string outputDir;
        std::string sfmEngine;
        std::string on_off;
        CustomCamera customCamera;
        if (getCustomCamera(customCamera) == EXIT_FAILURE)
            break;
        printf("\nTask Called: CMD_FULLAUTO \n\n");
        ifstream cmdCache;
        cmdCache.open(("/tmp/.OpenScan3D/cmdCache.tmp"), ios::in);
        if (!cmdCache)
        {
            printf("Task failed,can't get more parameters,no pointed directory\n");
            Global::process = PROCESSERROR;
            break;
        }

        std::string temp;
        getline(cmdCache, temp);
        if (temp != "CMD_FULLAUTO")
        {
            printf("Task failed,can't get more parameters\n");
            Global::process = PROCESSERROR;
            break;
        }

        getline(cmdCache, imagesInputDir);
        getline(cmdCache, sensorWidthDataBaseDir);
        getline(cmdCache, matchesOutputDir);
        getline(cmdCache, temp);

        EigenMatrix = temp;
        if (EigenMatrix == "NULL")
        {
            EigenMatrix = "";
        }

        getline(cmdCache, describerMethod); // 6特征点算子
        getline(cmdCache, featureQuality);  // 7特征点质量
        getline(cmdCache, temp);            // 8
        if (temp == "0")
            upRight = false;
        else
            upRight = true;

        getline(cmdCache, temp); // 9
        if (temp == "0")
            forceCompute = false;
        else
            forceCompute = true;

        getline(cmdCache, temp);            // 10
        distanceRatio = atof(temp.c_str()); // 10距离比率
        getline(cmdCache, temp);            // 11
        if (temp == "0")
            forceMatch = false;
        else
            forceMatch = true;
        getline(cmdCache, nearest_matching_method); // 12
        getline(cmdCache, geometricModel);          // 13

        getline(cmdCache, inputDir);  // 14
        getline(cmdCache, outputDir); // 15
        getline(cmdCache, sfmEngine); // 16
        getline(cmdCache, on_off);    // 17

        cmdCache.close();

        STATE_RETURN = IntrinsicsAnalysis(imagesInputDir, matchesOutputDir, sensorWidthDataBaseDir, customCamera);
        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("Load images failed\n");
            Global::process = PROCESSERROR;
            break;
        }

        STATE_RETURN = ComputeFeatures(matchesOutputDir + "/sfm_data.json", matchesOutputDir, describerMethod, featureQuality, upRight, forceCompute);
        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("Get feature info error\n");
            Global::process = PROCESSERROR;
            break;
        }
        printf("Obtaining feature points is complete, ready to start matching feature points\n");
        
        STATE_RETURN = ComputePairs(matchesOutputDir, matchesOutputDir); // if use video mode switch "CONTIGUOUS"
        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("Compute Pairs error\n");
            Global::process = PROCESSERROR;
            break;
        }
        STATE_RETURN = ComputeMatches(matchesOutputDir, matchesOutputDir, nearest_matching_method, distanceRatio);
        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("Get matched points info error\n");
            Global::process = PROCESSERROR;
            break;
        }

        STATE_RETURN = FilterMatches(matchesOutputDir, matchesOutputDir, geometricModel);
        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("Filter matches failed \n");
            Global::process = PROCESSERROR;
            break;
        }
        congmsgbuf mymsg;
        mymsg.mtype = 1;
        mymsg.data[0] = PROCESSCLOSE;
        sendMessage(mymsg);

        write_to_file("MFE");
        sleep(1);
        write_to_file("SFMS");

        printf("=============Reconstruction Starting===========\n");

        if (sfmEngine == "GLOBAL")
        {
            STATE_RETURN = Reconstruction(inputDir, outputDir, sfmEngine);
            if (STATE_RETURN == EXIT_FAILURE)
            {
                printf("GlobalReconstruction failed \n");
                Global::process = PROCESSERROR;
                break;
            }
        }
        else
        {
            STATE_RETURN = Reconstruction(inputDir, outputDir, sfmEngine);
            if (STATE_RETURN == EXIT_FAILURE)
            {
                printf("IncrementalReconstruction failed \n");
                Global::process = PROCESSERROR;
                break;
            }
        }

        printf("=============Reconstruction Successfully===========\n");
        printf("=============ColorizeStructure Starting===========\n");

        STATE_RETURN = ColorizeStructure(outputDir, outputDir);
        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("ColorizeStructure failed \n");
            Global::process = PROCESSERROR;
            break;
        }

        printf("=============ColorizeStructure Successfully===========\n");
        printf("=============StructureFromKnownPoses Starting===========\n");

        STATE_RETURN = StructureFromKnownPoses(outputDir, outputDir, outputDir, outputDir, on_off);
        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("StructureFromKnownPoses failed \n");
            Global::process = PROCESSERROR;
            break;
        }

        printf("=============StructureFromKnownPoses Successfully===========\n");
        printf("=============ColorizedRobustTriangulation Starting===========\n");
        STATE_RETURN = ColorizedRobustTriangulation(outputDir, outputDir);

        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("ColorizedRobustTriangulation failed \n");
            Global::process = PROCESSERROR;
            break;
        }

        printf("=============ColorizedRobustTriangulation Successfully===========\n");
        printf("=============ExportToOpenMVS Starting===========\n");
        STATE_RETURN = ExportToOpenMVS(outputDir, outputDir, outputDir);

        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("ExportToOpenMVS failed \n");
            Global::process = PROCESSERROR;
            break;
        }
        printf("=============ExportToOpenMVS Successfully===========\n");

        write_to_file("SFME");
        sleep(1);
        write_to_file("DPS");

        printf("=============DensifyPointCloud starting===========\n");

        std::string sceneDir = outputDir;

        // printf(outputDir);
        sceneDir.append("/scene.mvs");
        char *cmd1[10];

        cmd1[0] = "DensifyPointCloud";
        cmd1[1] = (char *)sceneDir.data();
        cmd1[2] = "--dense-config-file";
        cmd1[3] = "Densify.ini";
        cmd1[4] = "--resolution-level";
        cmd1[5] = "1";
        cmd1[6] = "--number-views";
        cmd1[7] = "-8";
        cmd1[8] = "-w";
        cmd1[9] = (char *)outputDir.data();

        std::cout << cmd1[0] << endl;
        STATE_RETURN = DensifyPointCloud(10, cmd1);
        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("DensifyPointCloud failed \n");
            Global::process = PROCESSERROR;
            break;
        }
        // outputDir = outputDir1Dir;

        printf("=============DensifyPointCloud Successfully===========\n");

        write_to_file("DPE");
        sleep(1);
        write_to_file("TRS");

        printf("=============ReconstructMesh starting===========\n");
        std::string scene_denseDir = outputDir;

        scene_denseDir.append("/scene_dense.mvs");

        char *cmd2[5];
        cmd2[0] = "ReconstructMesh";
        cmd2[1] = (char *)scene_denseDir.data();
        cmd2[2] = "-w";
        cmd2[3] = (char *)outputDir.data();

        STATE_RETURN = ReconstructMesh(4, cmd2);
        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("ReconstructTheMesh failed \n");
            Global::process = PROCESSERROR;
            break;
        }

        // outputDir = outputDir2Dir;
        printf("=============ReconstructMesh Successfully===========\n");
        printf("=============RefineMesh starting===========\n");
        std::string scene_dense_meshDir = outputDir;

        scene_dense_meshDir.append("/scene_dense_mesh.mvs");
        char *cmd3[8];
        cmd3[0] = "RefineMesh";
        cmd3[1] = (char *)scene_dense_meshDir.data();
        cmd3[2] = "--scales";
        cmd3[3] = "1";
        cmd3[4] = "--gradient-step";
        cmd3[5] = "25.05";
        cmd3[6] = "-w";
        cmd3[7] = (char *)outputDir.data();

        STATE_RETURN = RefineMesh(8, cmd3);
        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("RefineTheMesh failed \n");
            Global::process = PROCESSERROR;
            break;
        }
        // outputDir = outputDir3Dir;

        printf("=============RefineMesh Successfully===========\n");

        write_to_file("TRE");
        sleep(1);
        write_to_file("TMS");

        write_to_file("92");

        printf("=============TextureMesh starting===========\n");
        char *cmd4[8];
        std::string scene_dense_mesh_refineDir = outputDir;
        scene_dense_mesh_refineDir.append("/scene_dense_mesh_refine.mvs");

        cmd4[0] = "TextureMesh";
        cmd4[1] = (char *)scene_dense_mesh_refineDir.data();
        cmd4[2] = "--decimate";
        cmd4[3] = "0.5";
        cmd4[4] = "-w";
        cmd4[5] = (char *)outputDir.data();
        STATE_RETURN = TextureTheMesh(6, cmd4);

        if (STATE_RETURN == EXIT_FAILURE)
        {
            printf("TextureTheMesh failed \n");
            Global::process = PROCESSERROR;
            break;
        }

        write_to_file("TME");

        printf("=============TextureMesh Successfully===========\n");

        printf("Everything is OK");

        break;
    }
    case CMD_EXPORTSTL:
    {
        ifstream cmdCache;
        cmdCache.open(("/tmp/.OpenScan3D/cmdCache.tmp"), ios::in);
        if (!cmdCache)
        {
            printf("Task failed,can't get more parameters,no pointed directory\n");
            Global::process = PROCESSERROR;
            break;
        }
        std::string tmp, sourceFile, definiteDir;
        getline(cmdCache, tmp);
        if (tmp != "CMD_EXPORTSTL")
        {
            printf("Task failed,can't get more parameters\n");
            Global::process = PROCESSERROR;
            break;
        }
        getline(cmdCache, sourceFile);
        getline(cmdCache, definiteDir);
        definiteDir.append("/ExportSTL.stl");
        char *cmd[8];
        cmd[0] = "obj2stl";
        cmd[1] = (char *)sourceFile.data();
        cmd[2] = "-o";
        cmd[3] = (char *)definiteDir.data();
        obj2stl(4, cmd);
        printf("EXPORT SUCCESSFULLY\n");
        break;
    }
    case CMD_EXPORTPLY:
    {
        ifstream cmdCache;
        cmdCache.open(("/tmp/.OpenScan3D/cmdCache.tmp"), ios::in);
        if (!cmdCache)
        {
            printf("Task failed,can't get more parameters,no pointed directory\n");
            Global::process = PROCESSERROR;
            break;
        }
        std::string tmp, sourceFile, definiteDir;
        getline(cmdCache, tmp);
        if (tmp != "CMD_EXPORTPLY")
        {
            printf("Task failed,can't get more parameters\n");
            Global::process = PROCESSERROR;
            break;
        }
        getline(cmdCache, sourceFile);
        getline(cmdCache, definiteDir);
        definiteDir.append("/ExportPlY"); // fileName
        auto tmpScene = new MVS::Scene();
        if (!tmpScene->Load(sourceFile))
        {
            printf("no source\n");
            free(tmpScene);
            break;
        }
        tmpScene->mesh.Save(definiteDir + ".ply");
        printf("EXPORT SUCCESSFULLY\n");
        free(tmpScene);
        break;
    }
    case CMD_EXPORTOBJ:
    {
        ifstream cmdCache;
        cmdCache.open(("/tmp/.OpenScan3D/cmdCache.tmp"), ios::in);
        if (!cmdCache)
        {
            printf("Task failed,can't get more parameters,no pointed directory\n");
            Global::process = PROCESSERROR;
            break;
        }
        std::string tmp, sourceFile, definiteDir;
        getline(cmdCache, tmp);
        if (tmp != "CMD_EXPORTOBJ")
        {
            printf("Task failed,can't get more parameters\n");
            Global::process = PROCESSERROR;
            break;
        }
        getline(cmdCache, sourceFile);
        getline(cmdCache, definiteDir);
        definiteDir.append("/ExportOBJ"); // filename
        auto tmpScene = new MVS::Scene();
        if (!tmpScene->Load(sourceFile))
        {
            printf("no source\n");
            free(tmpScene);
            break;
        }
        tmpScene->mesh.Save(definiteDir + ".obj");
        printf("EXPORT SUCCESSFULLY\n");
        free(tmpScene);
        break;
    }

    default:
        break;
    }
}

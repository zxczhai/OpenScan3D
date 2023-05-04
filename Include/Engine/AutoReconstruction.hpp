#include <iostream>
#include <Global.h>
#include <string>
#include <dirent.h>

int checkDirExist(std::string pathToDir)
{
    DIR *dir;
    if ((dir = opendir(pathToDir.c_str())) == NULL)
        return PROCESSERROR;
    return PROCESSCLOSE;
}
/**
 * @brief 任务处理
 */
void MsgProc(std::string msg)
{
}

/**
 * @brief 分析并提取相机内参#1
 */
int IntrinsicsAnalysis(std::string inputImageDir, std::string outputDir, std::string cameraDBDir)
{
    // test Dir Exist is useful
    //  if(checkDirExist(inputImageDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR || checkDirExist(cameraDBDir)==PROCESSERROR)
    //  {
    //      system("echo directory not exist");
    //      return PROCESSERROR;
    //  }
    system("echo IntrinsicsAnalysis working.........");
    std::string cmd = "";
    cmd.append("openMVG_main_SfMInit_ImageListing");
    cmd.append(" ");
    cmd.append("-i");
    cmd.append(" ");
    cmd.append(inputImageDir);
    cmd.append(" ");
    cmd.append("-o");
    cmd.append(" ");
    cmd.append(outputDir);
    cmd.append(" ");
    cmd.append("-d");
    cmd.append(" ");
    cmd.append(cameraDBDir);
    system(cmd.c_str());
    system("echo IntrinsicsAnalysis Successfully.........");
    return PROCESSCLOSE;
}

/**
 * @brief 提取图像特征点#2
 *
 */
int ComputeFeatures(std::string sfmDataDir, std::string outputDir, std::string describerMethod = "SIFT", std::string describerPreset = "NORMAL")
{
    // if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
    // {
    //     system("echo directory not exist");
    //     return PROCESSERROR;
    // }
    system("echo ComputeFeatures working.........");
    std::string cmd = "";
    cmd.append("openMVG_main_ComputeFeatures");
    cmd.append(" ");
    cmd.append("-i");
    cmd.append(" ");
    cmd.append(sfmDataDir);
    cmd.append("/");
    cmd.append("sfm_data.json");
    cmd.append(" ");
    cmd.append("-o");
    cmd.append(" ");
    cmd.append(outputDir);
    cmd.append(" ");
    cmd.append("-m");
    cmd.append(" ");
    cmd.append(describerMethod);
    cmd.append(" ");
    cmd.append("-p");
    cmd.append(" ");
    cmd.append(describerPreset);
    system(cmd.c_str());
    system("echo ComputeFeatures Successfully.........");
    return PROCESSCLOSE;
}
/**
 * @brief 构建图像匹配对#3
 */
int ComputePairs(std::string sfmDataDir, std::string outputDir)
{
    // if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
    // {
    //     system("echo directory not exist");
    //     return PROCESSERROR;
    // }
    system("echo ComputePairs working.........");
    std::string cmd = "";
    cmd.append("openMVG_main_PairGenerator");
    cmd.append(" ");
    cmd.append("-i");
    cmd.append(" ");
    cmd.append(sfmDataDir);
    cmd.append("/");
    cmd.append("sfm_data.json");
    cmd.append(" ");
    cmd.append("-o");
    cmd.append(" ");
    cmd.append(outputDir);
    cmd.append("/");
    cmd.append("pairs.bin");
    system(cmd.c_str());
    system("echo ComputePairs Successfully.........");
    return PROCESSCLOSE;
}
/**
 * @brief 计算图像间的特征点匹配#4
 */
int ComputeMatches(std::string sfmDataDir, std::string outputDir, std::string nearest_matching_method = "AUTO", std::string describerPreset = "NORMAL")
{
    // if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
    // {
    //     system("echo directory not exist");
    //     return PROCESSERROR;
    // }
    system("echo ComputeMatches working.........");
    std::string cmd = "";
    cmd.append("openMVG_main_ComputeMatches");
    cmd.append(" ");
    cmd.append("-i");
    cmd.append(" ");
    cmd.append(sfmDataDir);
    cmd.append("/");
    cmd.append("sfm_data.json");
    cmd.append(" ");
    cmd.append("-p");
    cmd.append(" ");
    cmd.append(sfmDataDir);
    cmd.append("/");
    cmd.append("pairs.bin");
    cmd.append(" ");
    cmd.append("-o");
    cmd.append(" ");
    cmd.append(outputDir);
    cmd.append("/");
    cmd.append("matches.putative.bin");
    cmd.append(" ");
    cmd.append("-n");
    cmd.append(" ");
    cmd.append(nearest_matching_method);
    system(cmd.c_str());
    system("echo ComputeMatches Successfully.........");
    return PROCESSCLOSE;
}

/**
 * @brief 使用几何模型过滤不正确的匹配#5
 */
int FilterMatches(std::string sfmDataDir, std::string outputDir)
{
    // if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
    // {
    //     system("echo directory not exist");
    //     return PROCESSERROR;
    // }
    system("echo FilterMatches working.........");
    std::string cmd = "";
    cmd.append("openMVG_main_GeometricFilter");
    cmd.append(" ");
    cmd.append("-i");
    cmd.append(" ");
    cmd.append(sfmDataDir);
    cmd.append("/");
    cmd.append("sfm_data.json");
    cmd.append(" ");
    cmd.append("-m");
    cmd.append(" ");
    cmd.append(sfmDataDir);
    cmd.append("/");
    cmd.append("matches.putative.bin");
    cmd.append(" ");
    cmd.append("-o");
    cmd.append(" ");
    cmd.append(outputDir);
    cmd.append("/");
    cmd.append("matches.f.bin");
    system(cmd.c_str());
    system("echo Filter matches Successfully.........");
    return PROCESSCLOSE;
}

/**
 * @brief 进行增量式的三维重建#6
 */
int IncrementalReconstruction(std::string sfmDataDir, std::string outputDir)
{
    // if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
    // {
    //     system("echo directory not exist");
    //     return PROCESSERROR;
    // }
    system("echo Incremental reconstruction working.........");
    std::string cmd = "";
    cmd.append("openMVG_main_SfM");
    cmd.append(" ");
    cmd.append("-i");
    cmd.append(" ");
    cmd.append(sfmDataDir);
    cmd.append("/");
    cmd.append("sfm_data.json");
    cmd.append(" ");
    cmd.append("-m");
    cmd.append(" ");
    cmd.append(sfmDataDir);
    cmd.append(" ");
    cmd.append("-o");
    cmd.append(" ");
    cmd.append(outputDir);
    cmd.append(" ");
    cmd.append("-s");
    cmd.append(" ");
    cmd.append("INCREMENTAL");
    system(cmd.c_str());
    system("echo Incremental reconstruction Successfully.........");
    return PROCESSCLOSE;
}
/**
 * @brief 将增量式的结果进行全局融合，得到更完整的三维模型#7
 */
int GlobalReconstruction(std::string sfmDataDir, std::string outputDir)
{
    // 判断需要的文件是否存在
    //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
    //  {
    //      system("echo directory not exist");
    //      return PROCESSERROR;
    //  }
    system("echo GlobalReconstruction working.........");
    std::string cmd = "";
    cmd.append("openMVG_main_SfM");
    cmd.append(" ");
    cmd.append("-i");
    cmd.append(" ");
    cmd.append(sfmDataDir);
    cmd.append("/");
    cmd.append("sfm_data.json");
    cmd.append(" ");
    cmd.append("-m");
    cmd.append(" ");
    cmd.append(sfmDataDir);
    cmd.append(" ");
    cmd.append("-o");
    cmd.append(" ");
    cmd.append(outputDir);
    cmd.append(" ");
    cmd.append("-s");
    cmd.append(" ");
    cmd.append("GLOBAL");
    cmd.append(" ");
    cmd.append("-M");
    cmd.append(" ");
    cmd.append(outputDir);
    cmd.append("/");
    cmd.append("matches.e.bin");
    system(cmd.c_str());
    system("echo GlobalReconstruction Successfully.........");
    return PROCESSCLOSE;
}

/**
 * @brief 为三维模型添加颜色信息#8
 */
int ColorizeStructure(std::string sfmDataDir, std::string outputDir)
{
    // 判断需要的文件是否存在
    //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
    //  {
    //      system("echo directory not exist");
    //      return PROCESSERROR;
    //  }
    system("echo ColorizeStructure working.........");
    std::string cmd = "";
    cmd.append("openMVG_main_ComputeSfM_DataColor");
    cmd.append(" ");
    cmd.append("-i");
    cmd.append(" ");
    cmd.append(sfmDataDir);
    cmd.append("/");
    cmd.append("sfm_data.bin");
    cmd.append(" ");
    cmd.append("-o");
    cmd.append(" ");
    cmd.append(outputDir);
    cmd.append("/");
    cmd.append("colorized.ply");
    system(cmd.c_str());
    system("echo ColorizeStructure Successfully.........");
    return PROCESSCLOSE;
}

/**
 * @brief 利用已知位姿的情况下，提取更多的三维结构信息#9
 */
int StructureFromKnownPoses(std::string sfmDataDir, std::string outputDir)
{
    // 判断需要的文件是否存在
    //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
    //  {
    //      system("echo directory not exist");
    //      return PROCESSERROR;
    //  }
    system("echo StructureFromKnownPoses working.........");
    std::string cmd = "";
    cmd.append("openMVG_main_ComputeStructureFromKnownPoses");
    cmd.append(" ");
    cmd.append("-i");
    cmd.append(" ");
    cmd.append(sfmDataDir);
    cmd.append("/");
    cmd.append("sfm_data.bin");
    cmd.append(" ");
    cmd.append("-m");
    cmd.append(" ");
    cmd.append(outputDir);
    cmd.append(" ");
    cmd.append("-f");
    cmd.append(" ");
    cmd.append(outputDir);
    cmd.append("/");
    cmd.append("matches.f.bin");
    cmd.append(" ");
    cmd.append("-o");
    cmd.append(" ");
    cmd.append(outputDir);
    cmd.append("/");
    cmd.append("robust.bin");
    system(cmd.c_str());
    system("echo StructureFromKnownPoses Successfully.........");
    return PROCESSCLOSE;
}

/**
 * @brief 增加三维重建结果的稳健性和可靠性#10
 */
int ColorizedRobustTriangulation(std::string sfmDataDir, std::string outputDir)
{
    // 判断需要的文件是否存在
    //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
    //  {
    //      system("echo directory not exist");
    //      return PROCESSERROR;
    //  }
    system("echo ColorizedRobustTriangulation working.........");
    std::string cmd = "";
    cmd.append("openMVG_main_ComputeSfM_DataColor");
    cmd.append(" ");
    cmd.append("-i");
    cmd.append(" ");
    cmd.append(sfmDataDir);
    cmd.append("/");
    cmd.append("robust.bin");
    cmd.append(" ");
    cmd.append("-o");
    cmd.append(" ");
    cmd.append(outputDir);
    cmd.append("/");
    cmd.append("robust_colorized.ply");
    system(cmd.c_str());
    system("echo ColorizedRobustTriangulation Successfully.........");
    return PROCESSCLOSE;
}

/**
 * @brief 进行控制点的注册，以便在不同数据集之间进行对比和匹配#11
 */
int ControlPointsRegistration(std::string sfmDataDir, std::string outputDir)
{
    // 判断需要的文件是否存在
    //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
    //  {
    //      system("echo directory not exist");
    //      return PROCESSERROR;
    //  }
    system("echo ControlPointsRegistration working.........");
    std::string cmd = "";
    cmd.append("ui_openMVG_control_points_registration");
    cmd.append(" ");
    cmd.append("-i");
    cmd.append(" ");
    cmd.append(sfmDataDir);
    cmd.append("/");
    cmd.append("sfm_data.bin");
    system(cmd.c_str());
    system("echo ControlPointsRegistration Successfully.........");
    return PROCESSCLOSE;
}

/**
 * @brief 输出为 openMVS 格式，以便进行后续的密集重建#12
 */
int ExportToOpenMVS(std::string sfmDataDir, std::string outputDir)
{
    // 判断需要的文件是否存在
    //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
    //  {
    //      system("echo directory not exist");
    //      return PROCESSERROR;
    //  }
    system("echo ExportToOpenMVS working.........");
    std::string cmd = "";
    cmd.append("openMVG_main_openMVG2openMVS");
    cmd.append(" ");
    cmd.append("-i");
    cmd.append(" ");
    cmd.append(sfmDataDir);
    cmd.append("/");
    cmd.append("sfm_data.bin");
    cmd.append(" ");
    cmd.append("-o");
    cmd.append(" ");
    cmd.append(outputDir);
    cmd.append("/");
    cmd.append("scene.mvs");
    cmd.append(" ");
    cmd.append("-d");
    cmd.append(" ");
    cmd.append(outputDir);
    cmd.append("/");
    cmd.append("images");
    system(cmd.c_str());
    system("echo ExportToOpenMVS Successfully.........");
    return PROCESSCLOSE;
}

/**
 * @brief 密集化点云，以得到更精细的三维模型#13
 */
int DensifyPointCloud(std::string mvsDataDir, std::string outputDir)
{
    // 判断需要的文件是否存在
    //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
    //  {
    //      system("echo directory not exist");
    //      return PROCESSERROR;
    //  }
    system("echo DensifyPointCloud working.........");
    std::string cmd = "";
    cmd.append("../Binary/OpenMVS/DensifyPointCloud");
    cmd.append(" ");
    cmd.append(mvsDataDir);
    cmd.append("/");
    cmd.append("scene.mvs");
    cmd.append(" ");
    cmd.append("--dense-config-file");
    cmd.append(" ");
    cmd.append("Densify.ini");
    cmd.append(" ");
    cmd.append("--resolution-level");
    cmd.append(" ");
    cmd.append("1");
    cmd.append(" ");
    cmd.append("--number-views");
    cmd.append(" ");
    cmd.append("-8");
    cmd.append(" ");
    cmd.append("-w");
    cmd.append(" ");
    cmd.append(outputDir);
    system(cmd.c_str());
    system("echo DensifyPointCloud Successfully.........");
    return PROCESSCLOSE;
}

/**
 * @brief  基于点云重建网格模型#14
 */
int ReconstructTheMesh(std::string mvsDataDir, std::string outputDir)
{
    // 判断需要的文件是否存在
    //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
    //  {
    //      system("echo directory not exist");
    //      return PROCESSERROR;
    //  }
    system("echo ReconstructMesh working.........");
    std::string cmd = "";
    cmd.append("../Binary/OpenMVS/ReconstructMesh");
    cmd.append(" ");
    cmd.append(mvsDataDir);
    cmd.append("/");
    cmd.append("scene_dense.mvs");
    cmd.append(" ");
    cmd.append("-w");
    cmd.append(" ");
    cmd.append(outputDir);
    system(cmd.c_str());
    system("echo ReconstructMesh Successfully.........");
    return PROCESSCLOSE;
}

/**
 * @brief   对网格模型进行细化，使其更加精细#15
 */
int RefineTheMesh(std::string mvsDataDir, std::string outputDir)
{
    // 判断需要的文件是否存在
    //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
    //  {
    //      system("echo directory not exist");
    //      return PROCESSERROR;
    //  }
    system("echo RefineTheMesh working.........");
    std::string cmd = "";
    cmd.append("../Binary/OpenMVS/RefineMesh");
    cmd.append(" ");
    cmd.append(mvsDataDir);
    cmd.append("/");
    cmd.append("scene_dense_mesh.mvs");
    cmd.append(" ");
    cmd.append("--scales");
    cmd.append(" ");
    cmd.append("1");
    cmd.append(" ");
    cmd.append("--gradient-step");
    cmd.append(" ");
    cmd.append("25.05");
    cmd.append(" ");
    cmd.append("-w");
    cmd.append(" ");
    cmd.append(outputDir);
    system(cmd.c_str());
    system("echo RefineTheMesh Successfully.........");
    return PROCESSCLOSE;
}

/**
 * @brief  为网格模型添加纹理信息，以提高视觉效果#16
 */
int TextureTheMesh(std::string mvsDataDir, std::string outputDir)
{
    // 判断需要的文件是否存在
    //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
    //  {
    //      system("echo directory not exist");
    //      return PROCESSERROR;
    //  }
    system("echo TextureTheMesh working.........");
    std::string cmd = "";
    cmd.append("../Binary/OpenMVS/TextureMesh");
    cmd.append(" ");
    cmd.append(mvsDataDir);
    cmd.append("/");
    cmd.append("scene_dense_mesh_refine.mvs");
    cmd.append(" ");
    cmd.append("--decimate");
    cmd.append(" ");
    cmd.append("0.5");
    cmd.append(" ");
    cmd.append("-w");
    cmd.append(" ");
    cmd.append(outputDir);
    system(cmd.c_str());
    system("echo TextureTheMesh Successfully.........");
    return PROCESSCLOSE;
}

/**
 * @brief   估计视差图，以便进行后续的深度图融合#17
 */
int EstimateDisparityMaps(std::string mvsDataDir, std::string outputDir)
{
    // 判断需要的文件是否存在
    //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
    //  {
    //      system("echo directory not exist");
    //      return PROCESSERROR;
    //  }
    system("echo EstimateDisparityMaps working.........");
    std::string cmd = "";
    cmd.append("../Binary/OpenMVS/DensifyPointCloud");
    cmd.append(" ");
    cmd.append(mvsDataDir);
    cmd.append("/");
    cmd.append("scene.mvs");
    cmd.append(" ");
    cmd.append("--dense-config-file");
    cmd.append(" ");
    cmd.append("Densify.ini");
    cmd.append(" ");
    cmd.append("--fusion-mode");
    cmd.append(" ");
    cmd.append("-1");
    cmd.append(" ");
    cmd.append("-w");
    cmd.append(" ");
    cmd.append(outputDir);
    system(cmd.c_str());
    system("echo EstimateDisparityMaps Successfully.........");
    return PROCESSCLOSE;
}

/**
 * @brief   将多个视差图融合，得到最终的深度图#18
 */
int FuseDisparityMaps(std::string mvsDataDir, std::string outputDir)
{
    // 判断需要的文件是否存在
    //  if(checkDirExist(sfmDataDir)==PROCESSERROR || checkDirExist(outputDir)==PROCESSERROR)
    //  {
    //      system("echo directory not exist");
    //      return PROCESSERROR;
    //  }
    system("echo FuseDisparityMaps working.........");
    std::string cmd = "";
    cmd.append("../Binary/OpenMVS/DensifyPointCloud");
    cmd.append(" ");
    cmd.append(mvsDataDir);
    cmd.append("/");
    cmd.append("scene.mvs");
    cmd.append(" ");
    cmd.append("--dense-config-file");
    cmd.append(" ");
    cmd.append("Densify.ini");
    cmd.append(" ");
    cmd.append("--fusion-mode");
    cmd.append(" ");
    cmd.append("-2");
    cmd.append(" ");
    cmd.append("-w");
    cmd.append(" ");
    cmd.append(outputDir);
    // cmd.append("/final");
    cmd.append(" ");
    system(cmd.c_str());
    system("echo FuseDisparityMaps Successfully.........");
    return PROCESSCLOSE;
}

/**
 * @brief images2mvs Test
 */
int AutoR3d(std::string inputImageDir, std::string outputDir)
{
    IntrinsicsAnalysis(inputImageDir, outputDir, "../sensor_width_camera_database.txt");
    ComputeFeatures(outputDir, outputDir);
    ComputePairs(outputDir, outputDir);
    ComputeMatches(outputDir, outputDir);
    FilterMatches(outputDir, outputDir);
    IncrementalReconstruction(outputDir, outputDir);
    GlobalReconstruction(outputDir, outputDir);
    ColorizeStructure(outputDir, outputDir);
    StructureFromKnownPoses(outputDir, outputDir);
    ColorizedRobustTriangulation(outputDir, outputDir);
    //ControlPointsRegistration(outputDir, outputDir);
    ExportToOpenMVS(outputDir, outputDir);
    DensifyPointCloud(outputDir, outputDir);
    ReconstructTheMesh(outputDir, outputDir);
    RefineTheMesh(outputDir, outputDir);
    TextureTheMesh(outputDir, outputDir);
    EstimateDisparityMaps(outputDir, outputDir);
    FuseDisparityMaps(outputDir, outputDir);
    return PROCESSCLOSE;
}
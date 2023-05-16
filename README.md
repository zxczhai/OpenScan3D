# OpenScan3D
OpenScan3D是基于openmvg和openmvs二次开发的三维重建软件，旨在提供一个快速、易用、高精度的三维扫描解决方案。该软件支持多种输入数据源，包括单张图片、视频、点云等，并且可以进行自动对齐、稠密重建、纹理映射等处理，生成高质量的三维模型。  OpenScan3D还支持多种输出格式，包括OBJ、PLY、STL等，可以方便地导出到其他三维应用程序进行后续处理。此外，该软件还提供了一个可视化界面，方便用户交互和调整参数，使得三维扫描变得更加友好和简单。

```shell
#1内参分析
openMVG_main_SfMInit_ImageListing 
-i /home/zxc/3DReconstruction/testImages 
-d /home/zxc/3DReconstruction/sensor_width_camera_database.txt 
-o /home/zxc/3DReconstruction/output

#2计算特征
openMVG_main_ComputeFeatures 
-i /home/zxc/3DReconstruction/output/sfm_data.json 
-o /home/zxc/3DReconstruction/output 
-m SIFT 
-p NORMAL

#3计算匹配对 可不需要
openMVG_main_PairGenerator 
-i /home/zxc/3DReconstruction/output/sfm_data.json 
-m EXHAUSTIVE 
-o /home/zxc/3DReconstruction/output/pairs.bin

#4特征点匹配
openMVG_main_ComputeMatches 
-i /home/zxc/3DReconstruction/output/sfm_data.json 
-o /home/zxc/3DReconstruction/output/featurePointMatch.bin 
-p /home/zxc/3DReconstruction/output/pairs.bin 
-r 0.8 
-n AUTO

#5过滤匹配
openMVG_main_GeometricFilter 
-i /home/zxc/3DReconstruction/output/sfm_data.json 
-m /home/zxc/3DReconstruction/output/featurePointMatch.bin 
-o /home/zxc/3DReconstruction/output/matches_filtered.bin 
-g f
# -p /home/zxc/3DReconstruction/output/pairs.bin #对pairs.bin文件进行过滤,下方需设置-s参数保存过滤后的匹配点文件,否则后续会重建失败
# -s /home/zxc/3DReconstruction/output/filtered_pairs.bin 


#6增量式三维重建
openMVG_main_SfM 
-i /home/zxc/3DReconstruction/output/sfm_data.json 
-m /home/zxc/3DReconstruction/output 
-o /home/zxc/3DReconstruction/output/incrementalSFM 
-s INCREMENTAL 
-M matches_filtered.bin

#7
```
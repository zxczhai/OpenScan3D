# OpenScan3D
OpenScan3D是基于openmvg和openmvs二次开发的三维重建软件，旨在提供一个快速、易用、高精度的三维扫描解决方案。该软件支持多种输入数据源，包括单张图片、视频、点云等，并且可以进行自动对齐、稠密重建、纹理映射等处理，生成高质量的三维模型。  OpenScan3D还支持多种输出格式，包括OBJ、PLY、STL等，可以方便地导出到其他三维应用程序进行后续处理。此外，该软件还提供了一个可视化界面，方便用户交互和调整参数，使得三维扫描变得更加友好和简单。

# How to start
### Dependencies
* [Eigen](http://eigen.tuxfamily.org) version 3.4 or higher
* [OpenCV](http://opencv.org) version 2.4 or higher
* [Ceres](http://ceres-solver.org) version 1.10 or higher (optional)
* [CGAL](http://www.cgal.org) version 4.2 or higher
* [Boost](http://www.boost.org) version 1.56 or higher
* [VCG](http://vcg.isti.cnr.it/vcglib)
* [CUDA](https://developer.nvidia.com/cuda-downloads) (optional)
* [GLFW](http://www.glfw.org) (optional)
* [QT](https://www.qt.io/) version 5.14.2 or higher
### Clone
``` shell 
git clone --recursive git@github.com:zxczhai/OpenScan3D.git
```
### BUILD
``` shell
cd OpenScan3D
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ../
make -j8
```
### RUN
``` shell
#run Engine
cd build/bin
./R3D
#run GUI
cd build/bin
./OpenScan3D
```
## TODO:
- [x] 重构代码
- [x] 模型多种输出格式,OBJ&PLY&STL
- [x] 添加视频输入源
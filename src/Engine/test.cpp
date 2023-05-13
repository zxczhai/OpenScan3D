#include <iostream>
#include "AutoReconstruction.hpp"
int main()
{
    std::string imageDir,outputDir;
    printf("Please Enter ImageDir:\n");
    imageDir="/home/zxc/r3d/download/openMVS/test";
    // cin>>imageDir;
    printf("Please Enter outputDir:\n");
    // cin>>outputDir;
    outputDir="/home/zxc/r3d/download/openMVS/output_image";
    AutoR3d(imageDir,outputDir);
}
    
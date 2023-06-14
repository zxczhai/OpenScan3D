#ifndef _INTRINSICSANALYSIS_H
#define _INTRINSICSANALYSIS_H
#include <iostream>
#include "Camera.hpp"
int IntrinsicsAnalysis(std::string inputImageDir, std::string outputDir, std::string cameraDBDir,CustomCamera& customCamera);
#endif
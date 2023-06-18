#ifndef _PAIRGENERATOR_H
#define _PAIRGENERATOR_H
#include <iostream>

int ComputePairs(std::string sfmDataDir, std::string outputDir,std::string PairMode = "EXHAUSTIVE", int ContiguousCount = 3);
#endif
#ifndef _MYUTILS_H
#define _MYUTILS_H
#include "openMVG/sfm/sfm_data.hpp"
#include "openMVG/sfm/sfm_data_io.hpp"
#include "openMVG/sfm/sfm_data_colorization.hpp"
#include "openMVG/system/logger.hpp"
#include "openMVG/types.hpp"

#include "software/SfM/SfMPlyHelper.hpp"
#include "ColorizeStructure.hpp"
using namespace openMVG;
using namespace openMVG::sfm;
void GetCameraPositions(const SfM_Data &sfm_data, std::vector<Vec3> &vec_camPosition);

#endif
#include "Utils.hpp"

/// Export camera poses positions as a Vec3 vector
void GetCameraPositions(const SfM_Data &sfm_data, std::vector<Vec3> &vec_camPosition)
{
    for (const auto &view : sfm_data.GetViews())
    {
        if (sfm_data.IsPoseAndIntrinsicDefined(view.second.get()))
        {
            const geometry::Pose3 pose = sfm_data.GetPoseOrDie(view.second.get());
            vec_camPosition.push_back(pose.center());
        }
    }
}
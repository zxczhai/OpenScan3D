#include <iostream>
#include <vector>
#include <string>
#include <sys/stat.h> // For mkdir() function
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void extract_keyframes(const string& video_file, const string& output_dir)
{
    VideoCapture cap(video_file);
    if (!cap.isOpened())
    {
        cerr << "Error opening video file." << endl;
        return;
    }

    struct stat info;
    if (stat(output_dir.c_str(), &info) != 0)
    {
        int status = mkdir(output_dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (status != 0)
        {
            cerr << "Error creating output directory." << endl;
            return;
        }
    }

    Mat prev_frame, prev_gray;
    cap >> prev_frame;
    if (prev_frame.empty())
    {
        cerr << "Error reading video file." << endl;
        return;
    }
    cvtColor(prev_frame, prev_gray, COLOR_BGR2GRAY);

    int frame_count = 0, keyframe_count = 0;
    const int max_keyframes = 100; // Maximum number of keyframes to extract
    const double min_disp = 4;  // Minimum median displacement for a keyframe
    const int total_frames = cap.get(CAP_PROP_FRAME_COUNT); // Total number of frames in the video
    const int interval = total_frames / max_keyframes; // Interval between keyframes

    while (true)
    {
        Mat frame, gray;
        cap >> frame;
        if (frame.empty())
            break;
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        vector<Point2f> prev_pts, next_pts;
        const int max_corners = 500;
        const double quality_level = 0.01;
        const double min_distance = 10.0;
        const int block_size = 3;
        TermCriteria criteria = TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 30, 0.01);
        goodFeaturesToTrack(prev_gray, prev_pts, max_corners, quality_level, min_distance);
        vector<uchar> status;
        vector<float> error;
        calcOpticalFlowPyrLK(prev_gray, gray, prev_pts, next_pts, status, error, Size(block_size, block_size), 3, criteria);

        vector<double> displacements;
        for (decltype(prev_pts.size()) i = 0; i < prev_pts.size(); i++)
        {
            double dx = next_pts[i].x - prev_pts[i].x;
            double dy = next_pts[i].y - prev_pts[i].y;
            displacements.push_back(sqrt(dx*dx + dy*dy));
        }
        sort(displacements.begin(), displacements.end());
        double median_disp = displacements[displacements.size() / 2];
        if (median_disp >= min_disp && keyframe_count < max_keyframes && frame_count % interval == 0)
        {
            string filename = output_dir + "/keyframe_" + to_string(keyframe_count) + ".jpg";
            imwrite(filename, frame);
            cout << "Saved keyframe #" << keyframe_count << " with displacement " << median_disp << endl;
            keyframe_count++;
        }

        prev_frame = frame.clone();
        prev_gray = gray.clone();
        frame_count++;
    }

    cap.release();
}


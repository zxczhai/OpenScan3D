#include "Common.h"
#include "Scene.h"
#include <opencv2/opencv.hpp>
int main(int argc, char* argv[])
{
	auto OpenViewer = new VIEWER::Scene();
    
	if (!OpenViewer->Init(cv::Size(1920,1080), "Open Viewer", argv[1]))
		return -1;
	OpenViewer->window.SetVisible(true);
	OpenViewer->Loop();
    OpenViewer->Release();

	return 0;
}

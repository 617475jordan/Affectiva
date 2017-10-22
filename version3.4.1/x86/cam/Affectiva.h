#pragma once
#include "AFaceListener.hpp"
#include "PlottingImageListener.hpp"
#include "StatusListener.hpp"
#include <FrameDetector.h>
#include <memory>
#include <chrono>
#include <time.h>
using namespace std;
#if _DEBUG
#pragma  comment(lib,"..\\lib\\debug\\affdex-native.lib")
#else
#pragma  comment(lib,"..\\lib\\release\\affdex-native.lib")
#endif

class affectiva
{
public:
	affectiva();
	~affectiva();
	bool                        initialize(affdex::path DATA_FOLDER, int process_framerate, int buffer_length, int nFaces, int  faceDetectorMode);
	outputData                  runCam(cv::Mat m_matImg);
	void                        close();
private:


	shared_ptr<PlottingImageListener> listenPtr;
    shared_ptr<FrameDetector> frameDetector;
    shared_ptr<StatusListener> videoListenPtr;
    shared_ptr<FaceListener> faceListenPtr; 
	double m_firstTime = -1;
};


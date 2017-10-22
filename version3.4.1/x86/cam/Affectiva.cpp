#include "affectiva.h"


affectiva::affectiva()
{
}


affectiva::~affectiva()
{
}

bool affectiva::initialize(affdex::path DATA_FOLDER, int process_framerate, int buffer_length, int nFaces, int  faceDetectorMode)
{
	/*int                         process_framerate = 30;
	int                         buffer_length = 2;
	unsigned int                nFaces = 20;
	int                         faceDetectorMode = (int)FaceDetectorMode::LARGE_FACES;*/

	if (!boost::filesystem::exists(DATA_FOLDER))
	{
		return false;
	}

	std::ofstream csvFileStream;
	faceListenPtr = shared_ptr<FaceListener>(new AFaceListener());
	listenPtr = shared_ptr<PlottingImageListener>(new PlottingImageListener(csvFileStream, true));  
	videoListenPtr = shared_ptr<StatusListener>(new StatusListener());
	frameDetector = make_shared<FrameDetector>(buffer_length, process_framerate, nFaces, (affdex::FaceDetectorMode) faceDetectorMode);        // Init the FrameDetector Class


	frameDetector->setDetectAllEmotions(true);
	frameDetector->setDetectAllExpressions(true);
	frameDetector->setDetectAllEmojis(true);
	frameDetector->setDetectAllAppearances(true);
	frameDetector->setClassifierPath(DATA_FOLDER);
	frameDetector->setImageListener(listenPtr.get());
	frameDetector->setFaceListener(faceListenPtr.get());
	frameDetector->setProcessStatusListener(videoListenPtr.get());
	frameDetector->start();
	m_firstTime = clock();
	return true;
}

outputData affectiva::runCam(cv::Mat m_matImg)
{

	outputData m_outputData;
	m_outputData.clear();

	if (videoListenPtr->isRunning())//(cv::waitKey(20) != -1)

	{
		// Create a frame
		Frame m_frame(m_matImg.size().width, m_matImg.size().height, m_matImg.data, Frame::COLOR_FORMAT::BGR, (clock() - m_firstTime)/1000);

		frameDetector->process(m_frame);  //Pass the frame to detector

		// For each frame processed
		if (listenPtr->getDataSize() > 0)
		{

			std::pair<Frame, std::map<FaceId, Face> > dataPoint = listenPtr->getData();
			Frame frame = dataPoint.first;
			std::map<FaceId, Face> faces = dataPoint.second;
			m_outputData = listenPtr->draw(faces, m_matImg);

		}
	}
	return m_outputData;
}

void affectiva::close()
{
	frameDetector->stop();
}

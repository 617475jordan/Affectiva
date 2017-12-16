#include "affectiva.h"
#include <time.h>
#include "Header.h"
Mat drawRectangle(Mat m_matInput, vector<coordinate> m_vecCoordinate);

Mat drawRectangle(Mat m_matInput, vector<coordinate> m_vecCoordinate)
{
	for (int i = 0; i < m_vecCoordinate.size(); i++)
	{
		cv::Rect m_rect(m_vecCoordinate[i].x+10, m_vecCoordinate[i].y+10, m_vecCoordinate[i].width-10, m_vecCoordinate[i].height-10);
		cv::rectangle(m_matInput, m_rect, Scalar(255,255,0));
		char currentId[10];
		sprintf(currentId, "Id:%d", i);
		putText(m_matInput, currentId, Point(m_vecCoordinate[i].x + m_vecCoordinate[i].width -20,
			m_vecCoordinate[i].y + m_vecCoordinate[i].height - 20), 1, 1, Scalar(255, 255, 0));
	}
	//imshow("test", m_matInput);
	return m_matInput;
}

int main()
{
	int width = 320;
	int height = 240;
	int fps = 30;
	cv::VideoCapture capture;
	capture.open(0);
	capture.set(CV_CAP_PROP_FRAME_WIDTH, width);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, height);
	if (!capture.isOpened())
	{
		return -1;
	}
	affectiva *m_affectiva = new affectiva();
	Header *m_header = new Header();
	cv::Mat m_matImg;
	capture >> m_matImg;
	if (m_matImg.empty())
	{
		return 0;
	}
	m_header->Init(m_matImg);

	if (!m_affectiva->initialize())
	{
		return -2;
	}
	vector<coordinate> m_vecFirstCoordinate, m_vecSecondCoordinate;
	int m_count = 0;
	while (1)
	{
		int m_time = clock();
		m_vecFirstCoordinate.clear();
		capture >> m_matImg;
		if (m_matImg.empty())
		{
			return -3;
		}
		m_vecFirstCoordinate=m_affectiva->run(m_matImg);
		m_vecSecondCoordinate=m_header->Run(m_matImg);
		cout << m_vecSecondCoordinate.size() << endl;
		m_matImg=drawRectangle(m_matImg, m_vecSecondCoordinate);
		if (m_count == 0 && m_vecFirstCoordinate.size()>0)
		{
			m_header->update(m_vecFirstCoordinate);
			m_count++;
		}
		imshow("result", m_matImg);
		waitKey(1);
		cout << clock() - m_time << "ms" << endl;
		
	}
	m_affectiva = NULL;
	delete[] m_affectiva;
	return 0;
}
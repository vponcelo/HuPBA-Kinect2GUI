#ifndef KINECTGUI_H
#define KINECTGUI_H

#include <QtWidgets/QMainWindow>
#include <QtCore/qtimer.h>
#include <iostream>
#include "ui_kinectgui.h"
#include "ui_kinectguiaux.h"
#include "Kinect2Interface.h"
#include <thread>
#include <sstream>
#include <filesystem>
#include "Poco/MemoryStream.h"
#include "Poco/BinaryWriter.h"
#include "Skeleton.h"

class KinectGUI : public QMainWindow
{
	Q_OBJECT

public:
	KinectGUI(QWidget *parent = 0);
	~KinectGUI();

private:
	
	//GUI
	Ui::KinectGUIClass ui;

	//Other elements of GUI
	UiAux::KinectGuiAuxClass uiAux;
	QSignalMapper* _mapper;

	//Kinect connection
	Kinect2Interface *_kinect2Interface;
	QTimer *_timer;

	//Video Writer
	cv::Mat _skeletonImage;
	cv::Mat	_depthImage;
	cv::Mat	_bodyMaskImage;
	cv::Mat	_colorImage;
	cv::VideoWriter _vwColor;
	cv::VideoWriter _vwDepth;	
	cv::VideoWriter _vwMask;
	cv::VideoWriter _vwSkeleton;
	//int _rgbFrame;
	//int _depthFrame;
	//int _maskFrame;
	//int _skeletonFrame;
	
	//Current date
	time_t _dateTime;

	//Body Frame
	IBodyFrame*				_bodyFrame;
	vector<Skeleton>		_skels;

	//Audio Stream
	float*					_audioBuffer;
	IAudioSource*			_audioStream;
	std::fstream			_fileStream;

	//Recording thread
	std::thread				_rec;
	
	bool _isRecording;
	bool _isCapturing;

	bool _showColor;
	bool _showDepth;
	bool _showMask;
	bool _showSkeleton;
	bool _showAudio;
	bool _recordColor;
	bool _recordDepth;
	bool _recordMask;
	bool _recordSkeleton;
	bool _recordAudio;

	//Set up GUI behavior
	void setupInterfaceBehavior();

	//Set if the different image frames are enabled or not
	void enableImages(bool enabled);
	
	//Thread to record the streams
	std::thread RecordingThread();
	int Run();

	//Set if the different image frames are shown or not
	void showImages(bool show);
	
	void WriteWavHeader(int recordingLength);

	Skeleton getTrackedSkeleton(IBodyFrame* bodyFrame, UINT64 id, bool first);
	std::vector<Skeleton> getSkeletonsFromBodyFrame(IBodyFrame* bodyFrame);
	Skeleton KinectGUI::IBodyToSkeleton(IBody* body);

	template<class Interface> void safeRelease(Interface *& ppT);

private slots:
	//Start gathering from Kinect
	void startKinectCapturing();
	//Stop gathering from Kinect
	void stopKinectCapturing();
	//Start recording Kinect frames
	void startKinectRecording();
	//Stop recording Kinect frames
	void stopKinectRecording();
	//Gather new images
	void updateImages();
	//Update fps
	void updateFPS();
	//Save new images
	void saveFrames(cv::VideoWriter& vw, cv::Mat& image, string s);
	//void saveFrames(cv::Mat& image, int& currentFrame, string s);
	//Connection to checkboxes
	void checkboxChanged(int);

};

#endif // KINECTGUI_H

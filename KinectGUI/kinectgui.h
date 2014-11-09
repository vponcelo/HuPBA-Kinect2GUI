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
	cv::VideoWriter _vwColor;
	cv::VideoWriter _vwDepth;	
	cv::VideoWriter _vwMask;
	//int _rgbFrame;
	//int _depthFrame;
	//int _maskFrame;
	
	//Current date
	time_t _dateTime;

	//Audio Stream
	IAudioSource*			_audioStream;
	IAudioBeamFrameReader*	_pAudioBeamFrameReader;
	WAITABLE_HANDLE			_hFrameArrivedEvent;
	std::fstream _fileStream;

	
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
	
	//Set if the different image frames are shown or not
	void showImages(bool show);	

	//Thread to record the audio stream
	std::thread AudioRecordingThread();
	int Run();
	void WriteWavHeader(int recordingLength);

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

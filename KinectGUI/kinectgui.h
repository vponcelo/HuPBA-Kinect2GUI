#ifndef KINECTGUI_H
#define KINECTGUI_H

#include <QtWidgets/QMainWindow>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <qtimer.h>
#include "ui_kinectgui.h"
#include "ui_kinectguiaux.h"
#include "Kinect2Interface.h"

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
	cv::VideoWriter _vwDepth;
	cv::VideoWriter _vwColor;
	cv::VideoWriter _vwMask;
	
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
	void saveImages(cv::VideoWriter &vw, cv::Mat &image);
	//Connection to checkboxes
	void checkboxChanged(int);

};

#endif // KINECTGUI_H

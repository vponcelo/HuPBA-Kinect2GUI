#ifndef KINECTGUI_H
#define KINECTGUI_H

#include <QtWidgets/QMainWindow>
#include <QtCore/qtimer.h>
#include "ui_kinectgui.h"
#include "ui_kinectguiaux.h"
#include "Kinect2Interface.h"
#include <thread>
#include <stdio.h>
#include <strsafe.h>
#include <sstream>
#include <filesystem>
#include "Poco/MemoryStream.h"
#include "Poco/BinaryWriter.h"
#include "Skeleton.h"
#include "WASAPICapture.h"
#include <Functiondiscoverykeys_devpkey.h>
#include "stdafx.h"

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
	//IBodyFrameReader*		_bfReader;
	//IBodyFrame*			_bodyFrame;
	vector<Skeleton>		_skels;

	//Kinect Sensor
	//IKinectSensor*			_pKinectSensor;

	//Audio Stream
	wchar_t					_waveFileName[MAX_PATH];
	IMMDevice*				_device = NULL;
	HANDLE					_waveFile = INVALID_HANDLE_VALUE;
	CWASAPICapture*			_capturer = NULL;
	//float*				_audioBuffer;
	//std::ofstream			_fileStream;

	struct WAVEHEADER
	{
		DWORD   dwRiff;                     // "RIFF"
		DWORD   dwSize;                     // Size
		DWORD   dwWave;                     // "WAVE"
		DWORD   dwFmt;                      // "fmt "
		DWORD   dwFmtSize;                  // Wave Format Size
	};
	
	//Recording thread
	std::thread						_rec;
	std::thread						_audioThread;
	
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
	
	//Threads to record the streams
	std::thread AudioRecThread();
	int RunAudioRec();
	std::thread RecordingThread();
	int RunRec();

	//Set if the different image frames are shown or not
	void showImages(bool show);
	
	HRESULT GetKinectAudioDevice(IMMDevice **ppDevice);
	HRESULT GetWaveFileName(_Out_writes_(waveFileNameSize) wchar_t *waveFileName, UINT waveFileNameSize);
	HRESULT CaptureAudio(CWASAPICapture *capturer, HANDLE waveFile, const wchar_t *waveFileName);
	HRESULT WriteWaveHeader(HANDLE waveFile, const WAVEFORMATEX *pWaveFormat, DWORD dataSize);

	//void WriteWavHeader(int recordingLength);

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

#include "kinectgui.h"
#include <filesystem>

KinectGUI::KinectGUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	uiAux.setupUi(this, ui);

	setupInterfaceBehavior();
	
	_isCapturing = false;
	_isRecording = false;

	_showColor = false;
	_showDepth = false;
	_showMask = false;
	_showSkeleton = false;
	_showAudio = false;

	_recordColor = false;
	_recordDepth = false;
	_recordMask = false;
	_recordSkeleton = false;
	_recordAudio = false;	
}

KinectGUI::~KinectGUI()
{

}

void KinectGUI::setupInterfaceBehavior()
{
	ui.stopCaptureButton->setEnabled(false);
	ui.startRecordButton->setEnabled(false);
	ui.stopRecordButton->setEnabled(false);

	connect(ui.startCaptureButton, SIGNAL(clicked()), this, SLOT(startKinectCapturing()));
	connect(ui.stopCaptureButton, SIGNAL(clicked()), this, SLOT(stopKinectCapturing()));
	connect(ui.startRecordButton, SIGNAL(clicked()), this, SLOT(startKinectRecording()));
	connect(ui.stopRecordButton, SIGNAL(clicked()), this, SLOT(stopKinectRecording()));

	_mapper = new QSignalMapper(this);
	_mapper->setMapping(ui.colorShowCheck, 0);
	_mapper->setMapping(ui.colorRecordCheck, 1);
	_mapper->setMapping(ui.depthShowCheck, 2);
	_mapper->setMapping(ui.depthRecordCheck, 3);
	_mapper->setMapping(ui.maskShowCheck, 4);
	_mapper->setMapping(ui.maskRecordCheck, 5);
	_mapper->setMapping(ui.skeletonShowCheck, 6);
	_mapper->setMapping(ui.skeletonRecordCheck, 7);
	//_mapper->setMapping(ui.audioShowCheck, 8);
	_mapper->setMapping(ui.audioRecordCheck, 9);

	connect(ui.colorShowCheck, SIGNAL(stateChanged(int)), _mapper, SLOT(map()));
	connect(ui.colorRecordCheck, SIGNAL(stateChanged(int)), _mapper, SLOT(map()));
	connect(ui.depthShowCheck, SIGNAL(stateChanged(int)), _mapper, SLOT(map()));
	connect(ui.depthRecordCheck, SIGNAL(stateChanged(int)), _mapper, SLOT(map()));
	connect(ui.maskShowCheck, SIGNAL(stateChanged(int)), _mapper, SLOT(map()));
	connect(ui.maskRecordCheck, SIGNAL(stateChanged(int)), _mapper, SLOT(map()));
	connect(ui.skeletonShowCheck, SIGNAL(stateChanged(int)), _mapper, SLOT(map()));
	connect(ui.skeletonRecordCheck, SIGNAL(stateChanged(int)), _mapper, SLOT(map()));
	//connect(ui.audioShowCheck, SIGNAL(stateChanged(int)), _mapper, SLOT(map()));
	connect(ui.audioRecordCheck, SIGNAL(stateChanged(int)), _mapper, SLOT(map()));

	connect(_mapper, SIGNAL(mapped(int)), this, SLOT(checkboxChanged(int)));
}

void KinectGUI::checkboxChanged(int cb)
{
	switch (cb)
	{
	case 0:
		if (ui.colorShowCheck->isChecked())
		{
			_showColor = true;
			uiAux.rgbDataImage->show();
		}
		else {
			_showColor = false;
			uiAux.rgbDataImage->close();
		}
		break;
	case 1:
		ui.colorRecordCheck->isChecked() ? _recordColor = true : _recordColor = false;
		break;
	case 2:
		if (ui.depthShowCheck->isChecked())
		{
			_showDepth = true;
			uiAux.depthDataImage->show();
		}
		else
		{
			_showDepth = false;
			uiAux.depthDataImage->close();
		}
		break;
	case 3:
		ui.depthRecordCheck->isChecked() ? _recordDepth = true : _recordDepth = false;
		break;
	case 4:
		if (ui.maskShowCheck->isChecked())
		{
			_showMask = true;
			uiAux.bodyMaskImage->show();
		}
		else
		{
			_showMask = false;
			uiAux.bodyMaskImage->close();
		}
		break;
	case 5:
		ui.maskRecordCheck->isChecked() ? _recordMask = true : _recordMask = false;
		break;
	case 6:
		if (ui.skeletonShowCheck->isChecked())
		{
			_showSkeleton = true;
			uiAux.skeletonImage->show();
		} 
		else
		{
			_showSkeleton = false;
			uiAux.skeletonImage->close();
		}
		break;
	case 7:
		ui.skeletonRecordCheck->isChecked() ? _recordSkeleton = true : _recordSkeleton = false;
		break;
	/*case 8:
		if (ui.audioShowCheck->isChecked())
		{
			_showAudio = true;
			//uiAux.audioImage->show();
		}
		else
		{
			_showAudio = false;
			//uiAux.audioImage->close();
		}
		break;
		*/
	case 9:
		ui.audioRecordCheck->isChecked() ? _recordAudio = true : _recordAudio = false;
		break;
	default:
		break;
	}
}

void KinectGUI::startKinectCapturing()
{
	enableImages(true);
	
	_kinect2Interface = new Kinect2Interface();
	connect(_kinect2Interface, &Kinect2Interface::finished, _kinect2Interface, &QObject::deleteLater);
	_kinect2Interface->start();

	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(updateImages()));
	connect(_timer, SIGNAL(timeout()), this, SLOT(updateFPS()));
	_timer->start(33);
	
	showImages(true);

	ui.startCaptureButton->setEnabled(false);
	ui.stopCaptureButton->setEnabled(true);

	ui.startRecordButton->setEnabled(true);

}

void KinectGUI::enableImages(bool enabled)
{
	uiAux.bodyMaskImage->setEnabled(enabled);
	uiAux.skeletonImage->setEnabled(enabled);
	uiAux.depthDataImage->setEnabled(enabled);
	uiAux.rgbDataImage->setEnabled(enabled);
	//uiAux.audioImage->setEnabled(enabled);
}

void KinectGUI::showImages(bool show)
{
	if (show == true)
	{
		uiAux.bodyMaskImage->show();
		uiAux.skeletonImage->show();
		uiAux.depthDataImage->show();
		uiAux.rgbDataImage->show();
		//uiAux.audioImage->show();
	}
	else
	{
		uiAux.bodyMaskImage->close();
		uiAux.skeletonImage->close();
		uiAux.depthDataImage->close();
		uiAux.rgbDataImage->close();
		//uiAux.audioImage->close();
	}
}

void KinectGUI::updateFPS()
{
	ui.colorFps->setText(QString::number(_kinect2Interface->getRGBFps(), 'f', 2));
	ui.depthFps->setText(QString::number(_kinect2Interface->getDepthFps(), 'f', 2));
	ui.maskFps->setText(QString::number(_kinect2Interface->getBodyMaskFps(), 'f', 2));
	ui.skeletonFps->setText(QString::number(_kinect2Interface->getSkeletonFps(), 'f', 2));
	ui.audioBeam->setText(QString::number(_kinect2Interface->getAudioBeam(), 'f', 2));

	qDebug() << _kinect2Interface->getSkeletonFps();
}

void KinectGUI::updateImages()
{
//	BYTE *buffer = _kinect2Interface->getInputRGBXBuffer();
//	BYTE *bodyMask = _kinect2Interface->getBodyMask();
//	BYTE *depthRGBXBuffer = _kinect2Interface->getDepthRGBXBuffer();

	cv::Mat skeletonImage = _kinect2Interface->getSkeletonImage();
	cv::Mat depthImage = _kinect2Interface->getDepthImage();
	cv::Mat bodyMaskImage = _kinect2Interface->getBodyMaskImage();
	cv::Mat colorImage = _kinect2Interface->getColorImage();

	//Color data
	if (!colorImage.empty())
	{
		cv::Mat colorMat;
		//cv::Size s(_kinect2Interface->getColorWidth() / 3.5, _kinect2Interface->getColorHeight() / 3.5);
		cv::Size s(_kinect2Interface->getColorWidth() * 0.3, _kinect2Interface->getColorHeight() * 0.4);
		cv::resize(colorImage, colorMat, s);

		if (_showColor)
		{
			uiAux.rgbDataImage->showImage(colorMat);
		}

		//if (_isRecording && _recordColor) saveFrames(_vwColor, colorImage, _rgbFrame, "RGB");
		if (_isRecording && _recordColor) saveFrames(_vwColor, colorImage, "RGB");
	}

	//Body mask data
	if (!bodyMaskImage.empty())
	{
		if (_showMask) 
		{
			uiAux.bodyMaskImage->showImage(bodyMaskImage);
		}

		//if (_isRecording && _recordMask) saveFrames(_vwMask, bodyMaskImage, _maskFrame, "mask");
		if (_isRecording && _recordMask) saveFrames(_vwMask, bodyMaskImage, "mask");
	}

	//Depth data
	if (!depthImage.empty())
	{
		if (_showDepth)
		{
			uiAux.depthDataImage->showImage(depthImage);
		}

		//if (_isRecording && _recordDepth) saveFrames(_vwDepth, depthImage, _depthFrame, "depth");
		if (_isRecording && _recordDepth) saveFrames(_vwDepth, depthImage, "depth");
	}
	
	//Skeleton data
	if (!skeletonImage.empty())
	{
		if (_showSkeleton)
		{
			uiAux.skeletonImage->showImage(skeletonImage);
		}

		if (_isRecording && _recordSkeleton) {//TODO: save Joints }
		}
	}
}

void KinectGUI::stopKinectCapturing()
{
	if (_timer)
	{
		_timer->stop();
		delete _timer;
	}
	if (_kinect2Interface)
	{
		_kinect2Interface->finish();
	}

	_isRecording = false;
	_isCapturing = false;

	ui.startCaptureButton->setEnabled(true);
	ui.stopCaptureButton->setEnabled(false);

	ui.stopRecordButton->setEnabled(false);
	ui.startRecordButton->setEnabled(false);

	showImages(false);
}

void KinectGUI::startKinectRecording()
{
	ui.startRecordButton->setEnabled(false);
	ui.stopRecordButton->setEnabled(true);
	
	_isRecording = true;

	//_dateTime = QDateTime::currentDateTime().toString().toStdString(); //CV_FOURCC('M', 'J', 'P', 'G')
	std::time(&_dateTime);	
	std::tr2::sys::path bp(std::to_string(_dateTime));
	std::tr2::sys::create_directories(bp);

	/*
	if (_recordAudio)
	{
		// Use a separate thread for capturing audio because audio stream read operations
		// will block, and we don't want to block main UI thread.
		_fileStream.open(std::to_string(_dateTime) + ".wav");
		int rec_time = (int)300 * 2 * 16000;//300 sec (5 min)
		WriteWavHeader(rec_time);
		std::thread iface = AudioReadingThread();
	}
	*/

	//_rgbFrame = 0;
	//_depthFrame = 0;
	//_maskFrame = 0;

	int fourCC_code = -1;									// Window to select the codec
	//fourCC_code = 1;										// Uncompressed (decreases the frame rate)
	//fourCC_code = CV_FOURCC('M', 'J', 'P', 'G');	// M-JPEG codec (may not be reliable).
	//fourCC_code = CV_FOURCC('P', 'I', 'M', '1');	// MPEG 1 codec. 
	//fourCC_code = CV_FOURCC('D', 'I', 'V', '3');	// MPEG 4.3 codec. 
	//fourCC_code = CV_FOURCC('I', '2', '6', '3');	// H263I codec. 
	//fourCC_code = CV_FOURCC('F', 'L', 'V', '1');	// FLV1 codec. 
	//fourCC_code = CV_FOURCC('M', 'P', '4', '2');	// MP42. 
	//fourCC_code = CV_FOURCC('d', 'i', 'v', 'x');	// DIVX.
	//fourCC_code = CV_FOURCC('I', 'Y', 'U', 'V');	// YUY. 
	//fourCC_code = CV_FOURCC('Y', 'U', 'Y', '2');	// YUY2.
	//fourCC_code = CV_FOURCC('3', 'I', 'V', 'D');	// 3IVD.
	//fourCC_code = CV_FOURCC('3', 'I', 'V', 'X');	// 3IVX.
	//fourCC_code = CV_FOURCC('3', 'I', 'V', '2');	// 3IVX.
	//fourCC_code = CV_FOURCC('d', 'x', '5', '0');	// 3IVX.
	//fourCC_code = CV_FOURCC('H', '2', '6', '4');	// H264.
	//fourCC_code = CV_FOURCC('X', '2', '6', '4');	// X264.
	//fourCC_code = CV_FOURCC('M', 'P', 'G', '4');	// MPG4.
	//fourCC_code = CV_FOURCC('D', 'I', 'B', ' ');	// DIB.
	
	//TODO: open writer or use another kind of writer
	//_vwColor.open(dateTime.toStdString() + "hi_color.avi", CV_FOURCC('D', 'I', 'V', 'X'), 30, cv::Size(_kinect2Interface->getColorWidth() / 3.5, _kinect2Interface->getColorHeight() / 3.5));
	//_vwDepth.open("_depth.mp4", -1, 30, cv::Size(_kinect2Interface->getDepthWidth(), _kinect2Interface->getDepthHeight()));
	//_vwMask.open(dateTime.toStdString() + "_mask.avi", -1, 30, cv::Size(_kinect2Interface->getDepthWidth(), _kinect2Interface->getDepthHeight()));
	if (_recordColor) _vwColor.open(std::to_string(_dateTime) + "/hi_color.avi", fourCC_code, _kinect2Interface->getRGBFps(), cv::Size(_kinect2Interface->getColorWidth(), _kinect2Interface->getColorHeight()), 1);
	if (_recordDepth) _vwDepth.open(std::to_string(_dateTime) + "/depth.avi", fourCC_code, _kinect2Interface->getDepthFps(), cv::Size(_kinect2Interface->getDepthWidth(), _kinect2Interface->getDepthHeight()), 1);
	if (_recordMask) _vwMask.open(std::to_string(_dateTime) + "/mask.avi", fourCC_code, _kinect2Interface->getBodyMaskFps(), cv::Size(_kinect2Interface->getDepthWidth(), _kinect2Interface->getDepthHeight()), 1);
}

void KinectGUI::stopKinectRecording()
{
	ui.startRecordButton->setEnabled(true);
	ui.stopRecordButton->setEnabled(false);

	_isRecording = false;

	_vwColor.release();
	_vwDepth.release();
	_vwMask.release();
}

//void KinectGUI::saveFrames(cv::Mat &image, int currentFrame, string s)
void KinectGUI::saveFrames(cv::VideoWriter &vw, cv::Mat &image, string s)
{	
	// write frames to video
	if (vw.isOpened())
	{
		//cv:imshow(s, image);
		vw.write(image);		
	}
	
	// save frames as images (noisy: original images without compression decrease the frame rate)
	//cv::imwrite(std::to_string(_dateTime) + "/" + s + std::to_string(currentFrame) + ".jpg", image);
	//currentFrame++;
}

std::thread KinectGUI::AudioRecordingThread() 
{
	std::thread t1(&KinectGUI::Run, this);
	return t1;
	
}

int KinectGUI::Run()
{
	HRESULT hr = S_OK;
	IAudioBeamFrameArrivedEventArgs* pAudioBeamFrameArrivedEventArgs = NULL;
	IAudioBeamFrameReference* pAudioBeamFrameReference = NULL;
	IAudioBeamFrameList* pAudioBeamFrameList = NULL;
	IAudioBeamFrame* pAudioBeamFrame = NULL;
	UINT statusMessageFramesToPersistRemaining = 0;
	BOOL clearStatusMessage = FALSE;
	UINT32 subFrameCount = 0;

	_audioStream = _kinect2Interface->getAudioSource();

	hr = _audioStream->OpenReader(&_pAudioBeamFrameReader);

	if (SUCCEEDED(hr))
	{
		// Process new audio frame
		hr = _pAudioBeamFrameReader->GetFrameArrivedEventData(_hFrameArrivedEvent, &pAudioBeamFrameArrivedEventArgs);
		if (SUCCEEDED(hr))
		{
			hr = pAudioBeamFrameArrivedEventArgs->get_FrameReference(&pAudioBeamFrameReference);
		}

		if (SUCCEEDED(hr))
		{
			hr = pAudioBeamFrameReference->AcquireBeamFrames(&pAudioBeamFrameList);
		}

		if (SUCCEEDED(hr))
		{
			// Only one audio beam is currently supported
			hr = pAudioBeamFrameList->OpenAudioBeamFrame(0, &pAudioBeamFrame);
		}

		if (SUCCEEDED(hr))
		{
			hr = pAudioBeamFrame->get_SubFrameCount(&subFrameCount);
		}

		if (SUCCEEDED(hr) && subFrameCount > 0)
		{
			for (UINT32 i = 0; i < subFrameCount; i++)
			{
				// Process all subframes
				IAudioBeamSubFrame* pAudioBeamSubFrame = NULL;

				hr = pAudioBeamFrame->GetSubFrame(i, &pAudioBeamSubFrame);

				if (SUCCEEDED(hr))
				{
					float* pAudioBuffer = NULL;
					UINT cbRead = 0;

					hr = pAudioBeamSubFrame->AccessUnderlyingBuffer(&cbRead, (BYTE **)&pAudioBuffer);

					if (cbRead > 0)
					{
						/*
						DWORD nSampleCount = cbRead / sizeof(float);
						float _fAccumulatedSquareSum;
						int	_nAccumulatedSampleCount;
						static const int cAudioSamplesPerEnergySample = 40;
						// Calculate energy from audio						
						for (UINT i = 0; i < nSampleCount; i++)
						{
							// Compute the sum of squares of audio samples that will get accumulated
							// into a single energy value.
							__pragma(warning(push))
								__pragma(warning(disable:6385)) // Suppress warning about the range of i. The range is correct.
								_fAccumulatedSquareSum += pAudioBuffer[i] * pAudioBuffer[i];
							__pragma(warning(pop))
								++_nAccumulatedSampleCount;

							if (_nAccumulatedSampleCount < cAudioSamplesPerEnergySample)
							{
								continue;
							}
						}*/
						_fileStream << pAudioBuffer;
					}
				}
				safeRelease(pAudioBeamSubFrame);
			}
		}

		safeRelease(pAudioBeamFrame);
		safeRelease(pAudioBeamFrameList);
		safeRelease(pAudioBeamFrameReference);
		safeRelease(pAudioBeamFrameArrivedEventArgs);

		if (FAILED(hr))
		{

			// Persist the status message for some arbitrary amount of time, for example 30 successfully acquired audio frames
			statusMessageFramesToPersistRemaining = 30;
		}
		else if (clearStatusMessage)
		{
			// Clear any previous status messages if needed
			clearStatusMessage = FALSE;
		}
		else if (statusMessageFramesToPersistRemaining > 0)
		{
			// Update frame counter and signal a reset of status message when the counter hits zero
			if (--statusMessageFramesToPersistRemaining == 0)
			{
				clearStatusMessage = TRUE;
			}
		}
	}
	return 0;
}

void KinectGUI::WriteWavHeader(int recodingLength)
{
	int cbFormat = 18; //sizeof(WAVEFORMATEX)
	WAVEFORMATEX format;
	
	format.wFormatTag = 1;
	format.nChannels = 1;
	format.nSamplesPerSec = 16000;
	format.nAvgBytesPerSec = 32000;
	format.nBlockAlign = 2;
	format.wBitsPerSample = 16;
	format.cbSize = 0;

	int bufferSize = 64;
	char *_buffer = (char *)malloc(bufferSize);

	// >> WRITE BLOCK <<   
	//We need to use a memory stream because the BinaryWriter will close the underlying stream when it is closed
	Poco::MemoryOutputStream *memStream = new Poco::MemoryOutputStream(_buffer, bufferSize);
	Poco::BinaryWriter *myBinaryWriter;
	myBinaryWriter = new Poco::BinaryWriter(*memStream);

	//WAVEFORMATEX
	(*myBinaryWriter) << recodingLength + cbFormat + 4;
	(*myBinaryWriter) << cbFormat;
	(*myBinaryWriter) << format.wFormatTag;
	(*myBinaryWriter) << format.nChannels;
	(*myBinaryWriter) << format.nSamplesPerSec;
	(*myBinaryWriter) << format.nAvgBytesPerSec;
	(*myBinaryWriter) << format.nBlockAlign;
	(*myBinaryWriter) << format.wBitsPerSample;
	(*myBinaryWriter) << format.cbSize;

	//RIFF Header
	(*memStream) << "RIFF";
	(*myBinaryWriter) << recodingLength + cbFormat + 4; //File size - 8
	(*memStream) << "WAVE";
	(*memStream) << "fmt ";
	(*myBinaryWriter) << cbFormat;

	//data header
	(*memStream) << "data";
	(*myBinaryWriter) << recodingLength;
	_fileStream << &memStream;

	delete(myBinaryWriter);
	delete(memStream);
}

template<class Interface> void KinectGUI::safeRelease(Interface *& ppT)
{
	if (ppT)
	{
		ppT->Release();
		ppT = NULL;
	}
}
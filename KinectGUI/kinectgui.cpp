#include "kinectgui.h"

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
		cv::Size s(_kinect2Interface->getColorWidth() / 3.5, _kinect2Interface->getColorHeight() / 3.5);
		cv::resize(colorImage, colorMat, s);

		if (_showColor)
		{
			uiAux.rgbDataImage->showImage(colorMat);
		}

		if (_isRecording && _recordColor) saveImages(_vwColor, colorMat);
	}

	//Body mask data
	if (!bodyMaskImage.empty())
	{
		if (_showMask) 
		{
			uiAux.bodyMaskImage->showImage(bodyMaskImage);
		}

		if (_isRecording && _recordMask) saveImages(_vwMask, bodyMaskImage);
	}

	//Depth data
	if (!depthImage.empty())
	{
		if (_showDepth)
		{
			uiAux.depthDataImage->showImage(depthImage);
		}

		if (_isRecording && _recordDepth) saveImages(_vwDepth, depthImage);
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

	QString dateTime = QDateTime::currentDateTime().toString(); //CV_FOURCC('M', 'J', 'P', 'G')
	
	//TODO: open writer or use another kind of writer
	//_vwColor.open("hi_color.avi", CV_FOURCC('D', 'I', 'V', 'X'), 30, cv::Size(_kinect2Interface->getColorWidth() / 3.5, _kinect2Interface->getColorHeight() / 3.5));
	
	//_vwDepth.open("C:\\Users\\Cristina\\Documents\\Visual Studio 2013\\" + dateTime.toStdString() + "_depth.mp4", -1, 30, cv::Size(_kinect2Interface->getDepthWidth(), _kinect2Interface->getDepthHeight()));
	//_vwMask.open("C:\\Users\\Cristina\\Documents\\Visual Studio 2013\\" + dateTime.toStdString() + "_mask.avi", -1, 30, cv::Size(_kinect2Interface->getDepthWidth(), _kinect2Interface->getDepthHeight()));
}

void KinectGUI::stopKinectRecording()
{
	ui.startRecordButton->setEnabled(true);
	ui.stopRecordButton->setEnabled(false);

	_isRecording = false;

	_vwColor.release();
}

void KinectGUI::saveImages(cv::VideoWriter &vw, cv::Mat &image)
{
	if (vw.isOpened())
	{
		vw.write(image);
	}
}
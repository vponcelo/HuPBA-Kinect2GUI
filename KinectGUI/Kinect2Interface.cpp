#include "Kinect2Interface.h"
#include <qdebug.h>

Kinect2Interface::Kinect2Interface(QWidget *parent)
{
	_kinect = new Kinect2();
	_kinect->initializeDefaultSensor();

	_depthBuffer = NULL;
	_depthRGBXBuffer = NULL;
	_outputRGBXbuffer = NULL;
	_inputRGBXbuffer = NULL;
	_bodyMask = NULL;
	_cameraCoordinates = NULL;

	_finished = false;

	_depthHeight = _kinect->getDepthHeight();
	_depthWidth = _kinect->getDepthWidth();
	_colorHeight = _kinect->getColorHeight();
	_colorWidth = _kinect->getColorWidth();
}

Kinect2Interface::~Kinect2Interface()
{
	delete _kinect;
	_kinect = NULL;
}


void Kinect2Interface::run()
{
	while (!_finished)
	{
		_kinect->update();
		_depthBuffer = _kinect->getDepth();
		_depthRGBXBuffer = _kinect->getDepthRGB();
		_outputRGBXbuffer = _kinect->getOutputRGB();
		_inputRGBXbuffer = _kinect->getInputRGB();
		_bodyMask = _kinect->getBodyMask();
		_cameraCoordinates = _kinect->getCameraCoordinates();
		_skeletonJoints = _kinect->getSkeletonJoints();
		_skeletonJointPoints = _kinect->getSkeletonJointPoints();
		//_audioBuffer = _kinect->getAudioBuffer();
		_bodySkeleton = _kinect->getSkeleton();
		
		_skeletonFps = _kinect->getBodyFreq();
		_bodyMaskFps = _kinect->getBodyIndexFreq();
		_RGBFps = _kinect->getRGBFreq();
		_depthFps = _kinect->getDepthFreq();
		_audioBeam = _kinect->getAudioBeam();

		buildSkeletonImage(_skeletonImage);
		buildDepthImage(_depthImage);
		buildColorImage(_colorImage);
		buildBodyMaskImage(_bodyMaskImage);
	}
	emit finished();
}


void Kinect2Interface::buildDepthImage(cv::Mat &depthImage)
{
	if (_depthRGBXBuffer)
	{
		depthImage = cv::Mat(_depthHeight, _depthWidth, CV_8UC4, _depthRGBXBuffer, _depthWidth * sizeof(RGBQUAD));
	}
}

void Kinect2Interface::buildColorImage(cv::Mat &colorImage)
{
	if (_inputRGBXbuffer)
	{
		colorImage = cv::Mat(_colorHeight, _colorWidth, CV_8UC4, _inputRGBXbuffer, _colorWidth * sizeof(RGBQUAD));
	}
}

void Kinect2Interface::buildBodyMaskImage(cv::Mat &bodyMaskImage)
{
	if (_bodyMask)
	{
		cv::Mat ar(_depthHeight, _depthWidth, CV_8UC1, _bodyMask, _depthWidth);
		cv::Mat maskMat(ar.size(), CV_8UC1);
		cv::threshold(ar, maskMat, 100, 255, cv::THRESH_BINARY_INV);

		maskMat.copyTo(bodyMaskImage);
	}
}

void Kinect2Interface::buildSkeletonImage(cv::Mat &skeletonImage)
{
	if (!_skeletonJointPoints.empty() && _skeletonJoints)
	{
		skeletonImage = cv::Mat::zeros(_depthHeight, _depthWidth, CV_8UC3);

		if (!_skeletonJointPoints.empty() && _skeletonJoints)

		{
			// Torso
			drawBone(skeletonImage, _skeletonJoints, _skeletonJointPoints, JointType_Head, JointType_Neck);
			drawBone(skeletonImage, _skeletonJoints, _skeletonJointPoints, JointType_Neck, JointType_SpineShoulder);
			drawBone(skeletonImage, _skeletonJoints, _skeletonJointPoints, JointType_SpineShoulder, JointType_SpineMid);
			drawBone(skeletonImage, _skeletonJoints, _skeletonJointPoints, JointType_SpineMid, JointType_SpineBase);
			drawBone(skeletonImage, _skeletonJoints, _skeletonJointPoints, JointType_SpineShoulder, JointType_ShoulderRight);
			drawBone(skeletonImage, _skeletonJoints, _skeletonJointPoints, JointType_SpineShoulder, JointType_ShoulderLeft);
			drawBone(skeletonImage, _skeletonJoints, _skeletonJointPoints, JointType_SpineBase, JointType_HipRight);
			drawBone(skeletonImage, _skeletonJoints, _skeletonJointPoints, JointType_SpineBase, JointType_HipLeft);

			// Right Arm    
			drawBone(skeletonImage, _skeletonJoints, _skeletonJointPoints, JointType_ShoulderRight, JointType_ElbowRight);
			drawBone(skeletonImage, _skeletonJoints, _skeletonJointPoints, JointType_ElbowRight, JointType_WristRight);
			drawBone(skeletonImage, _skeletonJoints, _skeletonJointPoints, JointType_WristRight, JointType_HandRight);
			drawBone(skeletonImage, _skeletonJoints, _skeletonJointPoints, JointType_HandRight, JointType_HandTipRight);
			drawBone(skeletonImage, _skeletonJoints, _skeletonJointPoints, JointType_WristRight, JointType_ThumbRight);

			// Left Arm
			drawBone(skeletonImage, _skeletonJoints, _skeletonJointPoints, JointType_ShoulderLeft, JointType_ElbowLeft);
			drawBone(skeletonImage, _skeletonJoints, _skeletonJointPoints, JointType_ElbowLeft, JointType_WristLeft);
			drawBone(skeletonImage, _skeletonJoints, _skeletonJointPoints, JointType_WristLeft, JointType_HandLeft);
			drawBone(skeletonImage, _skeletonJoints, _skeletonJointPoints, JointType_HandLeft, JointType_HandTipLeft);
			drawBone(skeletonImage, _skeletonJoints, _skeletonJointPoints, JointType_WristLeft, JointType_ThumbLeft);

			// Draw the joints
			for (int i = 0; i < JointType_Count; ++i)
			{
				qDebug() << _skeletonJoints[i].JointType << " " << (_skeletonJoints[i].JointType != JointType_KneeLeft);
				if (_skeletonJoints[i].JointType != JointType_KneeLeft && _skeletonJoints[i].JointType != JointType_AnkleLeft && _skeletonJoints[i].JointType != JointType_FootLeft &&
					_skeletonJoints[i].JointType != JointType_KneeRight && _skeletonJoints[i].JointType != JointType_AnkleRight && _skeletonJoints[i].JointType != JointType_FootRight)
				{
					//draw ellipse
					cv::Point p1(_skeletonJointPoints[i][0], _skeletonJointPoints[i][1]);

					if (_skeletonJoints[i].TrackingState == TrackingState_Inferred)
					{
						//fill ellipse with brush joint inferred
						cv::ellipse(skeletonImage, p1, cv::Size(1.0, 1.0), 0, 0, 0, cv::Scalar(255, 255, 255), 5, 8);
						//qDebug() << "drawing inferred jointtype" << i;
					}
					else if (_skeletonJoints[i].TrackingState == TrackingState_Tracked)
					{
						//fill ellipse with brush joint tracked
						cv::ellipse(skeletonImage, p1, cv::Size(1.0, 1.0), 0, 0, 0, cv::Scalar(200, 200, 200), 7, 8);
						//qDebug() << "drawing tracked jointtype" << i;
					}

					//	else qDebug() << i << " is not seen";
				}
			}

		}
		//debug
		//cv::imshow("skeleton", skeletonImage);
		//cv::waitKey(30);
	}
}

void Kinect2Interface::drawBone(cv::Mat &image, const Joint* joints, const std::vector<std::vector<int> > &jointPoints, JointType joint0, JointType joint1)
{
	for (int i = 0; i < jointPoints.size(); i++)
	{
	//	qDebug() << joints[joint0].TrackingState << " " << joints[joint1].TrackingState;
	}

	TrackingState joint0State = joints[joint0].TrackingState;
	TrackingState joint1State = joints[joint1].TrackingState;

	cv::Point p1(jointPoints[joint0][0], jointPoints[joint0][1]);
	cv::Point p2(jointPoints[joint1][0], jointPoints[joint1][1]);

	// If we can't find either of these joints, exit
	if ((joint0State == TrackingState_NotTracked) || (joint1State == TrackingState_NotTracked))
	{
	//	qDebug() << "bone skipped not tracked";
		return;
	}

	// Don't draw if both points are inferred
	if ((joint0State == TrackingState_Inferred) && (joint1State == TrackingState_Inferred))
	{
	//	qDebug() << "bone skipped inferred";
		return;
	}

	// We assume all drawn bones are inferred unless BOTH joints are tracked
	if ((joint0State == TrackingState_Tracked) && (joint1State == TrackingState_Tracked))
	{
		//draw line for tracked bone
		cv::line(image, p1, p2, cv::Scalar(250, 250, 250), 5, 8);
	//	qDebug() << "drawing tracked bone" << joint0 << " to " << joint1;
		//m_pRenderTarget->DrawLine(pJointPoints[joint0], pJointPoints[joint1], m_pBrushBoneTracked, c_TrackedBoneThickness);
	}
	else
	{
		//draw line for inferred bone
		cv::line(image, p1, p2, cv::Scalar(250, 250, 250), 4, 8);
	//	qDebug() << "drawing inferred bone" << joint0 << " to " << joint1;
	}
}

Skeleton Kinect2Interface::getSkeleton()
{
	return _bodySkeleton;
}
/*
float* Kinect2Interface::getAudioBuffer()
{
	return _audioBuffer;
}
*/
USHORT* Kinect2Interface::getDepthBuffer()
{
	return _depthBuffer;
}

BYTE* Kinect2Interface::getDepthRGBXBuffer()
{
	return _depthRGBXBuffer;
}

BYTE* Kinect2Interface::getOutputRGBXBuffer()
{
	return _outputRGBXbuffer;
}

BYTE* Kinect2Interface::getInputRGBXBuffer()
{
	return _inputRGBXbuffer;
}

BYTE* Kinect2Interface::getBodyMask()
{
	return _bodyMask;
}

CameraSpacePoint* Kinect2Interface::getCameraCoordinates()
{
	return _cameraCoordinates;
}

Joint* Kinect2Interface::getSkeletonJoints()
{
	return _skeletonJoints;
}

std::vector<std::vector<int> > Kinect2Interface::getSkeletonJointPoints()
{
	return _skeletonJointPoints;
}

cv::Mat& Kinect2Interface::getSkeletonImage()
{
	return _skeletonImage;
}

cv::Mat& Kinect2Interface::getDepthImage()
{
	return _depthImage;
}

cv::Mat& Kinect2Interface::getColorImage()
{
	return _colorImage;
}

cv::Mat& Kinect2Interface::getBodyMaskImage()
{
	return _bodyMaskImage;
}


QVector<QVector3D> Kinect2Interface::getQtCameraCoordinates()
{
	QVector<QVector3D> qvector(_depthWidth * _depthHeight);
	for (int i = 0; i < (_depthWidth * _depthHeight); i++)
	{
		CameraSpacePoint point = _cameraCoordinates[i];
		qvector[i] = QVector3D(point.X, point.Y, point.Z);
	}

	return qvector;
}

int Kinect2Interface::getDepthWidth()
{
	return _depthWidth;
}

int Kinect2Interface::getDepthHeight()
{
	return _depthHeight;
}

int Kinect2Interface::getColorWidth()
{
	return _colorWidth;
}

int Kinect2Interface::getColorHeight()
{
	return _colorHeight;
}

double Kinect2Interface::getSkeletonFps()
{
	return _skeletonFps;
}

double Kinect2Interface::getDepthFps()
{
	return _depthFps;
}

double Kinect2Interface::getRGBFps()
{
	return _RGBFps;
}

double Kinect2Interface::getBodyMaskFps()
{
	return _bodyMaskFps;
}

double Kinect2Interface::getAudioBeam()
{
	return _audioBeam;
}

void Kinect2Interface::finish()
{
	_finished = true;
}

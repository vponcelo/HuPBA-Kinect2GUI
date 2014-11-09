#ifndef KINECT2INTERFACE_H
#define KINECT2INTERFACE_H

#include <qthread.h>
#include <qvector3d.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>

#include "Kinect2.h"
class Kinect2Interface : public QThread
{
	Q_OBJECT

protected:
	void run(); // Q_DECL_OVERRIDE

private:
	Kinect2 *_kinect;
	BYTE* _outputRGBXbuffer;
	BYTE* _inputRGBXbuffer;
	BYTE* _bodyMask;
	BYTE* _depthRGBXBuffer;
	USHORT* _depthBuffer;
	CameraSpacePoint* _cameraCoordinates;
	Joint* _skeletonJoints;
	std::vector<std::vector<int> > _skeletonJointPoints;
	IAudioSource* _audioStream;

	double _skeletonFps;
	double _depthFps;
	double _RGBFps;
	double _bodyMaskFps;
	double _audioBeam;

	bool _finished;

	int _depthWidth;
	int _depthHeight;
	int _colorWidth;
	int _colorHeight;

	cv::Mat _skeletonImage;
	cv::Mat _depthImage;
	cv::Mat _colorImage;
	cv::Mat _bodyMaskImage;

	void drawBone(cv::Mat &image, const Joint* joints, const std::vector<std::vector<int> > &jointPoints, JointType joint0, JointType joint1);

public:
	Kinect2Interface(QWidget *parent = 0);
	~Kinect2Interface();

	USHORT* getDepthBuffer();
	BYTE* getDepthRGBXBuffer();
	BYTE* getOutputRGBXBuffer();
	BYTE* getInputRGBXBuffer();
	BYTE* getBodyMask();
	Joint* getSkeletonJoints();
	std::vector<std::vector<int> > getSkeletonJointPoints();
	CameraSpacePoint* getCameraCoordinates();
	QVector<QVector3D> getQtCameraCoordinates();
	IAudioSource* getAudioSource();

	cv::Mat& getSkeletonImage();
	cv::Mat& getDepthImage();
	cv::Mat& getColorImage();
	cv::Mat& getBodyMaskImage();

	int getDepthWidth();
	int getDepthHeight();
	int getColorWidth();
	int getColorHeight();

	void buildSkeletonImage(cv::Mat &skeletonImage);
	void buildDepthImage(cv::Mat &depthImage);
	void buildColorImage(cv::Mat &colorImage);
	void buildBodyMaskImage(cv::Mat &bodyMaskImage);

	double getSkeletonFps();
	double getDepthFps();
	double getRGBFps();
	double getBodyMaskFps();
	double getAudioBeam();

	void finish();

signals:
	void finished();
};

#endif //KINECT2INTERFACE
#ifndef KINECT2_H
#define KINECT2_H

#include <Kinect.h>
#include <strsafe.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <qdebug.h>
#include <iostream>

class Kinect2
{

	static const int        cDepthWidth = 512;
	static const int        cDepthHeight = 424;
	static const int        cColorWidth = 1920;
	static const int        cColorHeight = 1080;

public:
	/// <summary>
	/// Constructor
	/// </summary>
	Kinect2();

	/// <summary>
	/// Destructor
	/// </summary>
	~Kinect2();

	/// <summary>
	/// Initializes the default Kinect sensor
	/// </summary>
	/// <returns>S_OK on success, otherwise failure code</returns>
	HRESULT initializeDefaultSensor();

	/// <summary>
	/// Main processing function
	/// </summary>
	void update();

	BYTE* getInputRGB();
	BYTE* getOutputRGB();
	USHORT* getDepth();
	BYTE* getDepthRGB();
	BYTE* getBodyMask();
	BYTE* getSkeleton();
	Joint* getSkeletonJoints();
	std::vector<std::vector<int> > getSkeletonJointPoints();
	CameraSpacePoint* getCameraCoordinates();


	int getColorWidth();
	int getColorHeight();
	int getDepthWidth();
	int getDepthHeight();

	double getBodyFreq();
	double getBodyIndexFreq();
	double getDepthFreq();
	double getRGBFreq();
	double getAudioFreq();

	float getMean();
	float getStd();

private:

	INT64						_nStartTime;
	INT64						_nLastCounter;
	INT64						_nLastCounterBody;
	DWORD						_nFramesSinceUpdate;
	DWORD						_nNextStatusTime;
	double						_fFreq;
	double						_fBodyFreq;
	double						_fDepthFreq;
	double						_fRGBFreq;
	double						_fBodyIndexFreq;
	double						_fAudioFreq;
	int							_nHeight;
	int							_nWidth;
	float						_mean;
	float						_std;


	//Output data
	RGBQUAD*					_pOutputRGBX;
	RGBQUAD*					_pColorRGBX;
	RGBQUAD*					_pDepthRGBX;
	USHORT*						_pDepth;
	BYTE*						_pBodyMask;

	//Skeleton data
	Joint*							_joints;
	std::vector<std::vector<int> >	_jointPoints;

	// Current Kinect
	IKinectSensor*				_pKinectSensor;

	//Coordinate mapper		
	ICoordinateMapper*			_pCoordinateMapper;
	ColorSpacePoint*			_pColorCoordinates;
	CameraSpacePoint*			_pCameraCoordinates;

	//Readers
	IMultiSourceFrameReader*	_pMultiSourceFrameReader;
	IAudioBeamFrameReader*		_pAudioBeamFrameReader;




	/// <summary>
	/// Handle new depth and color data
	/// <param name="nTime">timestamp of frame</param>
	/// <param name="pDepthBuffer">pointer to depth frame data</param>
	/// <param name="nDepthWidth">width (in pixels) of input depth image data</param>
	/// <param name="nDepthHeight">height (in pixels) of input depth image data</param>
	/// <param name="pColorBuffer">pointer to color frame data</param>
	/// <param name="nColorWidth">width (in pixels) of input color image data</param>
	/// <param name="nColorHeight">height (in pixels) of input color image data</param>
	/// <param name="pBodyIndexBuffer">pointer to body index frame data</param>
	/// <param name="nBodyIndexWidth">width (in pixels) of input body index data</param>
	/// <param name="nBodyIndexHeight">height (in pixels) of input body index data</param>
	/// </summary>
	void ProcessFrame(INT64 nTime,
		const UINT16* pDepthBuffer, int nDepthWidth, int nDepthHeight, USHORT nMinWidth, USHORT nMaxWidth,
		const RGBQUAD* pColorBuffer, int nColorWidth, int nColorHeight,
		const BYTE* pBodyIndexBuffer, int nBodyIndexWidth, int nBodyIndexHeight);

	/// <summary>
	/// Handle new body data
	/// <param name="nTime">timestamp of frame</param>
	/// <param name="nBodyCount">body data count</param>
	/// <param name="ppBodies">body data in frame</param>
	/// </summary>
	void ProcessBody(INT64 nTime, int nBodyCount, IBody** ppBodies);


	/// <summary>
	/// Capture new audio data.
	/// </summary>
	void ProcessAudio(IAudioBeamSubFrame* pAudioBeamSubFrame);

	template<class Interface> void safeRelease(Interface *& ppT);

	void BodyToScreen(const CameraSpacePoint& bodyPoint, std::vector<int> &points);
};

#endif //KINECT2_H
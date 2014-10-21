#include "Kinect2.h"

#include <qdebug.h>

Kinect2::Kinect2() :
_fFreq(0),
_nLastCounter(0),
_nLastCounterBody (0),
_nFramesSinceUpdate(0),
_nStartTime(0),
_nNextStatusTime(0),
_fBodyFreq(0),
_fDepthFreq(0),
_fRGBFreq(0),
_fBodyIndexFreq(0),
_fAudioFreq(0),
_nHeight(0),
_nWidth(0),
_pKinectSensor(NULL),
_pCoordinateMapper(NULL),
_pMultiSourceFrameReader(NULL),
_pDepth(NULL),
_pDepthRGBX(NULL),
_pOutputRGBX(NULL),
_pColorRGBX(NULL),
_pColorCoordinates(NULL),
_pCameraCoordinates(NULL),
_pBodyMask(NULL)

{
	LARGE_INTEGER qpf = { 0 };
	if (QueryPerformanceFrequency(&qpf))
	{
		_fFreq = double(qpf.QuadPart);
	}

	_nFramesSinceUpdate = 0;

	// create heap storage for composite image pixel data in RGBX format
	_pOutputRGBX = new RGBQUAD[cDepthWidth * cDepthHeight];

	// create heap storage for color pixel data in RGBX format
	_pColorRGBX = new RGBQUAD[cColorWidth * cColorHeight];

	// create heap storage for the coordinate mapping from depth to color
	_pColorCoordinates = new ColorSpacePoint[cDepthWidth * cDepthHeight];

	//create heap storage for the coordinate mapping from depth to camera space
	_pCameraCoordinates = new CameraSpacePoint[cDepthWidth * cDepthHeight];

	//create heap storage for person mask
	_pBodyMask = new BYTE[cDepthWidth * cDepthHeight];

	// create heap storage for depth pixel data in RGBX format
	_pDepthRGBX = new RGBQUAD[cDepthWidth * cDepthHeight];

	// create heap storage for depth pixel data
	_pDepth = new USHORT[cDepthWidth * cDepthHeight];

	_joints = new Joint[JointType_Count];
}


Kinect2::~Kinect2()
{
	if (_pDepth)
	{
		delete[] _pDepth;
		_pDepth = NULL;
	}

	if (_pDepthRGBX)
	{
		delete[] _pDepthRGBX;
		_pDepthRGBX = NULL;
	}

	if (_pOutputRGBX)
	{
		delete[] _pOutputRGBX;
		_pOutputRGBX = NULL;
	}

	if (_pColorRGBX)
	{
		delete[] _pColorRGBX;
		_pColorRGBX = NULL;
	}

	if (_pColorCoordinates)
	{
		delete[] _pColorCoordinates;
		_pColorCoordinates = NULL;
	}

	if (_pCameraCoordinates)
	{
		delete[] _pCameraCoordinates;
		_pCameraCoordinates = NULL;
	}

	if (_pBodyMask)
	{
		delete[] _pBodyMask;
		_pBodyMask = NULL;
	}
	// done with frame reader
	safeRelease(_pMultiSourceFrameReader);

	// done with coordinate mapper
	safeRelease(_pCoordinateMapper);

	// close the Kinect Sensor
	if (_pKinectSensor)
	{
		_pKinectSensor->Close();
	}

	safeRelease(_pKinectSensor);

}

HRESULT Kinect2::initializeDefaultSensor()
{
	HRESULT hr;

	//For more than one kinect - currently not supported
	//IKinectSensorCollection *kinectSensorCollection = 0;
	//hr = GetKinectSensorCollection(&kinectSensorCollection);
	hr = GetDefaultKinectSensor(&_pKinectSensor);
	if (FAILED(hr))
	{
		return hr;
	}
	int count = 0;

	//For more than one kinect - currently not supported
	//IEnumKinectSensor *sensorEnum = 0;
	//kinectSensorCollection->get_Enumerator(&sensorEnum);
	/*while (SUCCEEDED(hr) && sensorEnum != 0 && count < 2)
	{
	hr = sensorEnum->GetNext(&_pKinectSensor);
	if (_pKinectSensor != 0)
	{
	++count;
	}
	}*/

	if (_pKinectSensor)
	{
		//Coordinate mapper
		if (SUCCEEDED(hr))
		{
			hr = _pKinectSensor->get_CoordinateMapper(&_pCoordinateMapper);
		}

		hr = _pKinectSensor->Open();

		//Frames
		if (SUCCEEDED(hr))
		{
			hr = _pKinectSensor->OpenMultiSourceFrameReader(
				FrameSourceTypes::FrameSourceTypes_Depth | FrameSourceTypes::FrameSourceTypes_Color 
				| FrameSourceTypes::FrameSourceTypes_Body | FrameSourceTypes::FrameSourceTypes_BodyIndex,
				&_pMultiSourceFrameReader);
		}

		//Audio
		IAudioSource* pAudioSource = NULL;

		if (SUCCEEDED(hr))
		{
			
			hr = _pKinectSensor->get_AudioSource(&pAudioSource);
		}

		if (SUCCEEDED(hr))
		{
			hr = pAudioSource->OpenReader(&_pAudioBeamFrameReader);
		}
		safeRelease(pAudioSource);


	}

	if (!_pKinectSensor || FAILED(hr))
	{
		return E_FAIL;
	}

	return hr;
}

USHORT* Kinect2::getDepth()
{
	return reinterpret_cast<USHORT*>(_pDepth);
}

BYTE* Kinect2::getDepthRGB()
{
	return reinterpret_cast<BYTE*>(_pDepthRGBX);
}

BYTE* Kinect2::getInputRGB()
{
	return reinterpret_cast<BYTE*>(_pColorRGBX);
}

BYTE* Kinect2::getOutputRGB()
{
	return reinterpret_cast<BYTE*>(_pOutputRGBX);
}

CameraSpacePoint* Kinect2::getCameraCoordinates()
{
	return _pCameraCoordinates;
}

BYTE* Kinect2::getBodyMask()
{
	return _pBodyMask;
}

Joint* Kinect2::getSkeletonJoints()
{
	return _joints;
}

std::vector<std::vector<int> > Kinect2::getSkeletonJointPoints()
{
	return _jointPoints;
}

void Kinect2::update()
{
	if (!_pMultiSourceFrameReader)
	{
		return;
	}

	IMultiSourceFrame* pMultiSourceFrame = NULL;
	IDepthFrame* pDepthFrame = NULL;
	IColorFrame* pColorFrame = NULL;
	IBodyIndexFrame* pBodyIndexFrame = NULL;
	IBodyFrame* pBodyFrame = NULL;
	
	IAudioBeamFrameList* pAudioBeamFrameList = NULL;
	IAudioBeamFrame* pAudioBeamFrame = NULL;

	//Audio
	HRESULT hr = _pAudioBeamFrameReader->AcquireLatestBeamFrames(&pAudioBeamFrameList);

	if (SUCCEEDED(hr))
	{
		hr = pAudioBeamFrameList->OpenAudioBeamFrame(0, &pAudioBeamFrame);
	}

	if (SUCCEEDED(hr))
	{
		UINT32 subFrameCount = 0;

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
					ProcessAudio(pAudioBeamSubFrame);
				}

				safeRelease(pAudioBeamSubFrame);
			}
		}

		safeRelease(pAudioBeamFrame);
		safeRelease(pAudioBeamFrameList);
	}

	//Multisource frame reader
	hr = _pMultiSourceFrameReader->AcquireLatestFrame(&pMultiSourceFrame);

	if (SUCCEEDED(hr))
	{
		IDepthFrameReference* pDepthFrameReference = NULL;

		hr = pMultiSourceFrame->get_DepthFrameReference(&pDepthFrameReference);
		if (SUCCEEDED(hr))
		{
			hr = pDepthFrameReference->AcquireFrame(&pDepthFrame);
		}

		safeRelease(pDepthFrameReference);
	}

	if (SUCCEEDED(hr))
	{
		IColorFrameReference* pColorFrameReference = NULL;

		hr = pMultiSourceFrame->get_ColorFrameReference(&pColorFrameReference);
		if (SUCCEEDED(hr))
		{
			hr = pColorFrameReference->AcquireFrame(&pColorFrame);
		}

		safeRelease(pColorFrameReference);
	}

	if (SUCCEEDED(hr))
	{
		IBodyIndexFrameReference* pBodyIndexFrameReference = NULL;

		hr = pMultiSourceFrame->get_BodyIndexFrameReference(&pBodyIndexFrameReference);
		if (SUCCEEDED(hr))
		{
			hr = pBodyIndexFrameReference->AcquireFrame(&pBodyIndexFrame);
		}

		safeRelease(pBodyIndexFrameReference);
	}

	if (SUCCEEDED(hr))
	{
		IBodyFrameReference* pBodyFrameReference = NULL;

		hr = pMultiSourceFrame->get_BodyFrameReference(&pBodyFrameReference);

		if (SUCCEEDED(hr))
		{
			hr = pBodyFrameReference->AcquireFrame(&pBodyFrame);
		}

		safeRelease(pBodyFrameReference);
	}

	if (SUCCEEDED(hr))
	{
		
	}

	if (SUCCEEDED(hr))
	{
		INT64 nDepthTime = 0;
		IFrameDescription* pDepthFrameDescription = NULL;
		int nDepthWidth = 0;
		int nDepthHeight = 0;
		USHORT nDepthMinReliableDistance = 0;
		USHORT nDepthMaxReliableDistance = 0;
		UINT nDepthBufferSize = 0;
		UINT16 *pDepthBuffer = NULL;

		IFrameDescription* pColorFrameDescription = NULL;
		int nColorWidth = 0;
		int nColorHeight = 0;
		ColorImageFormat imageFormat = ColorImageFormat_None;
		UINT nColorBufferSize = 0;
		RGBQUAD *pColorBuffer = NULL;

		IFrameDescription* pBodyIndexFrameDescription = NULL;
		int nBodyIndexWidth = 0;
		int nBodyIndexHeight = 0;
		UINT nBodyIndexBufferSize = 0;
		BYTE *pBodyIndexBuffer = NULL;

		INT64 nTime = 0;
		// get depth frame data

		hr = pDepthFrame->get_RelativeTime(&nDepthTime);

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrame->get_FrameDescription(&pDepthFrameDescription);
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrameDescription->get_Width(&nDepthWidth);
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrameDescription->get_Height(&nDepthHeight);
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrame->get_DepthMinReliableDistance(&nDepthMinReliableDistance);
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrame->get_DepthMaxReliableDistance(&nDepthMaxReliableDistance);
		}

		if (SUCCEEDED(hr))
		{
			pDepthBuffer = _pDepth;
			hr = pDepthFrame->AccessUnderlyingBuffer(&nDepthBufferSize, reinterpret_cast<UINT16**>(&pDepthBuffer));
		}

		// get color frame data

		if (SUCCEEDED(hr))
		{
			hr = pColorFrame->get_FrameDescription(&pColorFrameDescription);
		}

		if (SUCCEEDED(hr))
		{
			hr = pColorFrameDescription->get_Width(&nColorWidth);
		}

		if (SUCCEEDED(hr))
		{
			hr = pColorFrameDescription->get_Height(&nColorHeight);
		}

		if (SUCCEEDED(hr))
		{
			hr = pColorFrame->get_RawColorImageFormat(&imageFormat);
		}

		if (SUCCEEDED(hr))
		{
			if (imageFormat == ColorImageFormat_Bgra)
			{
				hr = pColorFrame->AccessRawUnderlyingBuffer(&nColorBufferSize, reinterpret_cast<BYTE**>(&pColorBuffer));
			}
			else if (_pColorRGBX)
			{
				pColorBuffer = _pColorRGBX;
				nColorBufferSize = cColorWidth * cColorHeight * sizeof(RGBQUAD);
				if (_pColorRGBX)
					hr = pColorFrame->CopyConvertedFrameDataToArray(nColorBufferSize, reinterpret_cast<BYTE*>(pColorBuffer), ColorImageFormat_Bgra);
			}
			else
			{
				hr = E_FAIL;
			}
		}

		// get body index frame data

		if (SUCCEEDED(hr))
		{
			hr = pBodyIndexFrame->get_FrameDescription(&pBodyIndexFrameDescription);
		}

		if (SUCCEEDED(hr))
		{
			hr = pBodyIndexFrameDescription->get_Width(&nBodyIndexWidth);
		}

		if (SUCCEEDED(hr))
		{
			hr = pBodyIndexFrameDescription->get_Height(&nBodyIndexHeight);
		}

		if (SUCCEEDED(hr))
		{
			hr = pBodyIndexFrame->AccessUnderlyingBuffer(&nBodyIndexBufferSize, &pBodyIndexBuffer);
			hr = pBodyIndexFrame->CopyFrameDataToArray(nDepthWidth * nDepthHeight, _pBodyMask);
		}

		if (SUCCEEDED(hr))
		{

			ProcessFrame(nDepthTime, pDepthBuffer, nDepthWidth, nDepthHeight, nDepthMinReliableDistance, nDepthMaxReliableDistance,
				pColorBuffer, nColorWidth, nColorHeight,
				pBodyIndexBuffer, nBodyIndexWidth, nBodyIndexHeight);
		}

		//get body frame data
		if (SUCCEEDED(hr))
		{

			hr = pBodyFrame->get_RelativeTime(&nTime);

			IBody* ppBodies[BODY_COUNT] = { 0 };

			if (SUCCEEDED(hr))
			{
				hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
			}

			if (SUCCEEDED(hr))
			{
				ProcessBody(nTime, BODY_COUNT, ppBodies);
			}

			for (int i = 0; i < _countof(ppBodies); ++i)
			{
				safeRelease(ppBodies[i]);
			}
		}

		safeRelease(pDepthFrameDescription);
		safeRelease(pColorFrameDescription);
		safeRelease(pBodyIndexFrameDescription);
	}

	safeRelease(pDepthFrame);
	safeRelease(pColorFrame);
	safeRelease(pBodyIndexFrame);
	safeRelease(pBodyFrame);
	safeRelease(pMultiSourceFrame);
}

void Kinect2::ProcessFrame(INT64 nTime,
	const UINT16* pDepthBuffer, int nDepthWidth, int nDepthHeight, USHORT nMinDepth, USHORT nMaxDepth,
	const RGBQUAD* pColorBuffer, int nColorWidth, int nColorHeight,
	const BYTE* pBodyIndexBuffer, int nBodyIndexWidth, int nBodyIndexHeight)
{
	HRESULT hr = _pCoordinateMapper->MapDepthFrameToColorSpace(nDepthWidth * nDepthHeight, (UINT16*)pDepthBuffer, nDepthWidth * nDepthHeight, _pColorCoordinates);

	if (SUCCEEDED(hr))
	{

		RGBQUAD c_white = { 255, 255, 255 };

		hr = _pCoordinateMapper->MapDepthFrameToCameraSpace(nDepthWidth * nDepthHeight, (UINT16*)pDepthBuffer, nDepthWidth * nDepthHeight, _pCameraCoordinates);

		// loop over pixel of the output
		for (int depthIndex = 0; depthIndex < (nDepthWidth * nDepthHeight); ++depthIndex)
		{

			// retrieve the depth to color mapping for the current depth pixel
			ColorSpacePoint colorPoint = _pColorCoordinates[depthIndex];

			const RGBQUAD* pSrc = &c_white;
			//const UCHAR* pBod = 0;

			BYTE player = pBodyIndexBuffer[depthIndex];

			// if we're tracking a player for the current pixel, draw from the color camera
			if (player != 0xff)
			{

				// make sure the depth pixel maps to a valid point in color space
				int colorX = (int)(floor(colorPoint.X + 0.5));
				int colorY = (int)(floor(colorPoint.Y + 0.5));
				if ((colorX >= 0) && (colorX < nColorWidth) && (colorY >= 0) && (colorY < nColorHeight))
				{
					// calculate index into color array
					int colorIndex = colorX + (colorY * nColorWidth);
					// set source for copy to the color pixel
					pSrc = _pColorRGBX + colorIndex;
					//pBod = (UCHAR*)255;
				}
				_pBodyMask[depthIndex] = player;
			}

			_pOutputRGBX[depthIndex] = *pSrc;
		}


		//Process depth data
		RGBQUAD *pRGBX = _pDepthRGBX;
		// end pixel is start + width*height - 1
		const UINT16* pBufferEnd = pDepthBuffer + (nDepthWidth * nDepthHeight);

		while (pDepthBuffer < pBufferEnd)
		{
			USHORT depth = *pDepthBuffer;

			// To convert to a byte, we're discarding the most-significant
			// rather than least-significant bits.
			// We're preserving detail, although the intensity will "wrap."
			// Values outside the reliable depth range are mapped to 0 (black).
			BYTE intensity = depth < nMinDepth || depth > nMaxDepth ? 0 : 255 - (BYTE)(((float)depth / nMaxDepth) * 255.0f);
			pRGBX->rgbRed = intensity;
			pRGBX->rgbGreen = intensity;
			pRGBX->rgbBlue = intensity;

			++pRGBX;
			++pDepthBuffer;
		}
	}

	if (!_nStartTime)
	{
		_nStartTime = nTime;
	}

	double fps = 0.0;

	LARGE_INTEGER qpcNow = { 0 };
	if (_fFreq)
	{
		if (QueryPerformanceCounter(&qpcNow))
		{
			if (_nLastCounter)
			{
				_nFramesSinceUpdate++;
				fps = _fFreq * _nFramesSinceUpdate / double(qpcNow.QuadPart - _nLastCounter);
				_fBodyIndexFreq = fps;
				_fDepthFreq = fps;
				_fRGBFreq = fps;
			}
		}
	}

	_nLastCounter = qpcNow.QuadPart;
	_nFramesSinceUpdate = 0;

}

void Kinect2::ProcessBody(INT64 nTime, int nBodyCount, IBody** ppBodies)
{

	for (int i = 0; i < nBodyCount; ++i)
	{
		IBody* pBody = ppBodies[i];
		if (pBody)
		{
			BOOLEAN bTracked = false;
			HRESULT hr = pBody->get_IsTracked(&bTracked);

			if (SUCCEEDED(hr) && bTracked)
			{
				Joint joints[JointType_Count];
				std::vector<std::vector<int> > jointPoints(JointType_Count);
				
				//HandState leftHandState = HandState_Unknown;
				//HandState rightHandState = HandState_Unknown;

				//pBody->get_HandLeftState(&leftHandState);
				//pBody->get_HandRightState(&rightHandState);

				hr = pBody->GetJoints(_countof(joints), joints);
				if (SUCCEEDED(hr))
				{
					for (int j = 0; j < _countof(joints); ++j)
					{
						std::vector<int> points(2);
						BodyToScreen(joints[j].Position, points);
						jointPoints[j] = points;
						_joints[j] = joints[j];
						//qDebug() << points[0] << " " << points[1];
					}
				}

			
				_jointPoints = jointPoints;

			}
		}
	}

	if (!_nStartTime)
	{
		_nStartTime = nTime;
	}

	double fps = 0.0;

	LARGE_INTEGER qpcNow = { 0 };
	if (_fFreq)
	{
		if (QueryPerformanceCounter(&qpcNow))
		{
			if (_nLastCounter)
			{
				_nFramesSinceUpdate++;
				_fBodyFreq = _fFreq * _nFramesSinceUpdate / double(qpcNow.QuadPart - _nLastCounterBody);
			}
		}
	}
	_nLastCounterBody = qpcNow.QuadPart;
	_nFramesSinceUpdate = 0;
}

void Kinect2::ProcessAudio(IAudioBeamSubFrame* pAudioBeamSubFrame)
{
	HRESULT hr = S_OK;
	float* pAudioBuffer = NULL;
	UINT cbRead = 0;

	hr = pAudioBeamSubFrame->AccessUnderlyingBuffer(&cbRead, (BYTE **)&pAudioBuffer);

	if (cbRead > 0)
	{
		// continue
	}
}

void Kinect2::BodyToScreen(const CameraSpacePoint& bodyPoint, std::vector<int> &points)
{
	// Calculate the body's position on the screen
	DepthSpacePoint depthPoint = { 0 };
	_pCoordinateMapper->MapCameraPointToDepthSpace(bodyPoint, &depthPoint);

	float screenPointX = static_cast<float>(depthPoint.X);
	float screenPointY = static_cast<float>(depthPoint.Y);

	points[0] = screenPointX;
	points[1] = screenPointY;
}

template<class Interface> void Kinect2::safeRelease(Interface *& ppT)
{
	if (ppT)
	{
		ppT->Release();
		ppT = NULL;
	}
}

int Kinect2::getColorWidth()
{
	return cColorWidth;
}

int Kinect2::getColorHeight()
{
	return cColorHeight;
}

int Kinect2::getDepthWidth()
{
	return cDepthWidth;
}
int Kinect2::getDepthHeight()
{
	return cDepthHeight;
}

float Kinect2::getMean()
{
	return _mean;
}

float Kinect2::getStd()
{
	return _std;
}

double Kinect2::getBodyFreq()
{
	return _fBodyFreq;
}

double Kinect2::getBodyIndexFreq()
{
	return _fBodyIndexFreq;
}

double Kinect2::getDepthFreq()
{
	return _fDepthFreq;
}

double Kinect2::getRGBFreq()
{
	return _fRGBFreq;
}

double Kinect2::getAudioFreq()
{
	return _fAudioFreq;
}
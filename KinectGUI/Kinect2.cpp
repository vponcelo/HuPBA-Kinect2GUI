#include "Kinect2.h"

#define _USE_MATH_DEFINES
#include <math.h>


Kinect2::Kinect2() :

_fFreq(0),
_nLastCounter(0),
_nLastCounterBody(0),
_nFramesSinceUpdate(0),
_nStartTime(0),
_nNextStatusTime(0),
_fBodyFreq(0),
_fDepthFreq(0),
_fRGBFreq(0),
_fBodyIndexFreq(0),
_fAudioBeam(0),
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
_pBodyMask(NULL),
_pAudioBeamFrameReader(NULL),
_hFrameArrivedEvent(NULL),
_hTerminateWorkerThread(NULL),
_hWorkerThread(NULL),
_fBeamAngle(0.0f),
_fBeamAngleConfidence(0.0f),
_fAccumulatedSquareSum(0.0f),
_fEnergyError(0.0f),
_nAccumulatedSampleCount(0),
_nEnergyIndex(0),
_nEnergyRefreshIndex(0),
_nNewEnergyAvailable(0),
_nLastEnergyRefreshTime(NULL)

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

	// Audio
	InitializeCriticalSection(&_csLock);

	ZeroMemory(_fEnergyBuffer, sizeof(_fEnergyBuffer));	
}


Kinect2::~Kinect2()
{
	// Audio
	// Signal the worker thread to terminate
	if (NULL != _hTerminateWorkerThread)
	{
		SetEvent(_hTerminateWorkerThread);
	}

	// Wait for the worker thread to terminate
	if (NULL != _hWorkerThread)
	{
		WaitForSingleObject(_hWorkerThread, INFINITE);
		CloseHandle(_hWorkerThread);
	}

	if (NULL != _hTerminateWorkerThread)
	{
		CloseHandle(_hTerminateWorkerThread);
	}

	// Clean up Direct2D renderer
	
	if (NULL != _pAudioBeamFrameReader)
	{
		if (NULL != _hFrameArrivedEvent)
		{
			_pAudioBeamFrameReader->UnsubscribeFrameArrived(_hFrameArrivedEvent);
		}

		SafeRelease(_pAudioBeamFrameReader);
	}

	DeleteCriticalSection(&_csLock);
	
	// Destruct the remainder of modalities
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
	SafeRelease(_pMultiSourceFrameReader);

	// done with coordinate mapper
	SafeRelease(_pCoordinateMapper);

	// close the Kinect Sensor
	if (_pKinectSensor)
	{
		_pKinectSensor->Close();
	}

	SafeRelease(_pKinectSensor);
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

double Kinect2::getAudioBeam()
{
	return _fAudioBeam;
}
/*
IAudioSource* Kinect2::getAudioSource()
{
	return _pAudioSource;
}

float* Kinect2::getAudioBuffer()
{
	return _pAudioBuffer;
}

IBodyFrame* Kinect2::getBodyFrame()
{
	return _pBodyFrame;
}

IBody* Kinect2::getBody()
{
	return _pBody;
}
*/
Skeleton Kinect2::getSkeleton()
{
	return _bodySkeleton;
}

Skeleton Kinect2::IBodyToSkeleton(IBody* body) {
	//Hands
	TrackingConfidence	leftTc, rightTc;
	body->get_HandLeftConfidence(&leftTc);
	body->get_HandRightConfidence(&rightTc);

	HandState leftHs = HandState_Unknown, rightHs = HandState_Unknown;
	body->get_HandLeftState(&leftHs);
	body->get_HandRightState(&rightHs);

	//Tracking
	BOOLEAN	isTracked;
	UINT64 trackingId;
	body->get_TrackingId(&trackingId);
	body->get_IsTracked(&isTracked);

	//Joints
	JointOrientation jointOrientations[JointType_Count];
	Joint joints[JointType_Count];
	body->GetJointOrientations(_countof(jointOrientations), jointOrientations);
	body->GetJoints(_countof(joints), joints);

	return Skeleton(leftHs, rightHs, leftTc, rightTc, isTracked, trackingId, jointOrientations, joints);
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
		_pAudioSource = NULL;
		
		if (SUCCEEDED(hr))
		{
			
			hr = _pKinectSensor->get_AudioSource(&_pAudioSource);
		}

		if (SUCCEEDED(hr))
		{
			hr = _pAudioSource->OpenReader(&_pAudioBeamFrameReader);
		}
		
		if (SUCCEEDED(hr))
		{
			hr = _pAudioBeamFrameReader->SubscribeFrameArrived(&_hFrameArrivedEvent);
		}

		if (SUCCEEDED(hr))
		{
			_hTerminateWorkerThread = CreateEvent(NULL, FALSE, FALSE, NULL);
			if (NULL == _hTerminateWorkerThread)
			{
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
		}

		if (SUCCEEDED(hr))
		{
			_hWorkerThread = CreateThread(NULL, 0, &Kinect2::WorkerThread, this, 0, NULL);
			if (NULL == _hWorkerThread)
			{
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
		}

		SafeRelease(_pAudioSource);		
		
		return hr;		
	}

	if (!_pKinectSensor || FAILED(hr))
	{
		return E_FAIL;
	}

	return hr;
}

DWORD WINAPI Kinect2::WorkerThread(_In_ LPVOID lpParameter)
{
	HRESULT hr = S_OK;
	Kinect2* pThis = static_cast<Kinect2 *>(lpParameter);

	hr = pThis->WorkerThread();

	return SUCCEEDED(hr) ? 0 : 1;
}

/// <summary>
/// Handles frame arrived events
/// </summary>
HRESULT Kinect2::WorkerThread()
{
	HRESULT hr = S_OK;
	BOOL workerThreadRunning = TRUE;
	BOOL clearStatusMessage = FALSE;
	UINT statusMessageFramesToPersistRemaining = 0;
	DWORD timeout = 2000; // In msec. If we don't get a new frame event by this time, display an error message. Any number of seconds will do here.
	UINT32 subFrameCount = 0;
	IAudioBeamFrameArrivedEventArgs* pAudioBeamFrameArrivedEventArgs = NULL;
	IAudioBeamFrameReference* pAudioBeamFrameReference = NULL;
	IAudioBeamFrameList* pAudioBeamFrameList = NULL;
	IAudioBeamFrame* pAudioBeamFrame = NULL;
	HANDLE handles[] = { (HANDLE)_hFrameArrivedEvent, _hTerminateWorkerThread };

	while (workerThreadRunning)
	{
		// Wait for a new audio frame
		DWORD result = WaitForMultipleObjects(_countof(handles), handles, FALSE, timeout);

		if (WAIT_OBJECT_0 == result)
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
						ProcessAudio(pAudioBeamSubFrame);
					}

					SafeRelease(pAudioBeamSubFrame);
				}
			}

			SafeRelease(pAudioBeamFrame);
			SafeRelease(pAudioBeamFrameList);
			SafeRelease(pAudioBeamFrameReference);
			SafeRelease(pAudioBeamFrameArrivedEventArgs);

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
		else if (WAIT_OBJECT_0 + 1 == result)
		{
			// Terminate worker thread
			break;
		}
		else if (WAIT_TIMEOUT == result)
		{
			// This will clear the error message once a new frame is arrived
			clearStatusMessage = TRUE;
		}
		else
		{
			hr = E_FAIL;
			break;
		}
	}

	return hr;
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
	_bodySkeleton = Skeleton();

	//Audio
	ULONGLONG previousRefreshTime = _nLastEnergyRefreshTime;
	ULONGLONG now = GetTickCount64();

	//_nLastEnergyRefreshTime = now;

	// No need to refresh if there is no new energy available to render
	if (_nNewEnergyAvailable <= 0)
	{
		return;
	}

	{
		EnterCriticalSection(&_csLock);

		if (previousRefreshTime != NULL)
		{
			// Calculate how many energy samples we need to advance since the last Update() call in order to
			// have a smooth animation effect.
			float energyToAdvance = _fEnergyError + (((now - previousRefreshTime) * cAudioSamplesPerSecond / (float)1000.0) / cAudioSamplesPerEnergySample);
			int energySamplesToAdvance = std::min((int)_nNewEnergyAvailable, (int)(energyToAdvance));
			_fEnergyError = energyToAdvance - energySamplesToAdvance;
			_nEnergyRefreshIndex = (_nEnergyRefreshIndex + energySamplesToAdvance) % cEnergyBufferLength;
			_nNewEnergyAvailable -= energySamplesToAdvance;
		}

		// Apply latest beam angle and confidence. 
		// SetBeam() expects the angle in degrees, whereas Kinect gives it in radians. Convert from radians to degrees.
		_fAudioBeam = 180.0f * _fBeamAngle / static_cast<float>(M_PI);	

		LeaveCriticalSection(&_csLock);
	}

	//Multisource frame reader
	//HRESULT hr = _pAudioBeamFrameReader->AcquireLatestBeamFrames(&pAudioBeamFrameList);
	HRESULT hr = _pMultiSourceFrameReader->AcquireLatestFrame(&pMultiSourceFrame);

	if (SUCCEEDED(hr))
	{
		IDepthFrameReference* pDepthFrameReference = NULL;

		hr = pMultiSourceFrame->get_DepthFrameReference(&pDepthFrameReference);
		if (SUCCEEDED(hr))
		{
			hr = pDepthFrameReference->AcquireFrame(&pDepthFrame);
		}

		SafeRelease(pDepthFrameReference);
	}

	if (SUCCEEDED(hr))
	{
		IColorFrameReference* pColorFrameReference = NULL;

		hr = pMultiSourceFrame->get_ColorFrameReference(&pColorFrameReference);
		if (SUCCEEDED(hr))
		{
			hr = pColorFrameReference->AcquireFrame(&pColorFrame);
		}

		SafeRelease(pColorFrameReference);
	}

	if (SUCCEEDED(hr))
	{
		IBodyIndexFrameReference* pBodyIndexFrameReference = NULL;

		hr = pMultiSourceFrame->get_BodyIndexFrameReference(&pBodyIndexFrameReference);
		if (SUCCEEDED(hr))
		{
			hr = pBodyIndexFrameReference->AcquireFrame(&pBodyIndexFrame);
		}

		SafeRelease(pBodyIndexFrameReference);
	}

	if (SUCCEEDED(hr))
	{
		IBodyFrameReference* pBodyFrameReference = NULL;

		hr = pMultiSourceFrame->get_BodyFrameReference(&pBodyFrameReference);

		if (SUCCEEDED(hr))
		{
			hr = pBodyFrameReference->AcquireFrame(&pBodyFrame);			
		}

		SafeRelease(pBodyFrameReference);
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
				pColorBuffer, nColorWidth, nColorHeight, pBodyIndexBuffer, nBodyIndexWidth, nBodyIndexHeight);
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
				SafeRelease(ppBodies[i]);
			}
		}

		SafeRelease(pDepthFrameDescription);
		SafeRelease(pColorFrameDescription);
		SafeRelease(pBodyIndexFrameDescription);
	}

	SafeRelease(pDepthFrame);
	SafeRelease(pColorFrame);
	SafeRelease(pBodyIndexFrame);
	SafeRelease(pBodyFrame);
	SafeRelease(pMultiSourceFrame);
	SafeRelease(pAudioBeamFrameList);
	SafeRelease(pAudioBeamFrame);
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
				
				_bodySkeleton = IBodyToSkeleton(pBody);
				//HandState leftHandState = HandState_Unknown;
				//HandState rightHandState = HandState_Unknown;

				//pBody->get_HandLeftState(&leftHandState);
				//pBody->get_HandRightState(&rightHandState);

				hr = pBody->GetJoints(_countof(joints), joints);

				//SafeRelease(pBody);

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
	_pAudioBuffer = NULL;
	UINT cbRead = 0;

	hr = pAudioBeamSubFrame->AccessUnderlyingBuffer(&cbRead, (BYTE **)&_pAudioBuffer);

	if (cbRead > 0)
	{
		DWORD nSampleCount = cbRead / sizeof(float);
		float fBeamAngle = 0.f;
		float fBeamAngleConfidence = 0.0f;

		// Get audio beam angle and confidence
		pAudioBeamSubFrame->get_BeamAngle(&fBeamAngle);
		pAudioBeamSubFrame->get_BeamAngleConfidence(&fBeamAngleConfidence);

		// Calculate energy from audio
		for (UINT i = 0; i < nSampleCount; i++)
		{
			// Compute the sum of squares of audio samples that will get accumulated
			// into a single energy value.
			__pragma(warning(push))
				__pragma(warning(disable:6385)) // Suppress warning about the range of i. The range is correct.
				_fAccumulatedSquareSum += _pAudioBuffer[i] * _pAudioBuffer[i];
			__pragma(warning(pop))
				++_nAccumulatedSampleCount;

			if (_nAccumulatedSampleCount < cAudioSamplesPerEnergySample)
			{
				continue;
			}

			// Each energy value will represent the logarithm of the mean of the
			// sum of squares of a group of audio samples.
			float fMeanSquare = _fAccumulatedSquareSum / cAudioSamplesPerEnergySample;

			if (fMeanSquare > 1.0f)
			{
				// A loud audio source right next to the sensor may result in mean square values
				// greater than 1.0. Cap it at 1.0f for display purposes.
				fMeanSquare = 1.0f;
			}

			float fEnergy = cMinEnergy;
			if (fMeanSquare > 0.f)
			{
				// Convert to dB
				fEnergy = 10.0f*log10(fMeanSquare);
			}

			{
				// Protect shared resources with Update() method on another thread
				EnterCriticalSection(&_csLock);

				_fBeamAngle = fBeamAngle;
				_fBeamAngleConfidence = fBeamAngleConfidence;

				// Renormalize signal above noise floor to [0,1] range for visualization.
				_fEnergyBuffer[_nEnergyIndex] = (cMinEnergy - fEnergy) / cMinEnergy;
				_nNewEnergyAvailable++;
				_nEnergyIndex = (_nEnergyIndex + 1) % cEnergyBufferLength;

				LeaveCriticalSection(&_csLock);
			}

			_fAccumulatedSquareSum = 0.f;
			_nAccumulatedSampleCount = 0;
		}
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


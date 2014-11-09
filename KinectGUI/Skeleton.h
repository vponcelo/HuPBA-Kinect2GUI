#pragma once
#include "Kinect.h"
#include <array>
#include <vector> 
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>

/// Class to manage a Skeleton object, it's mainly a wrapper class
class Skeleton
{
public:
	Skeleton(HandState left, HandState right,
		TrackingConfidence leftTC, TrackingConfidence rightTC,
		BOOLEAN isTracked, UINT64 id,
		JointOrientation jOrient[], Joint joints[]);
	Skeleton();
	~Skeleton();

	//Setters
	void setHandState(HandState left, HandState right);
	void setHandTrackingConfidence(TrackingConfidence left, TrackingConfidence right);
	void setTracking(BOOLEAN isTracked, UINT64 id);
	void setJointsInfo(JointOrientation jOrient[], Joint joints[]);

	//Getters
	std::array<HandState, 2> getHandState();
	std::array<TrackingConfidence, 2> getHandTrackingConfidence();
	BOOLEAN getIsTracked();
	UINT64 getTrackingID();
	JointOrientation* getJointOrientations();
	Joint* getJoints();

	static float euclideanDistance(Joint a, Joint b);
	static float euclideanDistance(std::vector<float> a, std::vector<float> b);
	static float L1Distance(std::vector<float> a, std::vector<float> b);
	static float L1Distance(std::vector<float> a, std::vector<float> b, float alpha);
	static float getAngleBetween(Joint a, Joint mid, Joint c, bool rad);
	static float magnitude(float vec[3]);

	static void skeletonsToCSV(std::vector<Skeleton> gesture, std::string path);
	static std::vector<Skeleton> skeletonsFromCSV(std::string path);

	friend std::ostream& operator<<(std::ostream& os, const Skeleton& sk);
	bool operator==(const Skeleton& b);

	std::vector<float> getGestureRecognitionFeatures(bool rightBody);

private:
	// Info about the body
	TrackingConfidence	leftTc, rightTc;
	HandState			leftHs, rightHs;
	BOOLEAN				isTracked;
	UINT64				trackingId;
	JointOrientation	jointOrientations[JointType_Count];
	Joint				joints[JointType_Count];
	static const int	nJoints = JointType_Count; // Number of Joints
};


#include "Skeleton.h"
#include <assert.h>
#include <stdio.h>
#include <tchar.h>
#define _USE_MATH_DEFINES // To have PI and other constants
#include <math.h>


Skeleton::Skeleton(HandState left, HandState right,
	TrackingConfidence leftTC, TrackingConfidence rightTC,
	BOOLEAN isTracked, UINT64 id,
	JointOrientation jOrient[], Joint joints[]) {
	setHandState(left, right);
	setHandTrackingConfidence(leftTC, rightTC);
	setTracking(isTracked, id);
	setJointsInfo(jOrient, joints);
}

Skeleton::Skeleton() {
	trackingId = 0;
}

Skeleton::~Skeleton()
{

}

void Skeleton::setHandState(HandState left, HandState right) {
	leftHs = left;
	rightHs = right;
}

void Skeleton::setHandTrackingConfidence(TrackingConfidence left, TrackingConfidence right) {
	leftTc = left;
	rightTc = right;
}

void Skeleton::setTracking(BOOLEAN isTracked, UINT64 id) {
	this->isTracked = isTracked;
	trackingId = id;
}

void Skeleton::setJointsInfo(JointOrientation jOrient[], Joint joints[]) {
	//assert(_countof(jointOrientations) == _countof(jOrient) && _countof(this->joints) == _countof(joints));
	for (int i = 0; i < _countof(jointOrientations); ++i) {
		jointOrientations[i] = jOrient[i];
		this->joints[i] = joints[i];
	}
}

/// Returns the left hand state and the right one, in an array
std::array<HandState, 2> Skeleton::getHandState() {
	std::array<HandState, 2> ret = { leftHs, rightHs };
	return ret;
}

// Returns left tracking confidence and right, in an array with this order
std::array<TrackingConfidence, 2> Skeleton::getHandTrackingConfidence(){
	std::array<TrackingConfidence, 2> ret = { leftTc, rightTc };
	return ret;
}

BOOLEAN Skeleton::getIsTracked() {
	return isTracked;
}

UINT64 Skeleton::getTrackingID() {
	return trackingId;
}

JointOrientation* Skeleton::getJointOrientations() {
	return jointOrientations;
}

Joint* Skeleton::getJoints() {
	return joints;
}

bool Skeleton::operator==(const Skeleton& b) {
	bool equal = (leftHs == b.leftHs) && (leftTc == b.leftTc) && (rightHs == b.rightHs) && (rightTc == b.rightTc);
	equal &= (isTracked == b.isTracked) && (trackingId == b.trackingId);

	//Joints
	for (int i = 0; i < nJoints && equal; ++i) {
		equal &= (joints[i].JointType == b.joints[i].JointType);
		equal &= (joints[i].Position.X == b.joints[i].Position.X) && (joints[i].Position.Y == b.joints[i].Position.Y) && (joints[i].Position.Z == b.joints[i].Position.Z);
		equal &= (joints[i].TrackingState == b.joints[i].TrackingState);

		// Orientations
		equal &= (jointOrientations[i].JointType == b.jointOrientations[i].JointType);
		equal &= (jointOrientations[i].Orientation.w == b.jointOrientations[i].Orientation.w) && (jointOrientations[i].Orientation.x == b.jointOrientations[i].Orientation.x)
			&& (jointOrientations[i].Orientation.y == b.jointOrientations[i].Orientation.y) && (jointOrientations[i].Orientation.z == b.jointOrientations[i].Orientation.z);
	}
	return equal;
}

std::ostream& operator<<(std::ostream& os, const Skeleton& sk) {
	os << sk.leftHs << ", " << sk.leftTc << ", " << sk.rightHs << ", " << sk.rightTc << ", ";
	os << (bool)sk.isTracked << ", " << sk.trackingId << ", ";

	os << std::setprecision(std::numeric_limits<float>::max_digits10); // Set max precision
	//Joints
	for (int i = 0; i < sk.nJoints; ++i)
		os << sk.joints[i].JointType << " " << sk.joints[i].Position.X << " " << sk.joints[i].Position.Y << " " << sk.joints[i].Position.Z << " " << sk.joints[i].TrackingState << ", ";

	// Joint orientations
	for (int i = 0; i < sk.nJoints; ++i) {
		os << sk.jointOrientations[i].JointType << " " << sk.jointOrientations[i].Orientation.w << " " << sk.jointOrientations[i].Orientation.x << " ";
		os << sk.jointOrientations[i].Orientation.y << " " << sk.jointOrientations[i].Orientation.z << ((i + 1 == sk.nJoints) ? "" : ", ");
	}
	os << std::endl;
	return os;
}

void Skeleton::skeletonsToCSV(std::vector<Skeleton> gesture, std::string path) {
	if (path.substr(path.size() - 4, 4) != ".csv") path = path + ".csv";
	std::ofstream ofs(path, std::ofstream::out);

	// Write header
	ofs << "sep=, " << std::endl;
	ofs << "LeftHandState, LeftHandTrackingConfidence, RightHandState, RightHandTrackingConfidence, TrackingState, TrackingID, ";
	// TODO change joint number by name?
	int i = 0;
	for (i = 0; i < nJoints; ++i) ofs << "Joint" << i << ", ";
	for (i = 0; i < nJoints; ++i) ofs << "JointOrientation" << i << ((i + 1 == nJoints) ? "" : ", ");
	ofs << std::endl;

	// Add the elements for each gesture
	for (int i = 0; i < gesture.size(); ++i) ofs << gesture[i];
	ofs.close();
}

std::vector<Skeleton> Skeleton::skeletonsFromCSV(std::string path) {
	if (path.substr(path.size() - 4, 4) != ".csv") path = path + ".csv";
	std::ifstream ifs(path, std::ifstream::in);
	std::vector<Skeleton> gesture;
	std::string line;
	std::getline(ifs, line); // Read header line
	while (line.find("sep") != line.npos)
		std::getline(ifs, line);

	std::string delim;
	int aux;
	while (std::getline(ifs, line)) {
		Skeleton frame;
		std::istringstream iss(line);

		// Hands
		iss >> aux >> delim;
		assert(delim == ","); // Just to avoid errors while programming...
		frame.leftHs = static_cast<HandState>(aux);
		iss >> aux >> delim;
		assert(delim == ",");
		frame.leftTc = static_cast<TrackingConfidence>(aux);
		iss >> aux >> delim;
		assert(delim == ",");
		frame.rightHs = static_cast<HandState>(aux);
		iss >> aux >> delim;
		assert(delim == ",");
		frame.rightTc = static_cast<TrackingConfidence>(aux);

		// Tracking
		bool trstate;
		iss >> trstate >> delim;
		assert(delim == ",");
		frame.isTracked = static_cast<BOOLEAN>(trstate);
		iss >> frame.trackingId >> delim;
		assert(delim == ",");

		// Joints
		for (int i = 0; i < frame.nJoints; ++i) {
			iss >> aux >> frame.joints[i].Position.X >> frame.joints[i].Position.Y >> frame.joints[i].Position.Z;
			frame.joints[i].JointType = static_cast<JointType>(aux);
			iss >> aux >> delim;
			assert(delim == ",");
			frame.joints[i].TrackingState = static_cast<TrackingState>(aux);
		}
		for (int i = 0; i < frame.nJoints; ++i) {
			iss >> aux >> frame.jointOrientations[i].Orientation.w >> frame.jointOrientations[i].Orientation.x >> frame.jointOrientations[i].Orientation.y;
			iss >> frame.jointOrientations[i].Orientation.z;
			frame.jointOrientations[i].JointType = static_cast<JointType>(aux);
			if (i + 1 < frame.nJoints) {
				iss >> delim;
				assert(delim == ",");
			}
		}

		// Store this frame
		gesture.push_back(frame);
	}


	ifs.close();
	return gesture;
}

/// Returns two features: the distance between neck and hand and the angle of the arm and the elbow
std::vector<float> Skeleton::getGestureRecognitionFeatures(bool rightBody) {
	float hand_neck_dist, shoulder_elbow_hand_angle;
	if (rightBody) {
		hand_neck_dist = euclideanDistance(joints[JointType_HandRight], joints[JointType_Neck]);
		shoulder_elbow_hand_angle = getAngleBetween(joints[JointType_ShoulderRight], joints[JointType_ElbowRight], joints[JointType_HandRight], true);
	}
	else {
		hand_neck_dist = euclideanDistance(joints[JointType_HandLeft], joints[JointType_Neck]);
		shoulder_elbow_hand_angle = getAngleBetween(joints[JointType_ShoulderLeft], joints[JointType_ElbowLeft], joints[JointType_HandLeft], true);
	}
	std::vector<float> feature = { hand_neck_dist, shoulder_elbow_hand_angle };
	return feature;
}

float Skeleton::euclideanDistance(std::vector<float> a, std::vector<float> b) {
	assert(a.size() == b.size());
	float _sum = 0;
	for (int i = 0; i < a.size(); ++i) {
		float dif = a[i] - b[i];
		_sum += dif*dif;
	}
	return sqrt(_sum);
}

float L1Distance(std::vector<float> a, std::vector<float> b) {
	assert(a.size() == b.size());
	float _sum = 0;
	for (int i = 0; i < a.size(); ++i) _sum += abs(a[i] - b[i]);
	return _sum;
}

float Skeleton::L1Distance(std::vector<float> a, std::vector<float> b, float alpha) {
	return abs(a[0] - b[0])*alpha + abs(a[1] - b[1]);
}

float Skeleton::euclideanDistance(Joint a, Joint b) {
	float sq = pow(a.Position.X - b.Position.X, 2);
	sq += pow(a.Position.Y - b.Position.Y, 2);
	sq += pow(a.Position.Z - b.Position.Z, 2);
	return sqrt(sq);
}

/// Returns de angle between a->mid and mid->c. For example shoulder->elbow->hand
float Skeleton::getAngleBetween(Joint a, Joint mid, Joint c, bool rad) {
	// Vector mid->a
	float vec1[3] = { a.Position.X - mid.Position.X,
		a.Position.Y - mid.Position.Y,
		a.Position.Z - mid.Position.Z };
	// Vector mid->c
	float vec2[3] = { c.Position.X - mid.Position.X,
		c.Position.Y - mid.Position.Y,
		c.Position.Z - mid.Position.Z };

	float dotProd = vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
	float cosAngle = dotProd / (magnitude(vec1) * magnitude(vec2));
	if (rad)
		return acos(cosAngle);
	return acos(cosAngle) * 180.0 / M_PI;
}

float Skeleton::magnitude(float vec[3]) {
	return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}
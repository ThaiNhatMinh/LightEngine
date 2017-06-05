#pragma once
#include "..\pch.h"
// This class doesn't complete. I will finish it on the future when I learn more on Math

class Vector3;
class Quaternion;
class Matrix4;

class EulerAngle
{
	friend class Matrix4;
public:
	// In Degree
	float pitch;	// X axis 
	float yaw;		// Y axis bank
	float roll;		// Z axis heading
	EulerAngle();
	EulerAngle(float pitch, float yaw, float roll);
	EulerAngle(const Vector3& V) {};
	void Identity() {};
	void Set(float pitch, float yaw, float roll) {};
	void ToMatrixXYZ(Matrix4& m);
	Quaternion ToQuaternion();
	~EulerAngle();
};


#pragma once
#include "..\pch.h"


class Math
{
public:
	static const float EPSILON;
	static const float MAX_REAL;
	static const float PI;
	static const float TWO_PI;
	static const float HALF_PI;
	static const float DEG_TO_RAD;
	static const float RAD_TO_DEG;

public:
	// Basic math function
	static float ACos( float);
	static float ASin( float);
	static float ATan( float);
	static float ATan( float, float);
	static float Cos( float);
	static float Sin( float);
	static float Exp( float);
	static float FAbs( float);
	static float Floor( float);
	static float InvSqrt( float); // Simple return 1/sqrt()
	static float Log( float);
	static float Pow( float, float);
	static float Sqr( float);
	static float Sqrt( float);
	static float Tan( float);
	static void SinCos(float& sin, float& cos, float theta);
	static float ToRadian(float Angle);
	static float ToDegree(float Radian);
	static float DistanceSquare(const Vector3 v1, const Vector3& v2);
	// Vector Operators
	static Vector3 Normalize(const Vector3& v);
	static Vector3 Cross(const Vector3& a, const Vector3& b);
	static float Dot(const Vector3& v,const Vector3& u);
	// Matrix 3D Operators
	static Matrix4 LookAt(Vector3 cameraPosition, Vector3 cameraTarget, Vector3 cameraUpVector);
	static Matrix4 Perspective(float FOV, float aspectRaito, float Znear, float Zfar);
	static Matrix4 Ortho(float width, float height);
	// Quaternion Operators
	static Quaternion Pow(const Quaternion& Q, float exponent);
	static Quaternion slerp(const Quaternion& from, const Quaternion& to, float t); 
	static Vector3 lerp(const Vector3 & v0, const Vector3& v1, float t);
	static void Translate(const Matrix4& M,const Vector3& t);
	static void Translate(const Matrix4& M, float x, float y, float z);
	static void GetBasicVector(const Matrix4& M, Vector3& xaxis, Vector3& yaxis, Vector3& zaxis);

};


#include <iostream>
#include "Quaternion.h"
#include "CMath.h"
#include "Vector.h"
// This class finish basic operator on Matrix. 3D operator will be update soon..

// 0	1	2	3			x-axis	y-axis	z-axis	0
// 4	5	6	7	=>>		x-axis	y-axis	z-axis	0
// 8	9	10	11			x-axis	y-axis	z-axis	0
// 12	13	14	15			x-pos	y-pos	z-pos	1
// collum first

#pragma once
class Vector3;
class Matrix4
{
	friend class Math;
public:
	Matrix4();
	Matrix4(const Matrix4& M);
	Matrix4(bool Zero);
	Matrix4(const Vector3& v1,const Vector3& v2,const Vector3& v3);
	Matrix4(const Vector3& v1,const Vector3& v2,const Vector3& v3,const Vector3& v4);
	Matrix4(const float* Data);
	Matrix4( float m11, float m12, float m13, float m14,
			 float m21, float m22, float m23, float m24,
			 float m31, float m32, float m33, float m34,
			 float m41, float m42, float m43, float m44);
	~Matrix4();

	void Identity();
	void ZeroTranslate();
	// member acces
	
	// Collum Row
	float operator()(int Col, int Row ) const;
	float& operator()(int Col, int Row);
	Vector3 operator()(int axis) const;
	void operator()(int axis, Vector3 v);
	float operator[](int id) const;
	float& operator[](int id);
	// assignment
	Matrix4& operator=(const Matrix4& M);
	// Comparsion Operations
	bool operator==(const Matrix4& M)const;
	bool operator!=(const Matrix4& M)const;
	//bool operator>(const Matrix4& M)const;
	//bool operator>=(const Matrix4& M)const;
	//bool operator<(const Matrix4& M)const;
	//bool operator<=(const Matrix4& M)const;
	// Arithmetic Operations
	Matrix4 operator+(const Matrix4& M) const;
	Matrix4 operator-(const Matrix4& M) const;
	Matrix4 operator*(const Matrix4& M) const;
	Matrix4 operator*(float Scalar) const;
	Vector3	operator*(const Vector3& v)const;
	Vector4	operator*(const Vector4& v)const;
	Matrix4 operator/(float Scalar) const;
	Matrix4 operator-() const;
	const float * ToFloatPtr() const;
	float* ToFloatPtr();
	quat ToQuat()const;
	// Arithmetic Update
	Matrix4& operator+=(const Matrix4& M);
	Matrix4& operator-=(const Matrix4& M);
	Matrix4& operator*=(float Scalar);
	Matrix4& operator/=(float Scalar);

	// Vector3 operator*(const Vector3& v) const;
	void Translate(const Vector3& v);
	Vector3 GetTranslate2()const;
	void Translate2(const Vector3& v);
	Vector3 GetTranslate()const;
	void Translate(float x, float y, float z);
	Matrix4 Transpose() const;
	Matrix4 Inverse() const;
	//Matrix4 Adjoint() const;
	//float Determinant() const;

	// For debug
	friend std::ostream& operator<<(std::ostream& out, const Matrix4& source);
	friend std::ostream& operator<<(const Matrix4& source,std::ostream& out);

protected:
    // member variables
    float mV[16];

};

typedef Matrix4 mat4;
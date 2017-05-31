#pragma once
#include "..\pch.h"
class Matrix4;

class Quaternion
{
public: 
	//Basic value
	float w,x, y, z;

public:
	Quaternion();
	Quaternion(float w, float x, float y, float z);
	~Quaternion();

	void Identity();
	Quaternion& Normalize();
	float GetRotateAngle();
	Vector3 GetRotateAxis();
	float Dot(const Quaternion& Q);
	Quaternion Conjugate();
	void set(float w, float x, float y, float z){this->x =x;this->y=y;this->z=z;this->w=w;};
	Matrix4 ToMatrix();

	void setRotateXAxis(float theta);
	void setRotateYAxis(float theta);
	void setRotateZAxis(float theta);
	void setRotateAxis(const Vector3& Axis, float theta);

	Quaternion		operator*(const Quaternion& Q) const;
	Vector3			operator*(const Vector3& v) const;
	Quaternion&		operator*=(const Quaternion& Q);
	float			operator[]( int index ) const{	assert( ( index >= 0 ) && ( index < 4 ) );	return ( &w )[ index ];	};
	float &			operator[]( int index ){		assert( ( index >= 0 ) && ( index < 4 ) );	return ( &w )[ index ];	};

	// Debug Operations
	friend std::ostream& operator<<(std::ostream& out, const Quaternion& source);
	friend std::ostream& operator<<(const Quaternion& source,std::ostream& out);


};

Vector3 operator*(const Vector3& v, const Quaternion& q);
typedef Quaternion quat;

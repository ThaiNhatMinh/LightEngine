#include "..\pch.h"


Quaternion::Quaternion():x(0),y(0),z(0),w(1)
{
}

Quaternion::Quaternion(float w, float x, float y, float z)
{
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

Quaternion::~Quaternion()
{
}

void Quaternion::Identity()
{
	w = 1.0f;
	x = y = z = 0.0f;
}

// w = cos(theta/2)

float Quaternion::GetRotateAngle()
{
	float thetaOver2 = Math::ACos(w);
	return thetaOver2*2.0f;
}

Vector3 Quaternion::GetRotateAxis()
{
	float sinThetaSqr = 1.0f - w*w;
	if(sinThetaSqr <=0.0f) return Vector3(1.0f, 0.0f, 0.0f);

	float oneOverSin = Math::InvSqrt(sinThetaSqr);

	return Vector3( x*oneOverSin, y*oneOverSin, z*oneOverSin);
}

Quaternion& Quaternion::Normalize()
{
	float magSqr = w*w + x*x + y*y + z*z;
	if(magSqr>0.0f)
	{
		magSqr = 1.0f/magSqr;
		w*=magSqr;
		x*=magSqr;
		z*=magSqr;
		y*=magSqr;
	}
	else 
	{
		assert(false);
		Identity();
	}

	return *this;
}

void Quaternion::setRotateXAxis(float theta)
{
	float thetaOver2 = theta*0.5f;
	w = Math::Cos(Math::ToRadian(thetaOver2));
	x = Math::Sin(Math::ToRadian(thetaOver2));
	y = 0.0f;
	z = 0.0f;
}

void Quaternion::setRotateYAxis(float theta)
{
	float thetaOver2 = theta*0.5f;
	w = Math::Cos(Math::ToRadian(thetaOver2));
	x = 0.0f;
	y = Math::Sin(Math::ToRadian(thetaOver2));
	z = 0.0f;
}

// input: theta in 360*
void Quaternion::setRotateZAxis(float theta)
{
	float thetaOver2 = theta*0.5f;
	w = Math::Cos(Math::ToRadian(thetaOver2));
	x = 0.0f;
	y = 0.0f;
	z = Math::Sin(Math::ToRadian(thetaOver2));
}

void Quaternion::setRotateAxis(const Vector3& Axis, float theta)
{
	// The Axis of rotation must be normalize

	assert(Math::FAbs(Axis.Length() - 1.0f) <0.01f);

	float thetaOver2 = theta*0.5f;
	float sin  = Math::Sin(Math::ToRadian(thetaOver2));
	w = Math::Cos(Math::ToRadian(thetaOver2));
	x = Axis.x * sin;
	y = Axis.y * sin;
	z = Axis.z * sin;
}

Quaternion Quaternion::operator*(const Quaternion& Q) const
{
	return Quaternion(	w*Q.w - x*Q.x - y*Q.y - z*Q.z,
						w*Q.x + x*Q.w + z*Q.y - y*Q.z,
						w*Q.y + y*Q.w + x*Q.z - z*Q.x,
						w*Q.z + z*Q.w + y*Q.x - x*Q.y);
}

Vector3 Quaternion::operator*(const Vector3& v)const
{
	float vMul = 2.0f*(x*v.x + y*v.y + z*v.z);
	float crossMul = 2.0f*vMul;
	float pMul = crossMul - 1.0f;

	return Vector3(		pMul*v.x + vMul*x + crossMul*(y*v.z - z*v.y),
						pMul*v.y + vMul*y + crossMul*(z*v.x - x*v.z),
						pMul*v.z + vMul*z + crossMul*(x*v.y - y*v.x));

}

Vector3 operator*(const Vector3& v, const Quaternion& q)
{
	return q*v;
}

Quaternion& Quaternion::operator*=(const Quaternion& Q)
{
	*this = *this * Q;
	return *this;
}

float Quaternion::Dot(const Quaternion& Q)
{
	return w*Q.w + x*Q.x + y*Q.y + z*Q.z;
}

Quaternion Quaternion::Conjugate()
{
	return Quaternion(w, -x,-y,-z);
}

Matrix4 Quaternion::ToMatrix()
{
	float ww = w + w;
	float yy = y + y;
	float xx = x + x;
	float zz = z + z;

	return Matrix4( 1.0f - yy*y - zz*z, xx*y - ww*z, xx*z + ww*y, 0,
					xx*y + ww*z, 1.0f - xx*x - zz*z, yy*z - ww*x, 0,
					xx*z - ww*y, yy*z + ww*x, 1.0f - xx*x - yy*y, 0,
					0,		0,		0,		1);

}


std::ostream&  operator<<(std::ostream& os,const Quaternion& v )
{
	os<< '<' << v.x << ',' << v.y << ',' << v.z << ',' << v.w << '>';
	return os;
}

std::ostream&  operator<<(const Quaternion& v ,std::ostream& os)
{
	return os;
}

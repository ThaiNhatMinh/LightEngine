#include "..\pch.h"

const float Math::PI			= 3.14159265358979323846f;
const float Math::TWO_PI		= 2.0f* Math::PI;
const float Math::HALF_PI		= 0.5f* Math::PI;
const float Math::EPSILON		= 1.0e-6f;
const float Math::MAX_REAL		= 1e30f;
const float Math::DEG_TO_RAD	= Math::PI / 180.0f;
const float Math::RAD_TO_DEG	= 180.0f / Math::PI;
const mat4 Math::g_Indentity	= Matrix4(vec3(1.0,0,0),vec3(0,1.0,0),vec3(0,0,1.0));

float Math::ACos(float fValue)
{
	if(fValue<=-1.0f) return PI;
	if(fValue>=1.0f) return 0.0f;
	return acosf(fValue);
}

float Math::ASin(float fValue)
{
	if(fValue<=-1.0f) return -HALF_PI;
	if(fValue>=1.0f) return HALF_PI;
	return asinf(fValue);
}

 float Math::ATan(float fValue)
{
	return atanf(fValue);
}

 float Math::ATan(float y, float x)
{
	return atan2f(y,x);
}

 float Math::Cos(float fValue)
{
	return cosf(fValue);
}

 float Math::Exp(float fValue)
{
	return expf(fValue);
}

 float Math::FAbs(float fValue)
{
	int tmp = *reinterpret_cast<int *>( &fValue);
	tmp &= 0x7FFFFFFF;
	return *reinterpret_cast<float *>( &tmp );
}

 float Math::Floor(float fValue)
{
	return floorf(fValue);
}

 float Math::InvSqrt(float fValue)
{
	return 1.0f/sqrtf(fValue);
}

 float Math::Log(float fValue)
{
	return logf(fValue);
}

 float Math::Pow(float fValue,float fExponent)
{
	return powf(fValue,fExponent);
}

 float Math::Sin(float fValue)
{
	return sinf(fValue);
}

 float Math::Sqr(float fValue)
{
	return fValue*fValue;
}

 float Math::Sqrt(float fValue)
{
	return sqrtf(fValue);
}

 float Math::Tan(float fValue)
{
	return tanf(fValue);
}

 void Math::SinCos(float& sin, float& cos, float theta)
{
	sin = Sin(theta);
	cos = Cos(theta);
}

 float Math::ToDegree(float Radian)
{
	return Radian * RAD_TO_DEG;
}

 float Math::DistanceSquare(const Vector3 v1, const Vector3 & v2)
 {
	 float r = (v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y);
	 return r;
 }

 float Math::ToRadian(float Angle)
{
	return Angle * DEG_TO_RAD;
}

 Vector3 Math::Normalize(const Vector3& v)
 {
	 float oneover =  1.0f/Sqrt(v.x*v.x + v.y*v.y+v.z*v.z);

	 return Vector3(v.x*oneover, v.y*oneover, v.z*oneover);
 }

 float Math::Dot(const Vector3& v,const Vector3& u)
 {
	 return v.x*u.x + v.y*u.y + v.z*u.z;
 }

 Vector3 Math::Cross(const Vector3& a, const Vector3& b)
 {
	 return Vector3(	a.y*b.z - a.z*b.y,
						a.z*b.x - a.x*b.z,
						a.x*b.y - a.y*b.x);

 }
 
 Matrix4 Math::LookAt(Vector3 cameraPosition, Vector3 cameraTarget, Vector3 Up)
{
	Vector3 zaxis = Normalize(cameraPosition - cameraTarget);
	Vector3 xaxis = Normalize(Cross(Up,zaxis));
	Vector3 yaxis = Cross(zaxis,xaxis);
	Vector3 pos(-Dot(xaxis,cameraPosition),-Dot(yaxis,cameraPosition),-Dot(zaxis,cameraPosition));


	return	Matrix4(	xaxis.x,	yaxis.x,	zaxis.x,		0,
						xaxis.y,	yaxis.y,	zaxis.y,		0,
						xaxis.z,	yaxis.z,	zaxis.z,		0,
						pos.x,		pos.y,		pos.z,			1);
	/*
	return	Matrix4(	xaxis.x,	xaxis.y,	xaxis.z,		pos.x,
						yaxis.x,	yaxis.y,	yaxis.z,		pos.y,
						zaxis.x,	zaxis.y,	zaxis.z,		pos.z,
						0,		0,		0,			1);*/

}

 Matrix4 Math::Perspective(float FOV, float a, float znear, float zfar)
 {
	 float d = 1.0f/tanf(FOV/180.0f*PI*0.5f);

	 float xymax = znear * tanf(FOV / 180.0f*PI*0.5f);
	 float ymin = -xymax;
	 float xmin = -xymax;

	 float width = xymax - xmin;
	 float height = xymax - ymin;

	 float depth = zfar - znear;
	 float q = -(zfar + znear) / depth;
	 float qn = -2 * (zfar * znear) / depth;

	 float w = 2 * znear / width;
	 w = w / a;
	 float h = 2 * znear / height;

	 return Matrix4(w, 0, 0, 0,
					0, h, 0, 0,
					0, 0, q, -1,
					0, 0, qn, 0.0f);

	 /*return Matrix4(	d/a,	0,	0, 0,
						0,		d,	0, 0,
						0,		0,	(Zfar+Znear)/(Znear-Zfar),2.0f*Znear*Zfar/(Znear-Zfar),
						0,		0,	-1.0f,	0.0f);
	/* return Matrix4(	d/a,	0,	0, 0,
						0,		d,	0, 0,
						0,		0,	Zfar/(Zfar-Znear), -Znear*Zfar/(Zfar - Znear),
						0,		0,	1.0f,	1.0);
	 */
 }

 Matrix4 Math::Ortho(float width, float height)
 {
	 float right = width;
	 float left = 0;
	 float top = height;
	 float bottom = 0;

	 return Matrix4(2.0f/(right-left),		0,					0,						0,
					0,						2.0f/(top-bottom),	0,						0,
					0,						0,					-1.0f,					0,
		 -(right + left) / (right - left), -(top + bottom) / (top - bottom),					0,						1			);
 }

void Math::GetBasicVector(const Matrix4& M, Vector3& xaxis, Vector3& yaxis, Vector3& zaxis)
{
	xaxis.set( M.mV[0], M.mV[4], M.mV[8]);
	yaxis.set( M.mV[1], M.mV[5], M.mV[9]);
	zaxis.set( M.mV[2], M.mV[6], M.mV[10]);
}


 Quaternion Math::Pow(const Quaternion& Q, float exponent)
{
	if(FAbs(Q.w)>0.9999f) return Q;
	float alpha = ACos(Q.w);
	float newAlpla = alpha*exponent;
	Quaternion result;
	result.w = Cos(newAlpla);
	float mul = Sin(newAlpla)/Sin(alpha);
	result.x = Q.x*mul;
	result.y = Q.y*mul;
	result.z = Q.z*mul;

	return result;
}

Quaternion Math::slerp(const Quaternion& From, const Quaternion& To, float t)
{
	if(t<=0.0f) return From;
	if(t>=1.0f) return To;

	float cosOmega =  From.w*To.w + From.x*To.x + From.y*To.y + From.z*To.z;

	float ToW = To.w;
	float ToX = To.x;
	float ToY = To.y;
	float ToZ = To.z;
	if(cosOmega<0.0f)
	{
		ToW = -ToW;
		ToX = -ToX;
		ToY = -ToY;
		ToZ = -ToZ;
		cosOmega = -cosOmega;
	}

	// Must be unit quaternion

	assert(cosOmega<1.1f);

	// Compute interpolation fraction, checking for quaternions
	// almost exactly the same
	float k0,k1;
	if(cosOmega>0.9999f)
	{
		// Very close - just use linear interpolation,
		// which will protect againt a divide by zero
		k0 = 1.0f - t;
		k1 = t;
	}
	else 
	{
		// Compute the sin of the angle using the
		// trig identity sin^2(omega) + cos^2(omega) = 1
		float sinOmega = sqrt(1.0f - cosOmega*cosOmega);
		float omega = atan2(sinOmega, cosOmega);
		float oneOverSinOmega = 1.0f/sinOmega;
		k0 = sin((1.0f - t)* omega) * oneOverSinOmega;
		k1 = sin(t*omega) * oneOverSinOmega;
	}

	Quaternion result;
	result.w = k0*From.w + k1*ToW;
	result.x = k0*From.x + k1*ToX;
	result.y = k0*From.y + k1*ToY;
	result.z = k0*From.z + k1*ToZ;
	return result;
}
 Vector3 Math::lerp(const Vector3 & v0, const Vector3& v1, float t)
 {
	 assert(t>=0);
	 assert(t<=1);

	 return v0*(1-t) + v1*t;
 }

 void Math::Translate(const Matrix4 & M, const Vector3 & t)
 {

 }

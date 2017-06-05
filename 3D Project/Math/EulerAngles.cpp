
#include "..\pch.h"

EulerAngle::EulerAngle()
{
}

EulerAngle::EulerAngle(float pitch, float yaw, float roll)
{
	this->pitch = pitch;
	this->yaw = yaw;
	this->roll = roll;
}

EulerAngle::~EulerAngle()
{
}


void EulerAngle::ToMatrixXYZ(Matrix4& m)
{
	float ca = cosf(Math::ToRadian(pitch));
	float sa = sinf(Math::ToRadian(pitch));
	float cb = cosf(Math::ToRadian(yaw));
	float sb = sinf(Math::ToRadian(yaw));
	float cc = cosf(Math::ToRadian(roll));
	float sc = sinf(Math::ToRadian(roll));

	// x axis
	m.mV[0] = cb*cc;
	m.mV[1] = sa*sb*cc + ca*sb;
	m.mV[2] = -ca*sb*cc + sa + sc;

	// y axis
	m.mV[4] = -cb*sc;
	m.mV[5] = -sa*sb*sc + ca*cc;
	m.mV[6] = ca*sb*sc + sa*cc;

	// z axis
	m.mV[8] = sb;
	m.mV[9] = -sa*cb;
	m.mV[10] = ca*cb;

}

Quaternion EulerAngle::ToQuaternion()
{
	float sp,sy,sr;
	float cp,cy,cr;
	Math::SinCos(sp,cp, Math::ToRadian(pitch*0.5f));
	Math::SinCos(sy,cy,Math::ToRadian(yaw*0.5f));
	Math::SinCos(sr,cr,Math::ToRadian(roll*0.5f));

	return Quaternion(cr*cp*cy+ sp*sy*sr, cr*sp*cy + sr*cp*sy,-cr*sp*sy + sy*cp*cy,-sr*sp*cy+ cr*cp*sy);
}

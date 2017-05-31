
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


Quaternion EulerAngle::ToQuaternion()
{
	float sp,sy,sr;
	float cp,cy,cr;
	Math::SinCos(sp,cp, Math::ToRadian(pitch*0.5f));
	Math::SinCos(sy,cy,Math::ToRadian(yaw*0.5f));
	Math::SinCos(sr,cr,Math::ToRadian(roll*0.5f));

	return Quaternion(cr*cp*cy+ sp*sy*sr, cr*sp*cy + sr*cp*sy,-cr*sp*sy + sy*cp*cy,-sr*sp*cy+ cr*cp*sy);
}

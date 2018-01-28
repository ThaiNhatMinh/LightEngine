#include "pch.h"

vector<ICamera*> Camera::CameraList;
vector<ICamera*>::iterator Camera::CurrentCamera;
Camera::Camera()
{
	CameraList.push_back(this);
}

ICamera * Camera::GetCurrentCamera()
{
	return *CurrentCamera;
}

void Camera::SetCurrentCamera(Camera * cam)
{
	auto result = std::find(CameraList.begin(), CameraList.end(), cam);
	if (result == CameraList.end())
	{
		CameraList.push_back(cam);
		CurrentCamera = --CameraList.end();
	}
	else
	{
		CurrentCamera = result;
	}
}

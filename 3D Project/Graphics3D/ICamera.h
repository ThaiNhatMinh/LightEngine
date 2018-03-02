#pragma once


class ICamera
{

public:
	virtual ~ICamera() {};
	virtual const mat4& GetViewMatrix() = 0;
	virtual const mat4& GetProjMatrix() = 0;
	virtual const mat4& GetVPMatrix() = 0;
	virtual const vec3& GetPosition() = 0;
	virtual Frustum*	GetFrustum() = 0;
	virtual const vec3& GetUp() = 0;
	virtual const vec3& GetRight() = 0;
	virtual const vec3& GetFront() = 0;
};


class Camera : public ICamera
{
private:
	//static vector<ICamera*> CameraList;
	//static vector<ICamera*>::iterator	CurrentCamera;
public:
	Camera();
	~Camera() {};

	//static ICamera* GetCurrentCamera();
	//static void		SetCurrentCamera(Camera* cam);
};
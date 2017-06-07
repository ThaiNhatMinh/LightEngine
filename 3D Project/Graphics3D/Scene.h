#pragma once
#include "..\pch.h"

class Scene
{
private:
	Actor*			m_pRoot; // This store every thing
	Camera*			m_Camera;
	Frustum*		m_Frustum;
public:
	Scene();
	~Scene();

	bool OnRender();
	bool OnUpdate(float dt);

	const Camera* GetCamera() {		return m_Camera;	};
	const Frustum* GetFrustum() {		return m_Frustum;	};
	void SetCamera(Camera* pCam) { m_Camera = pCam; };
	void SetFrustum(Frustum* pFrustum) { m_Frustum = pFrustum; };
	Actor* GetRoot() { return m_pRoot; };
	mat4 GetViewProj();
};
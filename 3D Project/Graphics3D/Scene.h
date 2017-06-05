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
	mat4 GetViewProj();
};
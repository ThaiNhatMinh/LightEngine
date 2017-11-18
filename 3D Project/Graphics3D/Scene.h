#pragma once
#include "pch.h"
#include "RenderAPI.h"

class Scene
{
private:
	// Simple Object with only transform component
	// This store every thing in scene
	Actor*			m_pRoot;

	Camera*			m_Camera;
	Frustum*		m_Frustum;
	Light			m_DirectionLight; // only one direction light
	CameraComponent *m_pCameraNode;

	RenderAPICore* m_pRenderer;
public:
	Scene(RenderAPICore* pRender);
	~Scene();

	bool OnRender();
	bool OnUpdate(float dt);

	const Camera* GetCamera() {		return m_Camera;	};
	const Frustum* GetFrustum() {		return m_Frustum;	};
	void SetCamera(Camera* pCam) { m_Camera = pCam; };
	void SetCameraNode(Actor* pActor);
	void SetFrustum(Frustum* pFrustum) { m_Frustum = pFrustum; };
	Actor* GetRoot() { return m_pRoot; };
	mat4 GetViewProj();
	Light GetDirLight() { return m_DirectionLight; };

	RenderAPICore* GetRenderer() { return m_pRenderer; }
};
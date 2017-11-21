#pragma once
#include "pch.h"
#include "RenderAPI.h"

#include "Core\Camera.h"
class Scene
{
private:
	// Simple Object with only transform component
	// This store every thing in scene
	Actor*				m_pRoot;

	Camera				m_DefaultCamera;
	
	Light				m_DirectionLight; // only one direction light
	CameraComponent		*m_CurrentCamera;

	RenderAPICore*		m_pRenderer;
	Debug				m_Debug;
	ActorFactory		m_ActorFactory;
public:
	Scene(RenderAPICore* pRender);
	~Scene();

	bool OnRender();
	bool OnUpdate(float dt);

	/*	No longer need this. Delete it if you want
	const Camera* GetCamera() {		return m_DefaultCamera;	};
	const Frustum* GetFrustum() {		return m_Frustum;	};
	void SetCamera(Camera* pCam) { m_DefaultCamera = pCam; };
	void SetCameraNode(Actor* pActor);
	void SetFrustum(Frustum* pFrustum) { m_Frustum = pFrustum; };
	
	*/
	Actor* GetRoot() { return m_pRoot; };
	Light GetDirLight() { return m_DirectionLight; };
	void SetCamera(CameraComponent* pActor);
	mat4 GetViewProj();
	Debug& GetDebug() { return m_Debug; }
	ActorFactory& GetActorFactory() { return m_ActorFactory; }
	RenderAPICore* GetRenderer() { return m_pRenderer; }
};
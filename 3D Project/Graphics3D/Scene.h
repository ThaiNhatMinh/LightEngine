#pragma once
#include "pch.h"
#include "RenderAPI.h"

#include "Camera.h"
class Scene
{
private:
	// Simple Object with only transform component
	// This store every thing in scene
	std::unique_ptr<Actor>				m_pRoot;

	Camera				m_DefaultCamera;
	
	Light				m_DirectionLight; // only one direction light
	CameraComponent		*m_CurrentCamera;

	
	ActorFactory		m_ActorFactory;
	Context*			m_Context;
public:
	Scene(Context* c);
	~Scene();

	bool LoadScene(const char* filename);
	bool OnRender();
	bool OnUpdate(float dt);

	
	Actor* GetRoot() { return m_pRoot.get(); };
	Light GetDirLight() { return m_DirectionLight; };
	void SetCamera(CameraComponent* pActor);
	mat4 GetViewProj();
	ActorFactory& GetActorFactory() { return m_ActorFactory; }
	CameraComponent* GetCamera() { return m_CurrentCamera; }
	Camera* GetDefaultCamera() { return &m_DefaultCamera; }
};
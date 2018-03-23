#pragma once
#include <pch.h>
#include "RenderAPI.h"

#include "Camera.h"
class Scene
{
private:
	// Simple Object with only transform component
	// This store every thing in scene
	std::unique_ptr<Actor>	m_pRoot;
	//std::unique_ptr<Actor>	m_pSkyBox;
	std::list<Actor*>		m_ActorLast;
	DirectionLight					m_DirectionLight; // only one direction light
	vector<ICamera*>		m_CameraList;
	ICamera*				m_CurrentCamera;
	
	
	Context*			m_Context;

public:
	Scene(Context* c);
	~Scene();

	bool LoadScene(const string& filename);
	bool OnRender();

	bool OnUpdate(float dt);
	bool PostUpdate();
	
	ICamera*	GetCurrentCamera();
	void		SetCurrentCamera(Camera * cam);
	void		PushLastActor(Actor*);
	Actor*		GetRoot() { return m_pRoot.get(); };
	DirectionLight		GetDirLight() { return m_DirectionLight; };
};
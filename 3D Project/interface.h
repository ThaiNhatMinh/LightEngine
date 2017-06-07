#pragma once
#include "pch.h"

#define MAX_NAME 128

typedef unsigned int ActorId;
typedef unsigned int ComponentId;


class Scene;
class Actor;
class SceneNode;
class Mesh;
class Windows;
struct Texture;

struct Material
{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float exp;
};

// mesh interface
class IMesh
{
public:
	string					Name;
	unsigned int			VAO;
	unsigned int			VBO;
	unsigned int			EBO;
	unsigned int			NumIndices;
	Texture*				Tex;
	GLuint					Topology;
	Material				mat;
	// Use to generate Vertex Buffer Object, Vertex Array Object.
	virtual void Finalize(Shader* p) = 0;
	// Virtual destructor
	virtual ~IMesh() {};
};

class ISceneNode
{
public:

	virtual void VSetName(string name) = 0;
	virtual string VGetName() = 0;

	virtual Shader* VGetShader() = 0;
	virtual void VSetShader(Shader* p) = 0;
	virtual void VSetTransform(const mat4 *toWorld) = 0;
	virtual mat4 VGetTransform() = 0;

	virtual HRESULT VOnUpdate(Scene *pScene, DWORD const elapsedMs) = 0;

	virtual ActorId GetId(void) const = 0;
	virtual HRESULT VPreRender(Scene *pScene) = 0;
	virtual bool VIsVisible(Scene *pScene) const = 0;
	virtual HRESULT VRender(Scene *pScene) = 0;
	virtual HRESULT VRenderChildren(Scene *pScene) = 0;
	virtual HRESULT VPostRender(Scene *pScene) = 0;

	virtual bool VAddChild(Actor* kid) = 0;
	virtual bool VRemoveChild(ActorId id) =0;

	

	//virtual HRESULT VOnLostDevice(Scene *pScene) = 0;
	//virtual HRESULT VPick(Scene *pScene, RayCast *pRayCast) = 0;

	virtual ~ISceneNode() { };
};
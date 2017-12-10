#pragma once
#include "pch.h"

// This component using to render model for actor
// If m_MeshList is null mean 
class MeshRenderComponent : public ActorComponent
{
protected:
	vector<IMesh*>			m_MeshList;
	Shader					*m_pShader;
	Material				m_Material;
public:
	static const char* Name;
	
	MeshRenderComponent(void) {}
	MeshRenderComponent(const vector<IMesh*>& v) { m_MeshList = v; }
	virtual ~MeshRenderComponent() {};
	
	virtual bool VInit(const tinyxml2::XMLElement* pData);
	virtual void VPostInit(void);
	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument*p);
	virtual const char* VGetName() const { return Name; }

	// Question need to answer
	// Let this component render or just return m_MeshList for other sub-system
	// For easy now just render it
	// Request mesh and texture
	virtual void Render(Scene*);
	vector<IMesh*>& GetMeshList();
};
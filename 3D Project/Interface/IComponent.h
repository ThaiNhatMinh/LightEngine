#pragma once

#include <glm\mat4x4.hpp>
#include "..\Graphics3D\ModelRender.h"
namespace Light 
{
	class IActor;
	class IComponent
	{
	public:
		virtual ~IComponent() = default;

		virtual bool VInit(IContext* pContext, const tinyxml2::XMLElement* pData) = 0;
		// Not use now, update later
		virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument*p) {return nullptr;};
		virtual void VPostInit(void) {};
		virtual void VUpdate(float dt) {};
		virtual void VPostUpdate() {};
		IActor* GetOwner() {			return m_pOwner;		};
		void SetOwner(IActor* pActor) {			m_pOwner = pActor;		};
	private:
		IActor* m_pOwner;
	};


	/// really simple component

	class TransformComponent :public IComponent
	{
	public:
		glm::mat4 transform;
	};


	class MeshRenderComponent : public IComponent
	{
	public:
		render::ModelRender* m_pModel;
	};
}

#pragma once
#include "Material.h"
#include <list>
#include <string>
namespace Light
{
	namespace render
	{
		struct Renderable
		{
			IMeshRenderComponent*	m_RenderComponent;
			ITransformComponent*	m_TransformComponent;
			IActor*					m_pActor;
			// More data because when object is destroyed we can't access m_pActor any more.
			ActorId					m_ActorID;
		};

		class RenderDevice;
		class RenderPass
		{
		protected:

			std::string m_name;
			RenderDevice* pRenderer;
			std::list<Renderable> m_ObjectList;
		public:
			virtual ~RenderPass() = default;
			virtual void Render(const glm::mat4& pv) = 0;

			virtual void AddRenderObject(Renderable& Obj) = 0;
			virtual IActor* RemoveRenderObject(ActorId id) = 0;
			const std::string& GetName() { return m_name; };
		};
	}
}
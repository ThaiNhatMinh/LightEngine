#pragma once


#include "..\Interface\IComponent.h"
#include "..\Animation\AnimationLayer.h"
#include "..\Utilities\Utility.h"
namespace Light
{
	class AnimatorComponent : public IAnimatorComponent, public util::Updatable, public util::PreRenderable
	{
	public:
		virtual bool		VSerialize(IContext*pContext, const tinyxml2::XMLElement * pData);
		virtual tinyxml2::XMLElement* VDeserialize(tinyxml2::XMLDocument*p);

		virtual void		Play(const std::string& name, bool loop, const std::string& layer="Default");

		

		virtual void		VUpdate(float dt);
		virtual void		VPreRender(render::Material::MatrixParam& param)override;
		virtual math::AABB	GetBox();
	private:
		
		
	private:
		int								m_iNumNode;
		std::vector<std::unique_ptr<AnimationLayer>>		m_Layers;
		std::vector<FrameData>			m_CurrentFrame;

		std::vector<glm::mat4>			m_SkeTransform;			// matrix using to transform vertex, include invert bind-pose matrix
		std::vector<glm::mat4>			m_DbTransform;			// matrix using to debug
		SkeNode*						m_pSkeNodes;
	};
}
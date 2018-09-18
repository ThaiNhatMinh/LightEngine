#pragma once


#include "..\Interface\IComponent.h"
#include "..\Animation\AnimationState.h"
#include "..\Utilities\Utility.h"
namespace Light
{
	class AnimatorComponent : public IAnimatorComponent, public util::Updatable, public util::PreRenderable
	{
	public:
		virtual bool		VSerialize(IContext*pContext, const tinyxml2::XMLElement * pData);
		virtual tinyxml2::XMLElement* VDeserialize(tinyxml2::XMLDocument*p);

		virtual void		Play(const std::string& name, bool loop, float fadeinTime);

		void				SetCurrentState(AnimationState* pState);

		virtual void		VUpdate(float dt);
		virtual void		VPreRender(render::Material::MatrixParam& param)override;
	private:
		Animation* GetAnimation(LTRawData* pData,const std::string& name);
		AnimationState* GetState(const std::string& name);
	private:
		// All state availble in object
		std::vector<AnimationState> m_AllState;
		AnimationState* m_CurrentAnimation;

		//render::Model* m_pRenderModel;
	};
}
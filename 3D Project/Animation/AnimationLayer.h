#pragma once
#include "AnimationState.h"
#include "..\Utilities\Utility.h"
#include <vector>
#include <string>
namespace Light
{
	class AnimationLayer: public util::Serialization
	{
		enum MaskType
		{
			MASK_DEFAULT = 0,	// from xml data
			MASK_FROMMODEL,		// from ltb model
			MASK_ZERO,
			MASK_ONE
		};
	private:
		std::string			m_Name;
		float				m_fWeight;
		std::vector<float>	m_Masks;
		int					m_iNumNode;

	private:
		// All state availble in object
		std::vector<AnimationState> m_AllState;
		AnimationState* m_CurrentAnimation;
	public:
		AnimationLayer(int numNode);
		virtual bool					VSerialize(IContext*pContext, const tinyxml2::XMLElement * pData);
		virtual tinyxml2::XMLElement*	VDeserialize(tinyxml2::XMLDocument*p);
		void							Update(float dt);
		Light::FrameData				ComputerFrame(int i);
		void							SetCurrentState(AnimationState* pState);
		AnimationState*					GetCurrentState();
		void							Play(const std::string & name, bool loop);
		const std::string&				GetName();
	private:
		Animation*						GetAnimation(LTRawData* pData, const std::string& name);
		AnimationState*					GetState(const std::string& name);
	};
}
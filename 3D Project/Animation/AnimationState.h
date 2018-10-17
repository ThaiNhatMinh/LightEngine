#pragma once
#include <vector>

#include "..\Utilities\LTBStruct.h"

namespace Light
{
	// store state of animation state machine
	// manage 
	class AnimationLayer;
	class AnimationState
	{
		friend class AnimationLayer;
	private:
		
		AnimationLayer*					m_pOwner;
		std::vector<AnimationState*>	m_NextState;
		bool							m_bLoop;
		std::string						m_Name;
		AnimationState*					m_TransitionState;

		Animation*						m_pAnimData;
		float							m_fTime;
		uint32							m_iCurrentFrame;
		uint32							KeyFrameID;
		float							m_fSpeed;
		float							m_fTransitionTime;		// transition time between current state and next state
		bool							m_bFinish;
	public:
		AnimationState();
		void							Transition(const std::string& name, bool loop);
		void							Update(float dt);
		const std::string&				GetName();

		FrameData						ComputerFrame(int i);
		void							SetSpeed(float speed);
		bool							IsFinish();
		math::AABB						GetBox();
	private:
		FrameData						InterpolateFrame(AnimationState* state,const AnimNode & Anim, const vector<AnimKeyFrame>& KeyFrames);
		void							AnimEvent(const string&);
		void							OnFinish();
	};
}
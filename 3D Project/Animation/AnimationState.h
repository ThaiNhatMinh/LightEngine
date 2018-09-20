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

		//std::vector<FrameData>			m_CurrentFrame;
		//SkeNode*						m_pSkeNodes;
		//std::vector<glm::mat4>			m_SkeTransform;			// matrix using to transform vertex, include invert bind-pose matrix
		//std::vector<glm::mat4>			m_DbTransform;			// matrix using to debug
	public:
		AnimationState();
		void							Transition(const std::string& name, bool loop);
		void							Update(float dt);
		const std::string&				GetName();

		FrameData						ComputerFrame(int i);

		///float*							GetTransformMatrixs();
		//int								GetNumNodes();
		void							SetSpeed(float speed);
	private:
		FrameData						InterpolateFrame(AnimationState* state,const AnimNode & Anim, const vector<AnimKeyFrame>& KeyFrames);
		void							AnimEvent(const string&);
		void							OnFinish();
	};
}
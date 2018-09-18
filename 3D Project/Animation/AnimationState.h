#pragma once
#include <vector>

#include "..\Utilities\LTBStruct.h"

namespace Light
{
	// store state of animation state machine
	// manage 
	class AnimatorComponent;
	class AnimationState
	{
		friend class AnimatorComponent;
	private:
		
		AnimatorComponent*				m_pOwner;
		std::vector<AnimationState*>	m_NextState;
		bool							m_bLoop;
		std::string						m_Name;
		AnimationState*					m_TransitionState;
		uint32							m_iNumNode;

		Animation*						m_pAnimData;
		float							m_fTime;
		uint32							m_iCurrentFrame;
		uint32							KeyFrameID;

		float							m_fTransitionTime;		// transition time between current state and next state

		std::vector<FrameData>			m_CurrentFrame;
		SkeNode*						m_pSkeNodes;
		std::vector<glm::mat4>			m_SkeTransform;			// matrix using to transform vertex, include invert bind-pose matrix
		std::vector<glm::mat4>			m_DbTransform;			// matrix using to debug
	public:
		AnimationState(int numNode);
		void							Transition(const std::string& name, bool loop);
		void							Update(float dt);
		const std::string&				GetName();

		void							ComputerFrame(int i);

		float*							GetTransformMatrixs();
		int								GetNumNodes();
	private:
		FrameData						InterpolateFrame(AnimationState* state,const AnimNode & Anim, const vector<AnimKeyFrame>& KeyFrames);
		void							AnimEvent(const string&);
		void							OnFinish();
	};
}
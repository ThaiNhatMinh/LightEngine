#pragma once

#include "..\Interface\Renderer.h"
#include "Particles.h"
namespace Light
{
	namespace resources
	{
		struct SpriteData;
	}
	namespace render
	{
		class Sprite: public BaseParticle
		{

		public:
			Sprite() :m_MsCurTime(0), m_iCurrentFrame(0), m_Loop(0){};
			

		public:

			std::vector<render::Texture*>m_FrameLists;
			resources::SpriteData* m_pData;
			
			uint32					m_MsCurTime;
			uint32					m_iCurrentFrame;
			glm::vec3				m_Pos;
			float					m_fLife;
			bool					m_Loop;
			glm::vec2				m_Size;
		};
	}
}
#pragma once

#include "ISubSystem.h"
#include <glm/vec2.hpp>
#include "Renderer.h"
#include "IResourceManager.h"
namespace Light
{
	namespace vgui
	{
		class Font
		{
		public:
			virtual ~Font() = default;
			virtual render::Texture* GetCharTex(uint32 c) = 0;
		};

		class IVGUI : public ISubSystem
		{
		public:

			virtual void	VDrawText(const std::string& str, const  glm::vec2& position,vgui::Font* pFont=nullptr) = 0;
			virtual void	VDraw(render::Texture* pTex, const glm::vec2&position, const glm::vec4&color = glm::vec4(1), const glm::vec2& scale = glm::vec2(1, 1)) = 0;
			virtual void	VSetDefaultFont(Font*) = 0;
			virtual Font*	VCreateFont(const std::string& filename,int fontsize) = 0;

		};
	}
}
#include "pch.h"
#include "Material.h"
#include "../Interface/Renderer.h"
namespace Light
{
	using namespace render;

	PipelineParam * LightParam::operator[](const char * name)
	{
		if (strcmp(name, "Ia") == 0) return Ia;
		if (strcmp(name, "Id") == 0) return Id;
		if (strcmp(name, "Is") == 0) return Is;
		auto r = m_Extra.find(name);
		if(r==m_Extra.end()) return nullptr;

		return (*r).second;
	}

	const PipelineParam * render::LightParam::operator[](const char *) const
	{
		return nullptr;
	}

	void render::LightParam::AddParam(const char * name, PipelineParam * param)
	{
		if (strcmp(name, "Ia") == 0) Ia = param;
		else if (strcmp(name, "Id") == 0) Id = param;
		if (strcmp(name, "Is") == 0) Is = param;
		else
		{
			auto r = m_Extra.find(name);
			if (r != m_Extra.end()) return;

			m_Extra.insert(std::pair<const char*, PipelineParam*>(name, param));
		}
	}

}
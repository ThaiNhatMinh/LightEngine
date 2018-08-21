#pragma once
#include <vector>
#include <glm\vec3.hpp>
#include <glm\vec2.hpp>
#include "Mesh.h"
#include "..\ResourceManager\LTRawData.h"

class LTRawData;
namespace Light
{
	struct Weight
	{
		Weight(int b, float w) { Bone = (float)b; weight = w; };
		Weight() :Bone(0), weight(0) {};
		float Bone;
		float weight;
	};

	typedef std::vector<Weight> WeightList;
	struct SkeVertex
	{
		glm::vec3		pos;
		glm::vec3		normal;
		glm::vec2		uv;
		Weight		weights[4];

	};


	class SkeMesh :public Mesh
	{
	public:
		typedef std::vector<SkeVertex> VertexList;
		typedef std::vector<unsigned int> IndicesList;
	private:
		std::string m_Name;
		std::unique_ptr<render::VertexArray>		m_pVAO;
		std::unique_ptr<render::VertexBuffer>		m_pVBO;
		std::unique_ptr<render::IndexBuffer>		m_pIBO;

	public:
		SkeMesh(render::RenderDevice* pRenderDevice,LTRawMesh* pData);
		~SkeMesh() {};
	};
}
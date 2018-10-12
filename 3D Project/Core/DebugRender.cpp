#include "pch.h"
#include "DebugRender.h"
#include "..\Interface\IResourceManager.h"
namespace Light
{
	DebugRender::DebugRender(IContext * pContext)
	{
		pContext->VAddSystem(this);
		m_pRS = pContext->GetSystem<render::IRenderSystem>();
		m_pRenderer = m_pRS->GetRenderDevice();
		auto pResources = pContext->GetSystem<resources::IResourceManager>();

		

		SetupLineRender(pResources);
		SetupBoxRender(pResources);
	}

	void DebugRender::DrawLine(const vec3 & from, const vec3 & to, const vec3 & color, const mat4 & m)
	{
		DebugData db;
		db.color = color;
		db.pos = m * vec4(from, 1.0f);
		m_Lists.push_back(db);
		db.pos = m * vec4(to, 1.0f);
		m_Lists.push_back(db);
	}

	void DebugRender::DrawLineBox(vec3 min, vec3 max, vec3 color, const mat4 & m)
	{
		DrawLine(vec3(min.x, min.y, min.z), vec3(max.x, min.y, min.z),color,m);
		DrawLine(vec3(max.x, min.y, min.z), vec3(max.x, min.y, max.z), color,m);
		DrawLine(vec3(max.x, min.y, max.z), vec3(min.x, min.y, max.z), color,m);
		DrawLine(vec3(min.x, min.y, max.z), vec3(min.x, min.y, min.z), color,m);

		DrawLine(vec3(min.x, max.y, min.z), vec3(max.x, max.y, min.z), color,m);
		DrawLine(vec3(max.x, max.y, min.z), vec3(max.x, max.y, max.z), color,m);
		DrawLine(vec3(max.x, max.y, max.z), vec3(min.x, max.y, max.z), color,m);
		DrawLine(vec3(min.x, max.y, max.z), vec3(min.x, max.y, min.z), color,m);

		DrawLine(vec3(min.x, max.y, min.z), vec3(min.x, min.y, min.z), color,m);
		DrawLine(vec3(max.x, max.y, min.z), vec3(max.x, min.y, min.z), color,m);
		DrawLine(vec3(max.x, max.y, max.z), vec3(max.x, min.y, max.z), color,m);
		DrawLine(vec3(min.x, max.y, max.z), vec3(min.x, min.y, max.z), color,m);
	}

	void DebugRender::DrawCoord(const mat4 & m)
	{
		vec3 pos = m[3];
		vec3 x = m[0];
		vec3 y = m[1];
		vec3 z = m[2];
		DrawLine(pos, pos + x * 10.0f, vec3(1, 0, 0));
		DrawLine(pos, pos + y * 10.0f, vec3(0, 1, 0));
		DrawLine(pos, pos + z * 10.0f, vec3(0, 0, 1));
	}

	void DebugRender::AddLightBox(glm::vec3 pos, glm::vec3 color)
	{
		LightData data;
		data.color = color;
		data.model = glm::translate(glm::mat4(), pos);
		m_Lights.push_back(data);

	}

	void DebugRender::Render()
	{
		
		auto camera = m_pRS->VGetCurrentCamera();

		glm::mat4 PV = camera->GetProjMatrix() * camera->GetViewMatrix();


		RenderLight(PV);

		if (m_Lists.size() == 0) return;

		m_VBO->SetData(sizeof(DebugData)*m_Lists.size(), &m_Lists[0], render::STREAM_DRAW);
		
		m_pRenderer->SetPipeline(m_Shader.get());
		m_pRenderer->SetVertexArray(m_VAO.get());
		m_uMVP->SetAsMat4(glm::value_ptr(PV));
		m_pRenderer->Draw(0, m_Lists.size(), 0, render::PRIMITIVE_LINES);
		m_Lists.clear();

	}

	void DebugRender::RenderLight(const glm::mat4& pv)
	{
		
		m_pRenderer->SetPipeline(m_ShaderLight.get());
		m_pRenderer->SetVertexArray(m_VAOLightBox.get());
		m_pRenderer->SetIndexBuffer(m_IBOLightBox.get());
		m_uMVPLight->SetAsMat4(glm::value_ptr(pv));
		for (auto light : m_Lights)
		{
			m_uModelLight->SetAsMat4(glm::value_ptr(light.model));
			m_uColor->SetAsVec3(glm::value_ptr(light.color));
			m_pRenderer->DrawElement(36, 0);
		}
	}
	const char * DebugRender::VGetName()
	{
		static const char* pName = typeid(IDebugRender).name();
		return pName;
	}

	void DebugRender::SetupBoxRender(resources::IResourceManager*pResources)
	{

		m_ShaderLight = std::unique_ptr<render::Pipeline>(m_pRenderer->CreatePipeline(pResources->VGetVertexShader("Debug_Box"), pResources->VGetPixelShader("Debug_Box")));
		m_uMVPLight = m_ShaderLight->GetParam(render::uMVP);
		m_uModelLight = m_ShaderLight->GetParam(render::uMODEL);
		m_uColor = m_ShaderLight->GetParam(render::uColor);


		std::vector<glm::vec3> vertex =
		{
			glm::vec3(1.0,1.0,1.0),
			glm::vec3(1.0,1.0,-1.0),
			glm::vec3(-1.0,1.0,1.0),
			glm::vec3(-1.0,1.0,-1.0),

			glm::vec3(1.0,-1.0,1.0),
			glm::vec3(1.0,-1.0,-1.0),
			glm::vec3(-1.0,-1.0,1.0),
			glm::vec3(-1.0,-1.0,-1.0),
		};
		std::vector<unsigned int> indices = { 0,1,2,2,1,3,
			4,5,6,6,5,7,
			0,4,5,0,5,1,
			1,5,7,1,7,3,
			3,7,6,3,6,2,
			2,6,4,2,4,0 };

		render::VertexElement elementList = { render::SHADER_POSITION_ATTRIBUTE,render::VertexElementType::VERTEXELEMENTTYPE_FLOAT,3,sizeof(glm::vec3),0 };

		m_VBOLightBox = std::unique_ptr<render::VertexBuffer>(m_pRenderer->CreateVertexBuffer(sizeof(glm::vec3)*vertex.size(), &vertex[0]));
		m_IBOLightBox = std::unique_ptr<render::IndexBuffer>(m_pRenderer->CreateIndexBuffer(sizeof(unsigned int)*indices.size(), &indices[0]));
		render::VertexDescription* pVertexDes = m_pRenderer->CreateVertexDescription(1, &elementList);
		render::VertexBuffer* pTemp = m_VBOLightBox.get();

		m_VAOLightBox = std::unique_ptr<render::VertexArray>(m_pRenderer->CreateVertexArray(1, &pTemp, &pVertexDes));
		delete pVertexDes;
	}

	void DebugRender::SetupLineRender(resources::IResourceManager*pResources)
	{
		m_Shader = std::unique_ptr<render::Pipeline>(m_pRenderer->CreatePipeline(pResources->VGetVertexShader("Debug_Line"), pResources->VGetPixelShader("Debug_Line")));
		m_uMVP = m_Shader->GetParam(render::uMVP);

		

		m_DepthConfig = std::unique_ptr<render::DepthState>(m_pRenderer->CreateDepthState(false,true,render::COMPARE_LESS));
		
		m_VBO = std::unique_ptr<render::VertexBuffer>(m_pRenderer->CreateVertexBuffer(0));
		render::VertexElement elements[] =
		{
			{ render::SHADER_POSITION_ATTRIBUTE, render::VertexElementType::VERTEXELEMENTTYPE_FLOAT, 3,sizeof(DebugData),0 },
			{ render::SHADER_COLOR_ATTRIBUTE, render::VertexElementType::VERTEXELEMENTTYPE_FLOAT, 3,sizeof(DebugData),sizeof(vec3) }
		};

		render::VertexDescription* pVertexDes = m_pRenderer->CreateVertexDescription(2, elements);
		auto p = m_VBO.get();
		m_VAO = std::unique_ptr<render::VertexArray>(m_pRenderer->CreateVertexArray(1, &p, &pVertexDes));
		delete pVertexDes;
	}



}


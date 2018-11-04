#include "pch.h"
#include "SkyBox.h"

#include <experimental/filesystem>
#include "..\Interface\IResourceManager.h"

namespace fs = std::experimental::filesystem;

using namespace Light;

Light::render::SkyBox::SkyBox()
{

}

bool Light::render::SkyBox::VSerialize(IContext * pContext, const tinyxml2::XMLElement * pData)
{
	if (pContext == nullptr || pData == nullptr)
	{
		E_WARNING("[Skybox] Input data is null");
		return false;
	}
	auto pResourceManager = pContext->GetSystem<resources::IResourceManager>();
	m_pRS = pContext->GetSystem<IRenderSystem>();
	m_pRenderer = m_pRS->GetRenderDevice();

	auto pFolderNode = pData->FirstChildElement("Texture");
	const char* Path = pFolderNode->Attribute("Path");
	m_CubeTex = m_pRS->VCreateTexture(pResourceManager->VGetTexture(GetFileList(Path,pFolderNode),true));


	auto VS = m_pRenderer->CreateVertexShader(pResourceManager->VGetShaderCode("SkyBox.vs")->Get());
	auto PS = m_pRenderer->CreatePixelShader(pResourceManager->VGetShaderCode("SkyBox.fs")->Get());
	m_Shader = std::unique_ptr<Pipeline>(m_pRenderer->CreatePipeline(VS, PS));
	m_uMVP = m_Shader->GetParam(uMVP);

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

	VertexElement elementList = { SHADER_POSITION_ATTRIBUTE,VertexElementType::VERTEXELEMENTTYPE_FLOAT,3,sizeof(glm::vec3),0 };

	m_VBO = std::unique_ptr<VertexBuffer>(m_pRenderer->CreateVertexBuffer(sizeof(glm::vec3)*vertex.size(), &vertex[0]));
	m_IBO = std::unique_ptr<IndexBuffer>(m_pRenderer->CreateIndexBuffer(sizeof(unsigned int)*indices.size(), &indices[0]));
	render::VertexDescription* pVertexDes = m_pRenderer->CreateVertexDescription(1, &elementList);
	VertexBuffer* pTemp = m_VBO.get();

	m_VAO = std::unique_ptr<VertexArray>(m_pRenderer->CreateVertexArray(1, &pTemp, &pVertexDes));

	
	m_DepthState = std::unique_ptr<DepthState>(m_pRenderer->CreateDepthState(true,true, COMPARE_LEQUAL));
	
	{
		CullFaceConfig config;
		config.CullEnable = false;
		m_CullState = std::unique_ptr<RasterState>(m_pRenderer->CreateRasterState(config));
	}


	delete pVertexDes;

	return true;
}

tinyxml2::XMLElement * Light::render::SkyBox::VDeserialize(tinyxml2::XMLDocument * p)
{
	return nullptr;
}

void Light::render::SkyBox::VRender()
{
	auto pCamera = m_pRS->VGetCurrentCamera();
	glm::mat4 view = pCamera->GetViewMatrix();
	view = glm::mat3(view);
	glm::mat4 projection = pCamera->GetProjMatrix();

	glm::mat4 mvp = projection * view;
	m_pRenderer->SetDepthState(m_DepthState.get());
	m_pRenderer->SetRasterState(m_CullState.get());
	m_pRenderer->SetPipeline(m_Shader.get());
	m_uMVP->SetAsMat4(glm::value_ptr(mvp));
	
	m_pRenderer->SetTexture(render::UNIT_SKYBOX, m_CubeTex);
	m_pRenderer->SetVertexArray(m_VAO.get());
	m_pRenderer->SetIndexBuffer(m_IBO.get());
	m_pRenderer->DrawElement(36, 0);



}

render::Texture * Light::render::SkyBox::GetTexture()
{
	return m_CubeTex;
}

std::vector<std::string> Light::render::SkyBox::GetFileList(std::string folder, const tinyxml2::XMLElement *pData)
{
	std::vector<std::string> filelist;

	for (auto pNodeTex = pData->FirstChildElement(); pNodeTex; pNodeTex = pNodeTex->NextSiblingElement())
	{
		const char* pFile = pNodeTex->Attribute("File");
		filelist.push_back(folder + pFile);
	}
	return filelist;
}

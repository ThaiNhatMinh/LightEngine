#include "pch.h"
#include "DefaultMaterial.h"
#include "Renderer.h"
#include "Interface\IResourceManager.h"
namespace Light
{
	namespace render
	{
		DefaultMaterial::DefaultMaterial(IContext* pContext)
		{
			auto pRenderer = pContext->GetSystem<RenderDevice>();
			auto pResources = pContext->GetSystem<resources::IResourceManager>();
			auto pVertexShader = pResources->VGetVertexShader("Default");
			auto pPixelShader = pResources->VGetPixelShader("Default");
			m_Pipeline = std::unique_ptr<Pipeline>(pRenderer->CreatePipeline(pVertexShader, pPixelShader));
			//m_ShaderName = std::make_pair(pVertexNode->GetText(), pPixelNode->GetText());

			this->GetUniform();
		}
		/*bool DefaultMaterial::VSerialize(IContext*pContext,const tinyxml2::XMLElement * pData)
		{
			auto pRenderer = pContext->GetSystem<RenderDevice>();
			auto pResources = pContext->GetSystem<resources::IResourceManager>();

			const tinyxml2::XMLElement* pMaterialData = pData->FirstChildElement("coefficients");
			const tinyxml2::XMLElement* pKa = pMaterialData->FirstChildElement("Ka");
			Ka.x = pKa->FloatAttribute("r", 1.0f);
			Ka.y = pKa->FloatAttribute("g", 1.0f);
			Ka.z = pKa->FloatAttribute("b", 1.0f);
			const tinyxml2::XMLElement* pKd = pMaterialData->FirstChildElement("Kd");
			Kd.x = pKd->FloatAttribute("r", 1.0f);
			Kd.y = pKd->FloatAttribute("g", 1.0f);
			Kd.z = pKd->FloatAttribute("b", 1.0f);
			const tinyxml2::XMLElement* pKs = pMaterialData->FirstChildElement("Ks");
			Ks.x = pKs->FloatAttribute("r", 1.0f);
			Ks.y = pKs->FloatAttribute("g", 1.0f);
			Ks.z = pKs->FloatAttribute("b", 1.0f);
			exp = vec3(pKs->FloatAttribute("exp", 32.0f));

			auto pShaderNode = pData->FirstChildElement("Shader");
			auto pVertexNode = pShaderNode->FirstChildElement("VertexShader");
			auto pPixelNode = pShaderNode->FirstChildElement("PixelShader");
			auto pVertexShader = pResources->VGetVertexShader(pVertexNode->GetText());
			auto pPixelShader = pResources->VGetPixelShader(pPixelNode->GetText());
			m_Pipeline = std::unique_ptr<Pipeline>(pRenderer->CreatePipeline(pVertexShader, pPixelShader));
			m_ShaderName = std::make_pair(pVertexNode->GetText(),pPixelNode->GetText());

			this->GetUniform();

			return true;

		}
		tinyxml2::XMLElement * DefaultMaterial::VDeserialize(tinyxml2::XMLDocument * p)
		{
			tinyxml2::XMLElement* pCoefficients = p->NewElement("coefficients");
			tinyxml2::XMLElement* pKa = p->NewElement("Ka"); pCoefficients->InsertEndChild(pKa);
			tinyxml2::XMLElement* pKd = p->NewElement("Kd"); pCoefficients->InsertEndChild(pKd);
			tinyxml2::XMLElement* pKs = p->NewElement("Ks"); pCoefficients->InsertEndChild(pKs);
			tinyxml2::XMLElement* pShaderNode = p->NewElement("Shader");
			tinyxml2::XMLElement* pVertexShader = p->NewElement("VertexShader"); pShaderNode->InsertEndChild(pVertexShader);
			tinyxml2::XMLElement* pPixelShader = p->NewElement("PixelShader"); pShaderNode->InsertEndChild(pPixelShader);

			pKa->SetAttribute("r", Ka.x);
			pKa->SetAttribute("g", Ka.y);
			pKa->SetAttribute("b", Ka.z);
			pKd->SetAttribute("r", Kd.x);
			pKd->SetAttribute("g", Kd.y);
			pKd->SetAttribute("b", Kd.z);
			pKs->SetAttribute("r", Ks.x);
			pKs->SetAttribute("g", Ks.y);
			pKs->SetAttribute("b", Ks.z);
			pVertexShader->SetText(m_ShaderName.first.c_str());
			pPixelShader->SetText(m_ShaderName.second.c_str());

			tinyxml2::XMLElement* pMaterial = p->NewElement("Material");
			pMaterial->InsertEndChild(pCoefficients);
			pMaterial->InsertEndChild(pShaderNode);
			return pMaterial;
		}*/
		void DefaultMaterial::Apply(RenderDevice* renderer, const float * model, const float * mvp)
		{
			assert(model != nullptr && mvp != nullptr);
			renderer->SetPipeline(m_Pipeline.get());

			
			if(m_ModelUniform) m_ModelUniform->SetAsMat4(model);
			m_MVPUniform->SetAsMat4(mvp);
		}
		
		MaterialType DefaultMaterial::GetType()
		{
			static std::size_t type = typeid(DefaultMaterial).hash_code();
			return type;
		}
		void DefaultMaterial::SetPipeline(Pipeline * pipeline)
		{
			m_Pipeline.reset();
			m_Pipeline.reset(pipeline);
			this->GetUniform();

		}
		void DefaultMaterial::GetUniform()
		{
			assert(m_Pipeline != nullptr);
			m_ModelUniform = m_Pipeline->GetParam(uMODEL);
			m_MVPUniform = m_Pipeline->GetParam(uMVP);
		}
	}
}
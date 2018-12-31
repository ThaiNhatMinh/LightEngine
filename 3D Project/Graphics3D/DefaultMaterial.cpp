#include "pch.h"
#include "Scene.h"
#include "DefaultMaterial.h"
#include "..\Interface\Renderer.h"
#include "Interface\IResourceManager.h"
#include "..\Interface\IScene.h"
#include "..\Interface\IEventManager.h"
#include "..\Core\Events.h"

namespace Light
{
	namespace render
	{
		DefaultMaterial::DefaultMaterial(IContext* pContext) :m_pContext(pContext)
		{
			auto pRenderer = pContext->GetSystem<IRenderSystem>()->GetRenderDevice();
			auto pResources = pContext->GetSystem<resources::IResourceManager>();
			auto VS = pRenderer->CreateVertexShader(pResources->VGetShaderCode("Default.vs")->Get());
			auto PS = pRenderer->CreatePixelShader(pResources->VGetShaderCode("Default.fs")->Get());
			m_Pipeline = std::unique_ptr<Pipeline>(pRenderer->CreatePipeline(VS, PS));
			//m_ShaderName = std::make_pair(pVertexNode->GetText(), pPixelNode->GetText());
			pContext->GetSystem<IEventManager>()->VAddListener(DEBUG_NEW EventDelegate<DefaultMaterial>(this, &DefaultMaterial::OnSceneCreate), events::EvtSceneCreate::StaticType);
			this->GetUniform();
		}

		DefaultMaterial::DefaultMaterial()
		{

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
		void DefaultMaterial::Apply(RenderDevice* renderer, const MatrixParam& matrixParam, const MaterialData& matData)
		{
			const float* model = matrixParam.at(render::uMODEL);
			const float* mvp = matrixParam.at(render::uMVP);
			const float* cameraPos = matrixParam.at(uCameraPos);
			renderer->SetPipeline(m_Pipeline.get());

			m_uNumLight->SetAsInt(m_pLightManager->GetNumPointLight());
			m_pLightManager->SetupDirLight(&m_uDirLight);
			m_pLightManager->SetupPointLight(m_uPointLight);


			for (auto&el : m_TextureUnits)
				el.first->SetAsInt(el.second);

			m_uKa->SetAsVec3(glm::value_ptr(matData.Ka));
			m_uKd->SetAsVec3(glm::value_ptr(matData.Kd));
			m_uKs->SetAsVec3(glm::value_ptr(matData.Ks));
			m_uShiness->SetAsFloat(matData.exp.x);


			//renderer->SetTexture(UNIT_SKYBOX, renderer->GetSkyBoxTexture());
			m_uCameraPos->SetAsVec3(cameraPos);
			m_pModelUniform->SetAsMat4(model);
			m_pMVPUniform->SetAsMat4(mvp);
			
		}
		std::shared_ptr<Material> DefaultMaterial::Clone()
		{
			auto pTemp = new DefaultMaterial();

			auto pEventM = m_pContext->GetSystem<IEventManager>();
			pEventM->VAddListener(DEBUG_NEW EventDelegate<DefaultMaterial>(pTemp, &DefaultMaterial::OnSceneCreate), events::EvtSceneCreate::StaticType);
			pTemp->m_pContext = m_pContext;
			std::shared_ptr<Material> mat = std::shared_ptr<Material>(pTemp);
			return mat;
		}
	
		void DefaultMaterial::GetUniform()
		{
			assert(m_Pipeline != nullptr);
			assert(m_Pipeline != nullptr);
			m_pModelUniform = m_Pipeline->GetParam(uMODEL);
			m_pMVPUniform = m_Pipeline->GetParam(uMVP);

			this->AddTexUnit("mat.diffuse", UNIT_DIFFUSE);
			this->AddTexUnit("mat.ambient", UNIT_AMBIENT);
			this->AddTexUnit("mat.specular", UNIT_SPECULAR);
			this->AddTexUnit(uCubeTex, UNIT_SKYBOX);
			
			m_uCameraPos = m_Pipeline->GetParam(uCameraPos);

			m_uKa = m_Pipeline->GetParam("mat.Ka");
			m_uKd = m_Pipeline->GetParam("mat.Kd");
			m_uKs = m_Pipeline->GetParam("mat.Ks");
			m_uShiness = m_Pipeline->GetParam("mat.shininess");
			m_uNumLight = m_Pipeline->GetParam("uNumLight");

			m_uDirLight.AddParam("Ia", m_Pipeline->GetParam("dLight.Ia"));
			m_uDirLight.AddParam("Id", m_Pipeline->GetParam("dLight.Id"));
			m_uDirLight.AddParam("Is", m_Pipeline->GetParam("dLight.Is"));
			m_uDirLight.AddParam("Direction", m_Pipeline->GetParam("dLight.Direction"));
		}
		void DefaultMaterial::OnSceneCreate(std::shared_ptr<IEvent> event)
		{
			events::EvtSceneCreate* pEvent = static_cast<events::EvtSceneCreate*>(event.get());
			Scene* pScene = static_cast<Scene*>(pEvent->m_pScene);

			m_pLightManager = pScene->GetLightManager();
			
			int numLight = m_pLightManager->GetNumPointLight();
			for (int i = 0; i < numLight; i++)
			{
				LightParam param;
				string num = { char('0' + i)};
				
 				param.AddParam("Ia", m_Pipeline->GetParam(("pLights[" + num + "].Ia").c_str()));
				param.AddParam("Id", m_Pipeline->GetParam(("pLights[" + num + "].Id").c_str()));
				param.AddParam("Is", m_Pipeline->GetParam(("pLights[" + num + "].Is").c_str()));
				param.AddParam("Position", m_Pipeline->GetParam(("pLights[" + num + "].Pos").c_str()));
				param.AddParam("Constant", m_Pipeline->GetParam(("pLights[" + num + "].constant").c_str()));
				param.AddParam("Linear", m_Pipeline->GetParam(("pLights[" + num + "].linear").c_str()));
				param.AddParam("Quadratic", m_Pipeline->GetParam(("pLights[" + num + "].quadratic").c_str()));
				m_uPointLight.push_back(param);

			}

		}
	}
}


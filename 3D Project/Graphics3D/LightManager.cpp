#include "pch.h"
#include "LightManager.h"
#include "Material.h"
#include "..\Interface\IDebugRender.h"
bool Light::render::LightManager::VSerialize(IContext * pContext, const tinyxml2::XMLElement * pLightNode)
{
	IDebugRender* pDebuger = pContext->GetSystem<IDebugRender>();

	const tinyxml2::XMLElement* pDirLight = pLightNode->FirstChildElement("Direction");
	if (pDirLight)
	{
		m_DirLight.direction.x = pDirLight->DoubleAttribute("x");
		m_DirLight.direction.y = pDirLight->DoubleAttribute("y");
		m_DirLight.direction.z = pDirLight->DoubleAttribute("z");
		m_DirLight.direction = glm::normalize(m_DirLight.direction);

		const tinyxml2::XMLElement* pAmbient = pDirLight->FirstChildElement("Ambient");
		m_DirLight.Ia.x = pAmbient->DoubleAttribute("r", 1.0f);
		m_DirLight.Ia.y = pAmbient->DoubleAttribute("g", 1.0f);
		m_DirLight.Ia.z = pAmbient->DoubleAttribute("b", 1.0f);

		const tinyxml2::XMLElement* pDiffuse = pDirLight->FirstChildElement("Diffuse");
		m_DirLight.Id.x = pDiffuse->DoubleAttribute("r", 1.0f);
		m_DirLight.Id.y = pDiffuse->DoubleAttribute("g", 1.0f);
		m_DirLight.Id.z = pDiffuse->DoubleAttribute("b", 1.0f);

		const tinyxml2::XMLElement* pSpecular = pDirLight->FirstChildElement("Specular");
		m_DirLight.Is.x = pSpecular->DoubleAttribute("r", 1.0f);
		m_DirLight.Is.y = pSpecular->DoubleAttribute("g", 1.0f);
		m_DirLight.Is.z = pSpecular->DoubleAttribute("b", 1.0f);
	}

	for (const tinyxml2::XMLElement* pNode = pLightNode->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		if (!strcmp(pNode->Value(), "Point"))
		{
			PointLight light;
			light.Pos.x = pNode->DoubleAttribute("x");
			light.Pos.y = pNode->DoubleAttribute("y");
			light.Pos.z = pNode->DoubleAttribute("z");

			const tinyxml2::XMLElement* pAmbient = pNode->FirstChildElement("Ambient");
			light.Ia.x = pAmbient->DoubleAttribute("r", 1.0f);
			light.Ia.y = pAmbient->DoubleAttribute("g", 1.0f);
			light.Ia.z = pAmbient->DoubleAttribute("b", 1.0f);

			const tinyxml2::XMLElement* pDiffuse = pNode->FirstChildElement("Diffuse");
			light.Id.x = pDiffuse->DoubleAttribute("r", 1.0f);
			light.Id.y = pDiffuse->DoubleAttribute("g", 1.0f);
			light.Id.z = pDiffuse->DoubleAttribute("b", 1.0f);

			const tinyxml2::XMLElement* pSpecular = pNode->FirstChildElement("Specular");
			light.Is.x = pSpecular->DoubleAttribute("r", 1.0f);
			light.Is.y = pSpecular->DoubleAttribute("g", 1.0f);
			light.Is.z = pSpecular->DoubleAttribute("b", 1.0f);

			const tinyxml2::XMLElement* pAttenuation = pNode->FirstChildElement("Attenuation");
			light.constant = pAttenuation->DoubleAttribute("Const", 1.0f);
			light.linear = pAttenuation->DoubleAttribute("Linear", 0.09f);
			light.quadratic = pAttenuation->DoubleAttribute("Quadratic", 0.032f);

			m_PointLights.push_back(light);

			pDebuger->AddLightBox(light.Pos, light.Id);

		}

	}
	return false;
}

tinyxml2::XMLElement * Light::render::LightManager::VDeserialize(tinyxml2::XMLDocument * p)
{
	return nullptr;
}

void Light::render::LightManager::SetupDirLight(LightParam *pLightParam)
{
	(*pLightParam)["Ia"]->SetAsVec3(glm::value_ptr(m_DirLight.Ia));
	(*pLightParam)["Id"]->SetAsVec3(glm::value_ptr(m_DirLight.Id));
	(*pLightParam)["Is"]->SetAsVec3(glm::value_ptr(m_DirLight.Is));
	(*pLightParam)["Direction"]->SetAsVec3(glm::value_ptr(m_DirLight.direction));
}

void Light::render::LightManager::SetupPointLight(std::vector<LightParam>&lightParams)
{
	for (std::size_t i = 0; i < lightParams.size(); i++)
	{
		lightParams[i]["Ia"]->SetAsVec3(glm::value_ptr(m_PointLights[i].Ia));
		lightParams[i]["Id"]->SetAsVec3(glm::value_ptr(m_PointLights[i].Id));
		lightParams[i]["Is"]->SetAsVec3(glm::value_ptr(m_PointLights[i].Is));
		lightParams[i]["Position"]->SetAsVec3(glm::value_ptr(m_PointLights[i].Pos));
		lightParams[i]["Constant"]->SetAsFloat(m_PointLights[i].constant);
		lightParams[i]["Linear"]->SetAsFloat(m_PointLights[i].linear);
		lightParams[i]["Quadratic"]->SetAsFloat(m_PointLights[i].quadratic);
	}
		
}

int Light::render::LightManager::GetNumPointLight()
{
	return m_PointLights.size();
}

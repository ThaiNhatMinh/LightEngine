#include "stdafx.h"
#include "GS_Loading.h"
#include <IFactory.h>
#include "GS_Game.h"
#include "StateStack.h"
#include <tinyxml2/tinyxml2.h>
#include <sstream>
GS_Loading::GS_Loading(StateStack * pOwner):m_bLoadFinish(0), pOwner(pOwner)
{
	using namespace Light;
	m_pContext = pOwner->GetContext();
	m_pVGUI = m_pContext->GetSystem<vgui::IVGUI>();
}

GS_Loading::~GS_Loading()
{
	//pOwner->Push(new GS_Game(pOwner, m_pScene));
}


void GS_Loading::OnEnter(StateStack * pOwner)
{
	m_LoadThread = std::thread(&GS_Loading::LoadingFunc, this,m_pContext->GetSystem<Light::resources::IResourceManager>(),pOwner);
}

void GS_Loading::OnExit(StateStack * pOwner)
{
	//m_pContext->VReleaseContext();
}

bool GS_Loading::Update(float dt)
{
	std::stringstream ss;
	ss << "Loading: ";
	ss << status;
	ss << "%";
	m_pVGUI->VDrawText(ss.str(), glm::vec2(100, 100));
	if (m_bLoadFinish)
	{
		pOwner->Push(new GS_Game(pOwner));
	}
	return false;
}

bool GS_Loading::OnInputEvent(Light::events::EvtInput * pEvent)
{
	return false;
}

void GS_Loading::LoadingFunc(Light::resources::IResourceManager* resources, StateStack * pOwner)
{	
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile("GameAssets\\test\\resource.xml") == tinyxml2::XML_SUCCESS)
	{
		
		auto pReNode = doc.FirstChildElement("Resources");

		int totalResource = 0;
		for (tinyxml2::XMLElement* pNode = pReNode->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement()) totalResource++;
		int sumResource = 0;

		for (auto pNode = pReNode->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
		{
			auto file = pNode->GetText();
			const char* name = pNode->Value();


			if (!strcmp(name, "Texture"))
			{
				std::vector<std::string> filelist;
				filelist.push_back(file);
				resources->VGetTexture(filelist, false, true);
			}
			else if (!strcmp(name, "Shader")) resources->VGetShaderCode(file, true);
			else if (!strcmp(name, "CubeTexture"))
			{

				std::vector<std::string> filelist;

				for (tinyxml2::XMLElement* pNodeTex = pNode->FirstChildElement(); pNodeTex; pNodeTex = pNodeTex->NextSiblingElement())
				{
					const char* pFile = pNodeTex->Attribute("File");
					filelist.push_back(pFile);
				}

				resources->VGetTexture(filelist, true, true);
			}
			else if (!strcmp(name, "Model")) resources->VGetModel(file, true);
			else if (!strcmp(name, "HeightMap")) resources->VGetHeightMap(file, true);
			else if (!strcmp(name, "Sprite")) resources->VGetSprite(file, true);
			/*else if (!strcmp(name, "Sound"))
			{

				const char* pFile = pNode->Attribute("File");
				const char* pTag = pNode->Attribute("Tag");
				unsigned int mode = pNode->Int64Attribute("Sound3D", 0);
				SoundRAAI* pSound = LoadSound(pFile, pTag, mode ? FMOD_3D : FMOD_2D);


				if (pSound)
				{
					if (mode)
					{
						float InnerRadius = pNode->FloatAttribute("InnerRadius", 1.0f);
						float OuterRadius = pNode->FloatAttribute("OuterRadius", 512.0f);
						pSound->GetSound()->set3DMinMaxDistance(InnerRadius, OuterRadius);
						pSound->GetSound()->setMode(FMOD_3D_LINEARROLLOFF);
					}

					m_SoundList.insert({ pTag,std::unique_ptr<SoundRAAI>(pSound) });
				}
			}*/
			sumResource++;
			this->SetStatus(100* float(sumResource) / totalResource);
		}
	}
	m_Loading.lock();
	m_bLoadFinish = true;
	m_Loading.unlock();
	m_LoadThread.detach();
}

void GS_Loading::SetStatus(int s)
{
	m_Status.lock();
	status = s;
	m_Status.unlock();
}

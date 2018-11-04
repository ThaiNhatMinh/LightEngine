#include "stdafx.h"
#include "GS_Loading.h"
#include <IFactory.h>
#include "GS_Game.h"
#include "StateStack.h"
#include <tinyxml2/tinyxml2.h>

GS_Loading::GS_Loading(StateStack * pOwner):m_bLoadFinish(0), pOwner(pOwner)
{
	m_pContext = pOwner->GetContext();
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
	if (m_bLoadFinish)
	{
		//m_pContext->VReleaseContext();
		auto resources = m_pContext->GetSystem<Light::resources::IResourceManager>();
		tinyxml2::XMLDocument doc;
		if (doc.LoadFile("GameAssets\\test\\resource.xml") == tinyxml2::XML_SUCCESS)
		{
			auto pReNode = doc.FirstChildElement("Resources");
			for (auto pNode = pReNode->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
			{
				auto file = pNode->GetText();
				const char* name = pNode->Value();
				if (!strcmp(name, "Texture")) resources->VGetTexture(file, true);
				else if (!strcmp(name, "VertexShader")) resources->VGetVertexShader(file, true);
				else if (!strcmp(name, "PixelShader")) resources->VGetPixelShader(file, true);
				else if (!strcmp(name, "CubeTexture"))
				{

					std::vector<std::string> filelist;

					for (tinyxml2::XMLElement* pNodeTex = pNode->FirstChildElement(); pNodeTex; pNodeTex = pNodeTex->NextSiblingElement())
					{
						const char* pFile = pNodeTex->Attribute("File");
						filelist.push_back(pFile);
					}

					resources->VGetCubeTex(filelist, true);
				}
				else if (!strcmp(name, "Model")) resources->VGetModel(file, true);
				else if (!strcmp(name, "RawModel")) resources->VGetRawModel(file, true);
				else if (!strcmp(name, "HeightMap")) resources->VGetHeightMap(file, true);
				//else if (!strcmp(name, "Sprite")) resources->VGet
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

			}
		}
		pOwner->Push(new GS_Game(pOwner));
	}
	return false;
}

void GS_Loading::LoadingFunc(Light::resources::IResourceManager* resources, StateStack * pOwner)
{
	m_pContext->VMakeContext();
	
	
	
	m_Loading.lock();
	m_bLoadFinish = true;
	m_Loading.unlock();
	m_LoadThread.detach();
}

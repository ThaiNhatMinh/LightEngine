#include "stdafx.h"
#include "Game.h"

IGamePlugin * CreateInterface()
{
	return new Game();
}


void Game::Init(Context *c)
{
	m_Context = c;
	Actor::m_Context = c;
	ActorComponent::m_Context = c;
	ISubSystem::m_Context = c;
	c->AddSystem(this);

	m_Context->GetSystem<EventManager>()->VAddListener(MakeDelegate(this, &Game::EventTakeDamage), EvtTakeDamage::sk_EventType);
	m_Context->GetSystem<EventManager>()->VAddListener(MakeDelegate(this, &Game::EventCreateActor), EvtData_New_Actor::sk_EventType);
	m_Context->GetSystem<EventManager>()->VAddListener(MakeDelegate(this, &Game::EventExplosion), EvtExplosion::sk_EventType);


	m_Scene = std::unique_ptr<Scene>(new Scene(c));

	auto cameraFunc = [a = m_Scene.get()]() 
	{ 
		auto cam = new CameraComponent();
		a->SetCurrentCamera(cam);
		return  cam; 
	};

	c->GetSystem<ActorFactory>()->RegisterComponentFactory(LocalPlayerComponent::Name, []() {return new LocalPlayerComponent(); });
	c->GetSystem<ActorFactory>()->RegisterComponentFactory(CameraComponent::Name, cameraFunc);
	c->GetSystem<ActorFactory>()->RegisterComponentFactory(ZombieController::Name, []() {return new ZombieController(); });
	c->GetSystem<ActorFactory>()->RegisterActorFactory("Player", [](int id) {return new Player(id); });
	c->GetSystem<ActorFactory>()->RegisterActorFactory("PlayerView", [](int id) {return new PlayerView(id); });
	c->GetSystem<ActorFactory>()->RegisterActorFactory("Weapon", [](int id) {return new Weapon(id); });
	c->GetSystem<ActorFactory>()->RegisterActorFactory("GunPlayerView", [](int id) {return new GunPlayerView(id); });
	c->GetSystem<ActorFactory>()->RegisterActorFactory("AIExplosive", [](int id) {return new AIExplosive(id); });

	//LoadWeapon();
	//LoadCharacter();

	
	m_Scene->LoadScene("GameAssets\\ACTOR\\Scene.xml");
	
	

	auto root = m_Context->GetSystem<VGUI>()->GetRoot();

	

}

void Game::Update(float dt)
{
	m_Scene->OnUpdate(dt);

	m_Scene->PostUpdate();

	
	
}

void Game::Render()
{
	m_Scene->OnRender();
}

void Game::ShutDown()
{
	m_Context->GetSystem<EventManager>()->VRemoveListener(MakeDelegate(this, &Game::EventTakeDamage), EvtTakeDamage::sk_EventType);
	m_Context->GetSystem<EventManager>()->VRemoveListener(MakeDelegate(this, &Game::EventCreateActor), EvtData_New_Actor::sk_EventType);
	m_Context->GetSystem<EventManager>()->VRemoveListener(MakeDelegate(this, &Game::EventExplosion), EvtExplosion::sk_EventType);
}

Scene * Game::GetScene() {
	return m_Scene.get();
}

const std::vector<Player*>& Game::GetPlayerList()
{
	return m_PlayerLists;
}

CharacterResource Game::LoadCharacter(const std::string & file)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(file.c_str());
	tinyxml2::XMLElement* pNode = doc.FirstChildElement();

	CharacterResource cr;
	cr.Name = pNode->Value();
	tinyxml2::XMLElement* pBL = pNode->FirstChildElement("BL");
	cr.ModelFile[TEAM_BL] = pBL->FirstChildElement("Model")->Attribute("File");
	for (const tinyxml2::XMLAttribute* pSkin = pBL->FirstChildElement("Skin")->FirstAttribute(); pSkin; pSkin = pSkin->Next())
		cr.TexFile[TEAM_BL].insert({ pSkin->Name(),pSkin->Value() });

	tinyxml2::XMLElement* pPVTexture = pBL->FirstChildElement("PVTexture");
	for (tinyxml2::XMLElement* pTemp = pPVTexture->FirstChildElement(); pTemp; pTemp = pTemp->NextSiblingElement())
	{
		//cout << pTemp->Name() << "[ ]" << pTemp->Attribute("File") << endl;
		cr.PVTexFile[TEAM_BL].insert({ pTemp->Name(),pTemp->Attribute("File") });
	}
	pBL = pNode->FirstChildElement("GR");
	cr.ModelFile[TEAM_GR] = pBL->FirstChildElement("Model")->Attribute("File");
	for (const tinyxml2::XMLAttribute* pSkin = pBL->FirstChildElement("Skin")->FirstAttribute(); pSkin; pSkin = pSkin->Next())
		cr.TexFile[TEAM_GR].insert({ pSkin->Name(),pSkin->Value() });
	
	pPVTexture = pBL->FirstChildElement("PVTexture");
	for (tinyxml2::XMLElement* pTemp = pPVTexture->FirstChildElement(); pTemp; pTemp = pTemp->NextSiblingElement())
		cr.PVTexFile[TEAM_GR].insert({ pTemp->Name(),pTemp->Attribute("File") });

	cr.AnimFile = pNode->FirstChildElement("Animation")->Attribute("File");

	return cr;
}
WeaponResource Game::LoadWeaponInfo(const std::string & wpName)
{
	WeaponResource wr;

	std::string file = "GameAssets\\XML\\Weapon.xml";

	static tinyxml2::XMLDocument doc;
	doc.LoadFile(file.c_str());
	tinyxml2::XMLElement* pWeapon = doc.FirstChildElement("Weapon");

	// Loop through each child element and load the component
	int i = 0;
	for (tinyxml2::XMLElement* pNode = pWeapon->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		if (wpName != pNode->Value())
		{
			i++;
			continue;
		}

		wr.index = i++;
		wr.Name = pNode->Value();
		wr.Class = pNode->FirstChildElement("Class")->DoubleAttribute("Class");
		wr.ModelFile = pNode->FirstChildElement("ModelFileName")->Attribute("File");
		wr.ModelTex = pNode->FirstChildElement("SkinFileName")->Attribute("File");
		wr.PVModelFile = pNode->FirstChildElement("PViewModelFileName")->Attribute("File");
		wr.PVTexFile = pNode->FirstChildElement("PViewSkinFileName")->Attribute("File");
		wr.GViewAnimName = pNode->FirstChildElement("GViewAnimName")->Int64Attribute("Index");

		wr.Range = pNode->FirstChildElement("Info")->DoubleAttribute("Range");
		wr.MaxAmmo = pNode->FirstChildElement("Info")->DoubleAttribute("MaxAmmo");
		wr.AmmoPerMagazine = pNode->FirstChildElement("Info")->DoubleAttribute("AmmoPerMagazine");
		wr.AmmoDamage = pNode->FirstChildElement("Info")->DoubleAttribute("AmmoDamage");
		wr.TargetSlot = pNode->FirstChildElement("Info")->DoubleAttribute("TargetSlot");
		tinyxml2::XMLElement* pSound = pNode->FirstChildElement("Sound");
		if (pSound)
		{
			wr.ShotSoundName = pSound->Attribute("ShotSoundName");
			wr.MagazineClipInSoundName = pSound->Attribute("MagazineClipInSoundName");
			wr.MagazineClipOutSoundName = pSound->Attribute("MagazineClipOutSoundName");
			wr.ReloadSoundName = pSound->Attribute("ReloadSoundName");
		}

		return wr;
		
	}

}


void Game::EventTakeDamage(std::shared_ptr<IEvent> pEvent)
{
	EvtTakeDamage* p = static_cast<EvtTakeDamage*>(pEvent.get());

}

void Game::EventExplosion(std::shared_ptr<IEvent> pEvent)
{
	EvtExplosion* p = static_cast<EvtExplosion*>(pEvent.get());

	for (auto& el : m_PlayerLists)
	{
		float range = glm::distance(el->GetPosition(), p->GetPos());
		if (range < p->GetRange1()) el->TakeDamage(p->GetDamage());
		else if (range < p->GetRange2())
		{
			float percent = (p->GetRange2() - p->GetRange1()) / range;
			el->TakeDamage(p->GetDamage()*percent);
		}
		
		//cout << el->GetHP() << endl;
	}
}

void Game::EventCreateActor(std::shared_ptr<IEvent> pEvent)
{
	Actor* pActor = static_cast<EvtData_New_Actor*>(pEvent.get())->GetActorId();

	if (typeid(*pActor) == typeid(Player)) m_PlayerLists.push_back(static_cast<Player*>(pActor));
	//cout << pActor->VGetName() << endl;
	//cout << m_PlayerLists.size() << endl;
}
/*
void Game::LoadWeapon()
{
	std::string file = "GameAssets\\XML\\Weapon.xml";

	tinyxml2::XMLDocument doc;
	doc.LoadFile(file.c_str());
	tinyxml2::XMLElement* pWeapon = doc.FirstChildElement("Weapon");
	
	// Loop through each child element and load the component
	int i = 0;
	for (tinyxml2::XMLElement* pNode = pWeapon->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		WeaponResource wr;
		wr.index = i++;
		wr.Name = pNode->Value();
		wr.Class = pNode->FirstChildElement("Class")->DoubleAttribute("Class");
		wr.ModelFile = pNode->FirstChildElement("ModelFileName")->Attribute("File");
		wr.ModelTex = pNode->FirstChildElement("SkinFileName")->Attribute("File");
		wr.PVModelFile = pNode->FirstChildElement("PViewModelFileName")->Attribute("File");
		wr.PVTexFile = pNode->FirstChildElement("PViewSkinFileName")->Attribute("File");
		wr.AnimName = pNode->FirstChildElement("GViewAnimName")->Attribute("File");

		wr.Range = pNode->FirstChildElement("Info")->DoubleAttribute("Range");
		wr.MaxAmmo = pNode->FirstChildElement("Info")->DoubleAttribute("MaxAmmo");
		wr.AmmoPerMagazine = pNode->FirstChildElement("Info")->DoubleAttribute("AmmoPerMagazine");
		wr.AmmoDamage = pNode->FirstChildElement("Info")->DoubleAttribute("AmmoDamage");
		wr.TargetSlot = pNode->FirstChildElement("Info")->DoubleAttribute("TargetSlot");
		
		m_WeaponResources.push_back(wr);
	}

}

void Game::LoadCharacter()
{
	std::string file = "GameAssets\\XML\\Character.xml";

	tinyxml2::XMLDocument doc;
	doc.LoadFile(file.c_str());
	tinyxml2::XMLElement* pCharacter = doc.FirstChildElement("Character");
	for (tinyxml2::XMLElement* pNode = pCharacter->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		CharacterResource cr;
		cr.Name = pNode->Value();
		tinyxml2::XMLElement* pBL = pNode->FirstChildElement("BL");
		cr.ModelFile[TEAM_BL] = pBL->FirstChildElement("Model")->Attribute("File");
		for (const tinyxml2::XMLAttribute* pSkin = pBL->FirstChildElement("Skin")->FirstAttribute(); pSkin; pSkin = pSkin->Next())
			cr.TexFile[TEAM_BL].insert({ pSkin->Name(),pSkin->Value() });
		cr.ArmTex[TEAM_BL] = pBL->FirstChildElement("Arm")->Attribute("File");
		cr.HandTex[TEAM_BL] = pBL->FirstChildElement("Hand")->Attribute("File");

		pBL = pNode->FirstChildElement("GR");
		cr.ModelFile[TEAM_GR] = pBL->FirstChildElement("Model")->Attribute("File");
		for (const tinyxml2::XMLAttribute* pSkin = pBL->FirstChildElement("Skin")->FirstAttribute(); pSkin; pSkin = pSkin->Next())
			cr.TexFile[TEAM_GR].insert({ pSkin->Name(),pSkin->Value() });
		cr.ArmTex[TEAM_GR] = pBL->FirstChildElement("Arm")->Attribute("File");
		cr.HandTex[TEAM_GR] = pBL->FirstChildElement("Hand")->Attribute("File");

		cr.AnimFile = pNode->FirstChildElement("Animation")->Attribute("File");
		
		
		m_CharacterResources.push_back(cr);
	}


}
*/

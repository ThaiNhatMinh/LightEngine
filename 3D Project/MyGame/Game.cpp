#include <pch.h>
#include "GameHeader.h"
#include "Game.h"

void Game::Init(Context *c)
{
	m_Context = c;
	c->GetSystem<ActorFactory>()->RegisterComponentFactory(LocalPlayerComponent::Name, []() {return new LocalPlayerComponent(); });
	c->GetSystem<ActorFactory>()->RegisterActorFactory("Player", [](int id) {return new Player(id); });
	c->GetSystem<ActorFactory>()->RegisterActorFactory("PlayerView", [](int id) {return new PlayerView(id); });
	c->GetSystem<ActorFactory>()->RegisterActorFactory("Weapon", [](int id) {return new Weapon(id); });
	c->GetSystem<ActorFactory>()->RegisterActorFactory("GunPlayerView", [](int id) {return new GunPlayerView(id); });
	c->GetSystem<ActorFactory>()->RegisterActorFactory("Zombie", [](int id) {return new Zombie(id); });
	c->GetSystem<ActorFactory>()->RegisterActorFactory("AIExplosive", [](int id) {return new AIExplosive(id); });

	//LoadWeapon();
	//LoadCharacter();

	m_Scene = std::unique_ptr<Scene>(new Scene(c));
	m_Scene->LoadScene("GameAssets\\ACTOR\\Scene.xml");
	
	m_Context->GetSystem<EventManager>()->VAddListener(MakeDelegate(this, &Game::EventTakeDamage), EvtTakeDamage::sk_EventType);

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

Scene * Game::GetScene() {
	return m_Scene.get();
}

CharacterResource Game::LoadCharacter(const string & file)
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
WeaponResource Game::LoadWeaponInfo(const string & wpName)
{
	WeaponResource wr;

	string file = "GameAssets\\XML\\Weapon.xml";

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

	//cout << p->GetAttacker()->VGetName() << " Attack " << p->GetVictim()->VGetName() << endl;
	p->GetVictim()->TakeDamage(p->GetDamage());
	//printf("Index: %d\n", raycast.body->Inside(raycast.position));
}
/*
void Game::LoadWeapon()
{
	string file = "GameAssets\\XML\\Weapon.xml";

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
	string file = "GameAssets\\XML\\Character.xml";

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

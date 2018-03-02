#pragma once

class WeaponResource
{
public:
	WeaponResource() :index(-1), Name("Unknow"), Class(-1) {}
	int index;
	std::string Name;
	int Class;
	std::string ModelFile;
	std::string ModelTex;
	std::string PVModelFile;
	std::string PVTexFile;
	std::string ShotSoundName;
	std::string MagazineClipOutSoundName;
	std::string MagazineClipInSoundName;
	std::string ReloadSoundName;
	int GViewAnimName;
	int Range;
	int MaxAmmo;
	int AmmoPerMagazine;
	int AmmoDamage;
	int TargetSlot;		// slot of weapon 0,1,2,3,4,...
};

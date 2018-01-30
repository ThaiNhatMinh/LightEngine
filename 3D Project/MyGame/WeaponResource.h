#pragma once

class WeaponResource
{
public:
	WeaponResource() :index(-1), Name("Unknow"), Class(-1) {}
	int index;
	string Name;
	int Class;
	string ModelFile;
	string ModelTex;
	string PVModelFile;
	string PVTexFile;
	string GViewAnimName;
	int Range;
	int MaxAmmo;
	int AmmoPerMagazine;
	int AmmoDamage;
	int TargetSlot;		// slot of weapon 0,1,2,3,4,...
};

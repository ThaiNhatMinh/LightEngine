#pragma once

class WeaponResource
{
public:
	int index;
	string Name;
	int Class;
	string ModelFile;
	string ModelTex;
	string PVModelFile;
	string PVTexFile;
	string AnimName;
	int Range;
	int MaxAmmo;
	int AmmoPerMagazine;
	int AmmoDamage;
	int TargetSlot;		// slot of weapon 1,2,3,4,5
};

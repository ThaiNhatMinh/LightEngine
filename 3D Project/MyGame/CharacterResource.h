#pragma once
enum TeamClass
{
	TEAM_BL,
	TEAM_GR,
	TEAM_NONE,
	TEAM_MAX
};
class CharacterResource
{
public:
	string Name;
	string ModelFile[2];
	map<string,string> TexFile[2];
	map<string, string> PVTexFile[2];
	string AnimFile;
};
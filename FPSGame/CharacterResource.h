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
	std::string Name;
	std::string ModelFile[2];
	std::map<std::string,std::string> TexFile[2];
	std::map<std::string, std::string> PVTexFile[2];
	std::string AnimFile;
};
#pragma once
#include <string>

class IResource
{
public:
	IResource(const std::string& path):m_ResourcePath(path){};
	virtual ~IResource() = default;

	const std::string& GetPath() {return m_ResourcePath;};
private:
	std::string m_ResourcePath;
};
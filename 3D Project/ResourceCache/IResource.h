#pragma once
#include <string>

class IResource
{
public:
	~IResource() = default;

	virtual const std::string& GetName() final;
	void SetName(const std::string& name);
private:
	std::string m_Name;
};
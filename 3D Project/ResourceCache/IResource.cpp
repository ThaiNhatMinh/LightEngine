#include "pch.h"
#include "IResource.h"

const std::string& IResource::GetName()
{
	return m_Name;
}

void IResource::SetName(const std::string & name)
{
	m_Name = name;
}

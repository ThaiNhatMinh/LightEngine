#pragma once


class IModelResource :public IResource
{
public:
	IModelResource(const std::string& path) :IResource(path) {}
	~IModelResource() {};
	virtual int		VGetNumMesh() = 0;
	virtual IMesh*	VGetMesh(int i) = 0;
};


#pragma once
#include <pch.h>
#include "..\ResourceCache\LTBFileLoader.h"

class SkeMesh;
struct SkeNode;
struct WeightBlend;
struct Animation;
struct LTBSocket;
struct LTBProp;
class Material;
class IMesh;

class ModelCache:public IModelResource
{
public:
	LTBProp								Prop;
	vector<std::unique_ptr<SkeMesh>>	pMeshs;
	vector<std::unique_ptr<SkeNode>>	pSkeNodes;
	vector<WeightBlend>					wb;
	vector<string>						ChildName;
	vector<std::unique_ptr<Animation>>	pAnims;
	vector<LTBSocket>					Sockets;

	virtual int VGetNumMesh()override {
		return pMeshs.size();
	};
	virtual IMesh* VGetMesh(int i)override;

	ModelCache(const std::string& path):IModelResource(path) {}
	~ModelCache() {}
};

class ObjModel : public IModelResource
{
public:
	std::vector<std::unique_ptr<Mesh>> Meshs;
	//std::vector<Material> Materials;

	virtual int VGetNumMesh()
	{
		return Meshs.size();
	};
	virtual IMesh* VGetMesh(int i);
};
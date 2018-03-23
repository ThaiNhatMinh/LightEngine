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

class IModelResource :public IResource
{
public:
	~IModelResource() {};
	virtual int GetNumMesh() = 0;
	virtual IMesh* GetMesh(int i)=0;
};
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

	virtual int GetNumMesh() {
		return pMeshs.size();
	};
	virtual IMesh* GetMesh(int i);;

	ModelCache() {}
	~ModelCache() {}
};

class ObjModel : public IModelResource
{
public:
	std::vector<std::unique_ptr<Mesh>> Meshs;
	//std::vector<Material> Materials;

	virtual int GetNumMesh()
	{
		return Meshs.size();
	};
	virtual IMesh* GetMesh(int i);;
};
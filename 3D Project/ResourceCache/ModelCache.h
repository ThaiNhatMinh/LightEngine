#pragma once
#include "pch.h"
#include "..\ResourceCache\LTBFileLoader.h"

class SkeMesh;
struct SkeNode;
struct WeightBlend;
struct Animation;
struct LTBSocket;
struct LTBProp;
class ModelCache
{
public:
	char				szName[128];
	LTBProp				Prop;
	Material			mat;
	vector<SkeMesh*>	pMeshs;
	vector<SkeNode*>	pSkeNodes;
	vector<WeightBlend>	wb;
	vector<string>		ChildName;
	vector<Animation*>	pAnims;
	vector<LTBSocket>	Sockets;
	ModelCache() {}
	~ModelCache()
	{
		for (size_t i = 0; i < pMeshs.size(); i++)
			delete pMeshs[i];
		for (size_t i = 0; i < pSkeNodes.size(); i++)
			delete pSkeNodes[i];
		for (size_t i = 0; i < pAnims.size(); i++)
			delete pAnims[i];
	}
};
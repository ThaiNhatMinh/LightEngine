#pragma once
#include "..\pch.h"
#include "..\ResourceCache\LTBFileLoader.h"

class SkeMesh;
struct SkeNode;
struct WeightBlend;
struct Animation;
struct LTBSocket;
struct LTBProp;
struct ModelCache
{
	char				szName[128];
	LTBProp*			pProp;
	vector<SkeMesh*>	pMeshs;
	vector<SkeNode*>	pSkeNodes;
	vector<WeightBlend>	wb;
	vector<string>		ChildName;
	vector<Animation*>	pAnims;
	vector<LTBSocket>	Sockets;
};
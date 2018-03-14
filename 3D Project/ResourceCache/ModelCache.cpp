#include "pch.h"

IMesh * ObjModel::GetMesh(int i)
{
	return Meshs[i].get();
}

IMesh * ModelCache::GetMesh(int i)
{
	return pMeshs[i].get();
}

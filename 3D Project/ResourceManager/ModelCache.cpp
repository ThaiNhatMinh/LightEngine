#include <pch.h>

IMesh * ObjModel::VGetMesh(int i)
{
	return Meshs[i].get();
}

IMesh * ModelCache::VGetMesh(int i)
{
	return pMeshs[i].get();
}

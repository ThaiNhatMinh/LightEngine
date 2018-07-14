#pragma once

#include "ModelCache.h"
class ObjFile
{
private:
	Mesh * Calculate(const std::vector<vec3>& v, const std::vector<vec3>& n, const std::vector<vec2>& uv, const std::vector<vec3>& index);
public:

	static ObjModel*				LoadObjFile(const std::string& filename);
	static std::vector<Material>	LoadMaterial(const std::string& filename);
};
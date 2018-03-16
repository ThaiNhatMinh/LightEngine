#include "pch.h"
#include "ObjFileLoader.h"

Mesh * ObjFile::Calculate(const std::vector<vec3>& v, const std::vector<vec3>& n, const std::vector<vec2>& uv, const std::vector<vec3>& index)
{
	std::vector<DefaultVertex> vertexlist;
	std::vector<unsigned int> indices;
	for (size_t i = 0; i < index.size(); i++)
	{
		DefaultVertex df;
		df.pos = v[index[i].x - 1];
		df.uv = uv[index[i].y - 1];
		df.normal = n[index[i].z - 1];
		vertexlist.push_back(df);
		indices.push_back(index[i].x);
	}

	Mesh* pMesh = new Mesh(vertexlist, indices);
	return pMesh;
}

ObjModel * ObjFile::LoadObjFile(const std::string& filename)
{
	FILE* pFile = fopen(filename.c_str(), "rt");
	if (pFile == nullptr)
	{
		E_ERROR("Can't open file: " + filename);
		return nullptr;
	}

	std::vector<vec3> vertexs,normals;
	std::vector<vec2> uvs;

	std::vector< vec3 > indices;
	
	ObjModel* pModel = new ObjModel;
	int numMesh = 0;
	std::vector<int> mark;

	while (1)
	{
		char lineHeader[128];
		int result = fscanf(pFile, "%s", lineHeader);
		if (result == EOF) break;  

		if (strcmp(lineHeader, "v") == 0)
		{
			vec3 v;
			fscanf(pFile, "%f %f %f\n", &v.x, &v.y, &v.z);
			vertexs.push_back(v);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			vec2 uv;
			fscanf(pFile, "%f %f\n", &uv.x, &uv.y);

		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			vec3 n;
			fscanf(pFile, "%f %f %f\n", &n.x, &n.y, &n.z);
			normals.push_back(n);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

			int matches = fscanf(pFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			indices.push_back(vec3(vertexIndex[0], uvIndex[0], normalIndex[0]));
			indices.push_back(vec3(vertexIndex[1], uvIndex[1], normalIndex[1]));
			indices.push_back(vec3(vertexIndex[2], uvIndex[2], normalIndex[2]));
		}
		else if (strcmp(lineHeader, "mtllib")==0)
		{
			char filemtl[128];
			fscanf(pFile, "%s", filemtl);
			//pModel->Materials = LoadMaterial(filemtl);
		}
		else if (strcmp(lineHeader, "usemtl") == 0)
		{
			numMesh += 1;

		}

	}
}

std::vector<Material> ObjFile::LoadMaterial(const std::string& filename)
{
	return std::vector<Material>();
}

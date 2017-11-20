#include "pch.h"
#include "..\include\IL\il.h"
#include "..\include\IL\ilu.h"
#include "LTBFileLoader.h"


Resources* gResources()
{
	return Resources::InstancePtr();
}
//vector<Texture*> Resources::m_Textures;
//vector<ModelCache*> Resources::m_ModelCaches;

Texture * Resources::HasTexture(const char * filename)
{
	for (size_t i = 0; i < m_Textures.size(); i++)
		if (!strcmp(m_Textures[i]->szName, filename))
			return m_Textures[i];
	
	return NULL;
}

ModelCache * Resources::HasModel(const char * filename)
{
	for (size_t i = 0; i < m_ModelCaches.size(); i++)
		if (!strcmp(m_ModelCaches[i]->szName, filename))
			return m_ModelCaches[i];

	return NULL;
}

void Resources::ReleaseModel(ModelCache * p)
{
	if (!p) return;
	delete p->pProp;
	for (size_t i = 0; i < p->pMeshs.size(); i++)
		delete p->pMeshs[i];
	for (size_t i = 0; i < p->pSkeNodes.size(); i++)
		delete p->pSkeNodes[i];
	for (size_t i = 0; i < p->pAnims.size(); i++)
		delete p->pAnims[i];

	delete p;
}

Resources::Resources()
{
}


Resources::~Resources()
{
	
}

void Resources::onStartUp()
{
	ilInit();
	ILenum Error;
	Error = ilGetError();

	if (Error != IL_NO_ERROR)
		Log::Message(Log::LOG_ERROR, "Can't init Devil Lib.");

	// Load default tex
	LoadTexture("GameAssets/TEXTURE/Default.png");
		
}

Texture * Resources::LoadTexture(const char * filename)
{
	Texture* tex=NULL;
	if ((tex = HasTexture(filename)) != NULL) return tex;

	GLint width, height, iType, iBpp;

	ilLoadImage(filename);
	ILenum Error;
	Error = ilGetError();

	if (Error != IL_NO_ERROR)
	{
		//string error = iluErrorString(Error);
		Log::Message(Log::LOG_ERROR, "Can't load texture " + string(filename));
		//Log::Message(Log::LOG_ERROR, "Devil: " + error);
		return HasTexture("GameAssets/TEXTURE/Default.png");
	}
	
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	iType = ilGetInteger(IL_IMAGE_FORMAT);
	ILubyte *Data = ilGetData();
	iBpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);

	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, iBpp, width, height, 0, iType, GL_UNSIGNED_BYTE, Data);
	glBindTexture(GL_TEXTURE_2D, 0);
	glGenerateMipmap(GL_TEXTURE_2D);

	ilResetMemory();
	
	tex = new Texture;
	sprintf(tex->szName, "%s", filename);
	tex->iIndex = id;
	tex->iWidth = width;
	tex->iHeight = height;

	m_Textures.push_back(tex);


	return tex;
}

Texture * Resources::LoadCubeTex(const vector<string>& filelist)
{
	Texture* tex = NULL;
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	GLint width, height, iType, iBpp;
	for (size_t i = 0; i < filelist.size(); i++)
	{
		ilLoadImage(filelist[i].c_str());
		ILenum Error;
		Error = ilGetError();

		if (Error != IL_NO_ERROR)
		{
			//string error = iluErrorString(Error);
			Log::Message(Log::LOG_ERROR, "Can't load texture " + filelist[i]);
			//Log::Message(Log::LOG_ERROR, "Devil: " + error);
			return HasTexture("GameAssets/TEXTURE/Default.png");
		}

		width = ilGetInteger(IL_IMAGE_WIDTH);
		height = ilGetInteger(IL_IMAGE_HEIGHT);
		iType = ilGetInteger(IL_IMAGE_FORMAT);
		ILubyte *Data = ilGetData();
		iBpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X +i, 0, iBpp, width, height, 0, iType, GL_UNSIGNED_BYTE, Data);
		
	}
	ilResetMemory();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	//glGenerateMipmap(GL_TEXTURE_2D);

	

	tex = new Texture;
	sprintf(tex->szName, "%s", filelist[0].c_str());
	tex->iIndex = id;
	tex->iWidth = width;
	tex->iHeight = height;

	m_Textures.push_back(tex);


	return tex;

}

Texture * Resources::LoadTexMemory(const char* filename,unsigned char * data, int w, int h)
{
	Texture* tex = NULL;
	if ((tex = HasTexture(filename)) != NULL) return tex;

	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	tex = new Texture;
	sprintf(tex->szName, "%s", filename);
	tex->iIndex = id;
	tex->iWidth = w;
	tex->iHeight = h;

	m_Textures.push_back(tex);


	return tex;

}

Texture * Resources::LoadDTX(const char * filename)
{

	Texture* tex = NULL;
	if ((tex = HasTexture(filename)) != NULL) return tex;

	FILE* pFile = fopen(filename, "rb");
	if (!pFile)
	{
		Log::Message(Log::LOG_ERROR, "Can't open file: " + string(filename));
		
		return tex;
	}
	DtxHeader Header;
	memset(&Header, 0, sizeof(DtxHeader));
	fread(&Header, sizeof(DtxHeader), 1, pFile);
	if (Header.iResType != 0 || Header.iVersion != -5 || Header.usMipmaps == 0)
	{
		fclose(pFile);
		return tex;
	}

	int W, H;

	W = Header.usWidth;
	H = Header.usHeight;
	int iBpp = Header.ubExtra[2];
	int iSize;
	int InternalFormat;
	if (iBpp == 3)
	{
		iSize = Header.usWidth * Header.usHeight * 4;
		InternalFormat = GL_RGBA;
	}
	else if (iBpp == 4)
	{
		iSize = (Header.usWidth * Header.usHeight) >> 1;
		InternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
	}
	else if (iBpp == 5)
	{
		iSize = Header.usWidth * Header.usHeight;
		InternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
	}
	else if (iBpp == 6)
	{
		iSize = Header.usWidth * Header.usHeight;
		InternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
	}
	else
	{
		iSize = 0;
	}

	int ImageSize = iSize;

	int iBufferSize = 1024 * 1024 * 4;
	if (iSize == 0 || iSize > iBufferSize)
	{
		fclose(pFile);
		return tex;
	}

	unsigned char* ubBuffer = new unsigned char[1024 * 1024 * 4];

	fread(ubBuffer, iSize, 1, pFile);

	if (iBpp == 3)
	{
		for (int i = 0; i < iSize; i += 4)
		{
			ubBuffer[i + 0] ^= ubBuffer[i + 2];
			ubBuffer[i + 2] ^= ubBuffer[i + 0];
			ubBuffer[i + 0] ^= ubBuffer[i + 2];
		}
	}

	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	

	if (InternalFormat == GL_RGBA)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, ubBuffer);
	}
	else if (InternalFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || InternalFormat == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || InternalFormat == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
	{
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, W, H, 0, iSize, ubBuffer);
	}


	glBindTexture(GL_TEXTURE_2D, 0);



	tex = new Texture;
	sprintf(tex->szName, "%s", filename);
	tex->iIndex = id;
	tex->iWidth = W;
	tex->iHeight = H;
	
	m_Textures.push_back(tex);

	fclose(pFile);
	delete[] ubBuffer;
	return tex;

}

byte * Resources::LoadHeightMap(const char * filename, int& w, int& h)
{
	
	ilLoadImage(filename);
	ILenum Error;
	Error = ilGetError();

	if (Error != IL_NO_ERROR)
	{
		//string error = iluErrorString(Error);
		Log::Message(Log::LOG_ERROR, "Can't load texture " + string(filename));
		//Log::Message(Log::LOG_ERROR, "Devil: " + error);
		return NULL;
	}

	w = ilGetInteger(IL_IMAGE_WIDTH);
	h = ilGetInteger(IL_IMAGE_HEIGHT);
	
	ILubyte *Data = ilGetData();
	
	return Data;

}

ModelCache * Resources::LoadModel(const char * filename)
{
	ModelCache* pModel = NULL;
	if ((pModel = HasModel(filename)) != NULL) return pModel;

	pModel = new ModelCache;
	if (LTBFile::BeginLoad(filename))
	{
		pModel->pProp = LTBFile::LoadProp();
		pModel->pMeshs = LTBFile::LoadMesh();
		pModel->pSkeNodes = LTBFile::LoadSkeleton();
		pModel->wb = LTBFile::LoadWS();
		pModel->ChildName = LTBFile::LoadChildName();
		pModel->pAnims = LTBFile::LoadAnimation(pModel->pSkeNodes);
		pModel->Sockets = LTBFile::LoadSocket();
		LTBFile::EndLoad();

	}
	else
	{
		Log::Message(Log::LOG_ERROR, "Can't open file: " + string(filename));
		return NULL;
	}

	strcpy(pModel->szName, filename);
	vector<AABB> abb(pModel->pSkeNodes.size());
	for (size_t i = 0; i < pModel->pMeshs.size(); i++)
	{
		SkeMesh* pMesh = pModel->pMeshs[i];
		for (size_t j = 0; j < pMesh->m_Vertexs.size(); j++)
		{
			SkeVertex& vertex = pMesh->m_Vertexs[j];
			for (int k = 0; k < 4; k++)
			{
				if (vertex.weights[k].Bone <100.0f && vertex.weights[k].weight>=0.01)
				{
					vec3 local = pModel->pSkeNodes[vertex.weights[k].Bone]->m_InvBindPose*vec4(vertex.pos, 1.0f);
					local *= vertex.weights[k].weight;
					abb[vertex.weights[k].Bone].Insert(local);
					pModel->pSkeNodes[vertex.weights[k].Bone]->m_Flag = 1;
				}
				else break;
			}
		}
	}

	for (size_t i = 0; i < pModel->pSkeNodes.size(); i++)
	{
		
		pModel->pSkeNodes[i]->m_BoundBox = abb[i];
	}

	m_ModelCaches.push_back(pModel);
	return pModel;
}

ModelCache * Resources::LoadModelXML(const char * XMLFile)
{
	tinyxml2::XMLDocument doc;
	int errorID = doc.LoadFile(XMLFile);
	if (errorID)
	{
		E_ERROR("Failed to load file: " + string(XMLFile));
		return nullptr;
	}
	tinyxml2::XMLElement* pData = doc.FirstChildElement("Data");

	// load model
	tinyxml2::XMLElement* pModelNode = pData->FirstChildElement("Model");
	const char* pFileName = pModelNode->Attribute("File");

	ModelCache* pModel = LoadModel(pFileName);
	if (pModel)
	{
		// load texture
		tinyxml2::XMLElement* pTextureNode = pData->FirstChildElement("Texture");
		vector<SkeMesh*>& ve = pModel->pMeshs;
		for (size_t i = 0; i < ve.size(); i++)
		{
			tinyxml2::XMLElement* pTexture = pTextureNode->FirstChildElement(ve[i]->Name.c_str());
			const char* pTextureFile = pTexture->Attribute("File");
			ve[i]->Tex = LoadDTX(pTextureFile);
		}
	}		

	

	// load material
	tinyxml2::XMLElement* pMaterialData = pData->FirstChildElement("Material");
	Material mat;
	tinyxml2::XMLElement* pKa = pMaterialData->FirstChildElement("Ka");
	mat.Ka.x = pKa->FloatAttribute("r", 1.0f);
	mat.Ka.y = pKa->FloatAttribute("g", 1.0f);
	mat.Ka.z = pKa->FloatAttribute("b", 1.0f);
	tinyxml2::XMLElement* pKd = pMaterialData->FirstChildElement("Kd");
	mat.Kd.x = pKd->FloatAttribute("r", 1.0f);
	mat.Kd.y = pKd->FloatAttribute("g", 1.0f);
	mat.Kd.z = pKd->FloatAttribute("b", 1.0f);
	tinyxml2::XMLElement* pKs = pMaterialData->FirstChildElement("Ks");
	mat.Ks.x = pKs->FloatAttribute("r", 1.0f);
	mat.Ks.y = pKs->FloatAttribute("g", 1.0f);
	mat.Ks.z = pKs->FloatAttribute("b", 1.0f);
	mat.exp = pKs->FloatAttribute("exp", 1.0f);

	pModel->mat  = mat;
	// Get mesh list and update Material 
	//for (size_t i = 0; i < pModel->pMeshs.size(); i++)
	//{
	//	pModel->pMeshs[i]->mat = mat;
	//}

	// Done return ModelCache

//	delete doc;
	return pModel;
}



Shader * Resources::GetShader(string key)
{
	return m_ShaderList[key];
}

IMesh * Resources::CreateShape(ShapeType type)
{

	IMesh* pBox = new CubeMesh();
	pBox->Name = ShapeName[type];
	m_PrimList.push_back(pBox);

	return pBox;
}

void Resources::onShutDown()
{
	for (size_t i = 0; i < m_Textures.size(); i++)
		delete m_Textures[i];

	for (size_t i = 0; i < m_ModelCaches.size(); i++)
		ReleaseModel(m_ModelCaches[i]);

	map<string, Shader*>::iterator pos = m_ShaderList.begin();
	while (pos != m_ShaderList.end())
	{
		delete pos->second;
		//pos->second = NULL;
		pos++;
	}

	for (size_t i = 0; i < m_PrimList.size(); i++)
		delete m_PrimList[i];
}


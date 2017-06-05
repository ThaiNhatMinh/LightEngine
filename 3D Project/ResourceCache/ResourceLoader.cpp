#include "..\pch.h"
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
		return NULL;
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
			return NULL;
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
	m_ModelCaches.push_back(pModel);
	return pModel;
}


Shader * Resources::LoadShader(string key, const char * vs, const char* fs)
{
	map<string, Shader*>::iterator pos = m_ShaderList.find(key);
	if (pos != m_ShaderList.end()) return pos->second;

	Shader* p = new Shader(vs, fs);
	m_ShaderList.insert({ key, p });
	return p;
}

Shader * Resources::GetShader(string key)
{
	return m_ShaderList[key];
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

}


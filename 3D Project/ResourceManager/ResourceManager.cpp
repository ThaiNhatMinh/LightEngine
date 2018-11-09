#include <pch.h>
#include <DevIL/include/IL/il.h>
#include <DevIL/include/IL/ilu.h>
#include "LTBFileLoader.h"
#include <fmod_errors.h>
#include <fmod.hpp>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include "ResourceManager.h"

#include "..\Core\OpenGLWindows.h"
#include "..\Graphics3D\DefaultMesh.h"
#include "..\Graphics3D\LTModel.h"
#include "..\Core\SoundEngine.h"
#include "..\Core\RenderSystem.h"
namespace Light
{
	namespace resources
	{
		struct DtxHeader
		{
			unsigned int iResType;
			int iVersion;
			unsigned short usWidth;
			unsigned short usHeight;
			unsigned short usMipmaps;
			unsigned short usSections;
			int iFlags;
			int iUserFlags;
			unsigned char ubExtra[12];
			char szCommandString[128];
		};

		ResourceManager::ResourceManager(IContext* c) :m_pContext(c)
		{
			c->VAddSystem(this);
			
			ilInit();
			ILenum Error;
			Error = ilGetError();

			if (Error != IL_NO_ERROR)
				E_ERROR("Can't init Devil Lib.");

			
			CheckResourceFunc a = [](const std::string&a, const std::string& b)
			{
				return (a.find(b) != string::npos);
			};
			// Load default tex
			//m_pDefaultTex = HasResource(m_Textures, "White.png",a );

			//LoadResources("GameAssets/" + string(Light::RESOURCES_FILE));

			
			
			//m_FMOD = c->GetSystem<SoundEngine>()->GetFMODSystem();

		}


		ResourceManager::~ResourceManager()
		{
		
		}

		const char * ResourceManager::VGetName()
		{
			static const char * pName = typeid(IResourceManager).name();
			return pName;
		}



		SpriteData * ResourceManager::LoadSpriteAnimation(const string& filename)
		{
			

			SpriteData* s = nullptr;

			if (filename.size() == 0) return nullptr;

			

			FILE* pFile = fopen(filename.c_str(), "rb");
			if (!pFile)
			{
				E_ERROR("Can't load sprite: %s",filename.c_str());
				return nullptr;
			}

			uint32 nFrames, nFrameRate, bTransparent, bTranslucent, colourKey;
			char filetex[1024];
			uint16 strLen;
			fread(&nFrames, sizeof(uint32), 1, pFile);
			fread(&nFrameRate, sizeof(uint32), 1, pFile);
			fread(&bTransparent, sizeof(uint32), 1, pFile);
			fread(&bTranslucent, sizeof(uint32), 1, pFile);
			fread(&colourKey, sizeof(uint32), 1, pFile);

			s = DEBUG_NEW SpriteData;

			//s->m_FrameLists.resize(nFrames);
			s->m_MsFrameRate = nFrameRate;
			s->m_MsAnimLength = (1000 / nFrameRate) * nFrames;

			s->m_bKeyed = (uint8)bTransparent;
			s->m_bTranslucent = (uint8)bTranslucent;
			s->m_ColourKey = colourKey;

			for (size_t i = 0; i < nFrames; i++)
			{
				// Read in frame file name
				fread(&strLen, sizeof(strLen), 1, pFile);
				if (strLen > 1000)
				{
					delete s;
					return nullptr;
				}

				fread(filetex, strLen, 1, pFile);
				filetex[strLen] = 0;

				for (int j = 0; j < strLen; j++)
					if (filetex[j] > 'a' && filetex[j] < 'z')
						filetex[j] += 'A' - 'a';

				std::string fullfile = "TEXTURES\\";
				fullfile += filetex;
				s->m_FrameLists.push_back(fullfile);
				LoadTexture(fullfile);

			}

			
			return s;
		}

		HeightMapData* ResourceManager::LoadHeightMap(const string& filename)
		{
			HeightMapData* hm = nullptr;

			GLint width, height, iType, iBpp;

			
			ilLoadImage(filename.c_str());

			ILenum Error;
			Error = ilGetError();

			if (Error != IL_NO_ERROR)
			{
				E_ERROR("Can't load heightmap %",filename.c_str());
				return  nullptr;
			}
			width = ilGetInteger(IL_IMAGE_WIDTH);
			height = ilGetInteger(IL_IMAGE_HEIGHT);
			iType = ilGetInteger(IL_IMAGE_FORMAT);
			ILubyte *Data = ilGetData();
			iBpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);


			//vec2 size = vec2(width*stepsize, height*stepsize);

			GLubyte* pRawData = DEBUG_NEW GLubyte[width*height];
			int c = 0;
			float min = 100000000;
			float max = -min;
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					int b = i * width*iBpp + j * iBpp;

					c = i * width + j;
					pRawData[c] = (Data[b] + Data[b + 1] + Data[b + 2]) / 3.0;
					//pRawData[c] = 10;
					if (min > pRawData[c]) min = pRawData[c];
					if (max < pRawData[c]) max = pRawData[c];
					
				}
			}

			

			auto average = [width, height](unsigned char* pData, int i, int j)
			{
				float avg = 0.0f;
				float num = 0.0f;
				for (int m = i - 1; m <= i + 1; m++)
				{
					for (int n = j - 1; n <= j + 1; n++)
					{
						avg += pData[m*width + n];
						num++;
					
					}
				}
				
 				return avg / num;
				
			};

			GLubyte* pRawData2 = DEBUG_NEW GLubyte[width*height];
			for (int i = 1; i < height-1; i++)
				for (int j = 1; j < width-1; j++)
					pRawData2[i*width + j] = average(pRawData, i, j);
				
			delete[] pRawData;
			ilResetMemory();
			hm = DEBUG_NEW HeightMapData();
			hm->Data = std::unique_ptr<GLubyte[]>(pRawData2);
			hm->Width = width;
			hm->Height = height;
			hm->maxH = max;
			hm->minH = min;
		

			return hm;
		}

		TextureData * ResourceManager::LoadTexture(const string& filename)
		{
			TextureData* tex = nullptr;

			if (filename.find(".DTX") != string::npos) return LoadDTX(filename);

			GLint width, height, iType, iBpp;

			
			ilLoadImage(filename.c_str());
			ILenum Error;
			Error = ilGetError();

			if (Error != IL_NO_ERROR)
			{
				//string error = iluErrorString(Error);
				
				E_ERROR("Can't load texture: %s",filename.c_str());
				E_ERROR("[IL] %d",Error);
				//Log::Message(Log::LOG_ERROR, "Devil: " + error);
				return nullptr;//HasTexture("GameAssets/TEXTURE/Default.png");
			}

			width = ilGetInteger(IL_IMAGE_WIDTH);
			height = ilGetInteger(IL_IMAGE_HEIGHT);
			iType = ilGetInteger(IL_IMAGE_FORMAT);
			ILubyte *Data = ilGetData();
			iBpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);

			render::ColorFormat format;
			switch (iBpp)
			{
			case 1:
				format= render::FORMAT_RED;
				break;
			case 3:
				format= render::FORMAT_RGB;
				break;
			case 4:
				format = render::FORMAT_RGBA;
				break;
			default:
				format = render::FORMAT_RGB;
				break;
			}


			tex = DEBUG_NEW TextureData;
			tex->eTarget = render::TEXTURE_2D;
			tex->iLevel = 0;
			tex->iInternalFormat = format;
			tex->uiWidth = width;
			tex->uiHeight = height;
			tex->eType = render::UNSIGNED_BYTE;
			tex->eFormat = tex->iInternalFormat;
			tex->flag = Flag_Normal;
			auto pData = DEBUG_NEW unsigned char[width*height*iBpp];
			memcpy(pData, Data, width*height*iBpp);
			tex->pData = pData;
			ilResetMemory();
			return tex;
		}

		TextureData * ResourceManager::LoadCubeTex(const vector<string>& filelist)
		{
			TextureData* tex = nullptr;
			unsigned char** pData;
			pData = DEBUG_NEW unsigned char*[6];
			GLint width, height, iType, iBpp;
			for (size_t i = 0; i < filelist.size(); i++)
			{
				
				ilLoadImage(filelist[i].c_str());
				ILenum Error;
				Error = ilGetError();

				if (Error != IL_NO_ERROR)
				{
					E_ERROR("Can't load texture: %s",filelist[i].c_str());
					return nullptr;
				}

				width = ilGetInteger(IL_IMAGE_WIDTH);
				height = ilGetInteger(IL_IMAGE_HEIGHT);
				iType = ilGetInteger(IL_IMAGE_FORMAT);
				ILubyte *Data = ilGetData();
				iBpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);

				pData[i] = DEBUG_NEW unsigned char[width*height*iBpp];
				memcpy(pData[i], Data, width*height*iBpp);

			}


			

			tex = DEBUG_NEW TextureData;
			tex->eTarget = render::TEXTURE_CUBE_MAP;
			tex->iLevel = 0;
			tex->iInternalFormat = iBpp == 3 ? render::FORMAT_RGB : render::FORMAT_RGBA;
			tex->uiWidth = width;
			tex->uiHeight = height;
			tex->pData = pData;
			tex->eType = render::UNSIGNED_BYTE;
			tex->eFormat = tex->iInternalFormat;
			tex->flag = Flag_CubeTex;

			ilResetMemory();
			return tex;

		}

		TextureData * ResourceManager::LoadDTX(const string& filename)
		{

			TextureData* tex = nullptr;
						

			FILE* pFile = fopen(filename.c_str(), "rb");
			if (!pFile)
			{
				E_ERROR("Can't open file: %s",filename.c_str());

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
			render::ColorFormat InternalFormat;
			if (iBpp == 3)
			{
				iSize = Header.usWidth * Header.usHeight * 4;
				InternalFormat = render::ColorFormat::FORMAT_RGBA;
			}
			else if (iBpp == 4)
			{
				iSize = (Header.usWidth * Header.usHeight) >> 1;
				InternalFormat = render::ColorFormat::FORMAT_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			}
			else if (iBpp == 5)
			{
				iSize = Header.usWidth * Header.usHeight;
				InternalFormat = render::ColorFormat::FORMAT_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			}
			else if (iBpp == 6)
			{
				iSize = Header.usWidth * Header.usHeight;
				InternalFormat = render::ColorFormat::FORMAT_COMPRESSED_RGBA_S3TC_DXT5_EXT;
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

			unsigned char* ubBuffer = DEBUG_NEW unsigned char[1024 * 1024 * 4];

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

	

			if (InternalFormat == render::FORMAT_RGBA)
			{
				tex = DEBUG_NEW TextureData;
				tex->eTarget = render::TEXTURE_2D;
				tex->pData = ubBuffer;
				tex->eType = render::UNSIGNED_BYTE;
				tex->uiWidth = W;
				tex->uiHeight = H;
				tex->iInternalFormat = InternalFormat;
				tex->eFormat = render::FORMAT_RGBA;
				tex->flag = Flag_Normal;
			}
			else if (InternalFormat == render::FORMAT_COMPRESSED_RGBA_S3TC_DXT1_EXT || InternalFormat == render::FORMAT_COMPRESSED_RGBA_S3TC_DXT3_EXT || InternalFormat == render::FORMAT_COMPRESSED_RGBA_S3TC_DXT5_EXT)
			{
				tex = DEBUG_NEW TextureData;
				tex->pData = ubBuffer;
				tex->uiWidth = W;
				tex->uiHeight = H;
				tex->iInternalFormat = InternalFormat;
				tex->iLevel = iSize;
				tex->eTarget = render::TEXTURE_2D;
				tex->flag = Flag_Compress;
			}


			
			/*tex->iWidht = W;
			tex->iHeight = H;
			tex->iType = 0;
			tex->format = InternalFormat;
			tex->pData = ubBuffer;
			tex->iSize = iSize;
			if (InternalFormat == render::FORMAT_RGBA)
				tex->flag = Flag_Normal;
			else if (InternalFormat == render::FORMAT_COMPRESSED_RGBA_S3TC_DXT1_EXT || InternalFormat == render::FORMAT_COMPRESSED_RGBA_S3TC_DXT3_EXT || InternalFormat == render::FORMAT_COMPRESSED_RGBA_S3TC_DXT5_EXT)
				tex->flag = Flag_Compress;*/

			fclose(pFile);
			return tex;

		}


		ModelData * ResourceManager::LoadModel(const string& filename)
		{
			ModelData* pModel = nullptr;
			if (filename.find(".LTB") != std::string::npos) pModel = LoadLTBModel(filename);
			else pModel = LoadObjModel(filename);
			return pModel;
		}

		Sound * ResourceManager::LoadSound(const string & filename, int mode)
		{
			FModSound* pSound = nullptr;
			

			FMOD::Sound* pFMODSound = nullptr;
			FMOD::System* pSystem = static_cast<SoundEngine*>(m_pContext->GetSystem<ISoundEngine>())->GetFMODSystem();
			FMOD_RESULT result;

			string fullpath = filename;
			if ((result = pSystem->createSound(fullpath.c_str(), mode, 0, &pFMODSound)) != FMOD_OK)
			{
				E_ERROR("Can't create sound: %s" , fullpath.c_str());
				E_ERROR("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
				return nullptr;
			}
			pFMODSound->set3DMinMaxDistance(0.5f, 10000.0f);
			pSound = DEBUG_NEW FModSound(pFMODSound);

			return pSound;
		}

		
		ModelData * ResourceManager::LoadObjModel(const std::string filename)
		{
			
			string localPath;
			int pos = filename.find_last_of("/\\");
			localPath = filename.substr(0, pos) + '\\';
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
			
			if (!scene)
			{
				E_ERROR("Assimp: %s ",importer.GetErrorString());
				getchar();
				return false;
			}
			DefaultModelData* pModel = DEBUG_NEW DefaultModelData();
		
			std::vector<DefaultVertex> vertexs;
			std::vector<unsigned int> Indices;
			for (size_t i = 0; i < scene->mNumMeshes; i++)
			{
				vertexs.clear();
				Indices.clear();
		
				const aiMesh* mesh = scene->mMeshes[i];
				
				for (size_t j = 0; j < mesh->mNumVertices;j++)
				{
					aiVector3D pos = mesh->mVertices[j];
					
					aiVector3D UVW = mesh->mTextureCoords[0][j];
					aiVector3D n = mesh->mNormals[j];
					DefaultVertex dv;
					dv.pos = vec3(pos.x, pos.y, pos.z);
					dv.normal = vec3(n.x, n.y, n.z);
					dv.uv = vec2(UVW.x, UVW.y);
					vertexs.push_back(dv);

					pModel->Box.Test(dv.pos);
				}
				for (size_t j = 0; j < mesh->mNumFaces; j++)
				{
					const aiFace& Face = mesh->mFaces[j];
					if (Face.mNumIndices == 3) {
						Indices.push_back(Face.mIndices[0]);
						Indices.push_back(Face.mIndices[1]);
						Indices.push_back(Face.mIndices[2]);
		
					}
				}
				
				uint32 a = mesh->mMaterialIndex;
				aiMaterial* mat = scene->mMaterials[a];
				
				//auto m = m_pContext->GetSystem<IFactory>()->VGetMaterial("Default");
				render::MaterialData matParam;
				aiString name;
				mat->Get<aiString>(AI_MATKEY_NAME, name);
				matParam.Name = name.C_Str();
				aiColor3D color(0.f, 0.f, 0.f);
				mat->Get(AI_MATKEY_COLOR_AMBIENT,color);
				matParam.Ka = vec3(color.r, color.g, color.b);
				mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
				matParam.Kd = vec3(color.r, color.g, color.b);
				mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
				matParam.Ks = vec3(color.r, color.g, color.b);
				float exp;
				mat->Get(AI_MATKEY_SHININESS, exp);
				matParam.exp = vec3(exp);

				pModel->Mats.push_back(matParam);

				aiString Path;
				/*{
					mat->GetTexture(aiTextureType_AMBIENT, 0, &Path, NULL, NULL, NULL, NULL, NULL);
					pModel->Ambient.push_back(VGetTexture(localPath + Path.C_Str()));
				}*/
				{
					mat->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL);
					pModel->Texs.push_back(VGetTexture(std::vector<std::string>{localPath + Path.C_Str()},false,true));
				}
				/*{
					mat->GetTexture(aiTextureType_SPECULAR, 0, &Path, NULL, NULL, NULL, NULL, NULL);
					pModel->Specular.push_back(VGetTexture(localPath + Path.C_Str()));
				}	*/			
				//pModel->Materials.push_back(m);
				
				//DefaultMesh* pMesh = DEBUG_NEW DefaultMesh(m_pRenderDevice, vertexs, Indices, mesh->mName.C_Str());
				DefaultMeshData meshData;
				meshData.Vertexs = vertexs;
				meshData.Indices = Indices;
				meshData.Name = mesh->mName.C_Str();
				pModel->Meshs.push_back(meshData);
				
				
			}		
			return pModel;
		}

	
		TextureData * ResourceManager::VGetTexture(const std::vector<std::string>& filename, bool isCube, bool tryload)
		{
			TextureData* tex = nullptr;
			tex = HasResource(m_Textures,filename[0]);
			if (tex == nullptr && tryload)
			{
				if (!isCube)
				{
					for (auto&el : filename)
					{
						tex = LoadTexture(filename[0]);
						if (tex) m_Textures.push_back(ResourceHandle<TextureData>(el, tex));
						else E_ERROR("Can't load texture: %s", el);
					}
				}
				else
				{
					tex = LoadCubeTex(filename);
					if (tex) m_Textures.push_back(ResourceHandle<TextureData>(filename[0], tex));
					else E_ERROR("Can't load cube texture: %s", filename[0]);
				}
				
			}
			
			return tex;

		}
		ShaderCode* ResourceManager::VGetShaderCode(const std::string & filename, bool tryload)
		{
			ShaderCode* pCode = nullptr;
			CheckResourceFunc a = [](const std::string&a, const std::string& b)
			{
				bool s = a.find(b) != string::npos;
				return s;
			};

			pCode = HasResource(m_ShaderCodes, filename,a);

			if (pCode == nullptr && tryload)
			{
				pCode = LoadShaderCode(filename);
				if (pCode) m_ShaderCodes.push_back(ResourceHandle<ShaderCode>(filename, pCode));

			}
			return pCode;
		}
	
		ModelData * ResourceManager::VGetModel(const string& filename, bool tryload)
		{
			ModelData* pModel = nullptr;
			pModel = HasResource(m_Models, filename);
			if (pModel) return pModel;

			if (!tryload) return pModel;

			
			pModel = LoadModel(filename);
			
			
			if(pModel==nullptr) E_ERROR("Cound not load model: %s", filename.c_str());
			else m_Models.push_back(ResourceHandle<ModelData>(filename, pModel));

			return pModel;
		}

		HeightMapData * ResourceManager::VGetHeightMap(const string& filename, bool tryload)
		{
			HeightMapData* hm = nullptr;
			hm = HasResource(m_HeightMaps,filename);
			if (hm == nullptr && tryload)
			{
				hm = LoadHeightMap(filename/*,20,-1,-1,10,16*/);
				if(hm==nullptr) E_ERROR("Could not find heightmap: %s", filename.c_str());
				else m_HeightMaps.push_back(ResourceHandle<HeightMapData>(filename, hm));
				
			}
			return hm;
		}
		Sound * ResourceManager::VGetSound(const string & tag, bool tryload)
		{
			Sound* pSound = HasResource(m_SoundList, tag, [](const std::string& inList, const std::string& tag)
			{return inList.find(tag) != std::string::npos; });

			if (pSound) return pSound;
			if (!tryload) return pSound;

			pSound = LoadSound(tag, FMOD_3D);

			if (pSound)
			{
				m_SoundList.push_back(ResourceHandle<Sound>(tag, pSound));
				return pSound;
			}
			
			return nullptr;
		}

		SpriteData * ResourceManager::VGetSprite(const std::string & filename, bool tryload)
		{
			SpriteData* pData = HasResource(m_Sprites, filename);
			if (pData == nullptr)
			{
				if(tryload) pData = LoadSpriteAnimation(filename);
				if(pData==nullptr) return nullptr;

				m_Sprites.push_back(ResourceHandle<SpriteData>(filename, pData));
			}
		
			return pData;
		}

		void ResourceManager::PostInit()
		{
			//m_pRenderDevice = m_pContext->GetSystem<render::IRenderSystem>()->GetRenderDevice();
			LoadSystemResources();
		}

		

		

		ShaderCode* ResourceManager::LoadShaderCode(const std::string & filepath)
		{
			char* sourceVertex=nullptr;
			ShaderCode* pShaderCode = NULL;
			FILE* pvFile = fopen(filepath.c_str(), "rt");

			if (!pvFile)
			{
				E_ERROR("Can't open File: %s", filepath.c_str());
				return nullptr;
			}
			if (fseek(pvFile, 0L, SEEK_END) == 0)
			{
				long bufsize = ftell(pvFile);
				fseek(pvFile, 0L, SEEK_SET);

				sourceVertex = DEBUG_NEW char[bufsize+1];
				size_t newLen = fread(sourceVertex, sizeof(char), bufsize, pvFile);
				sourceVertex[newLen] = '\0'; /* Just to be safe. */

				ShaderCode* pShaderCode = DEBUG_NEW ShaderCode;
				pShaderCode->m_Object.reset(sourceVertex);
				pShaderCode->size = bufsize + 1;
				return pShaderCode;
			}
			fclose(pvFile);

			return pShaderCode;
		}

		void ResourceManager::LoadResources(string path)
		{

			tinyxml2::XMLDocument doc;
			doc.LoadFile(path.c_str());
			tinyxml2::XMLElement* p = doc.FirstChildElement("Resources");

			std::string Resourcepath = p->Attribute("Path");
			// Loop through each child element and load the component
			for (tinyxml2::XMLElement* pNode = p->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
			{
				const char* name = pNode->Value();
				if (name == nullptr) continue;

				if (!strcmp(name, "Texture"))
				{
					const char* pFile = pNode->Attribute("File");
					VGetTexture(std::vector<std::string>{Resourcepath + pFile},true);
				}
				else if (!strcmp(name, "Shader"))
				{
					const char* pFile = pNode->Attribute("File");
					VGetShaderCode(Resourcepath + pFile,true);
				}
				else if (!strcmp(name, "Shader"))
				{
					const char* pFile = pNode->Attribute("File");
					VGetShaderCode(Resourcepath + pFile, true);
				}
				else if (!strcmp(name, "CubeTexture"))
				{
					std::vector<std::string> filelist;

					for (tinyxml2::XMLElement* pNodeTex = pNode->FirstChildElement(); pNodeTex; pNodeTex = pNodeTex->NextSiblingElement())
					{
						const char* pFile = pNodeTex->Attribute("File");
						filelist.push_back(Resourcepath + pFile);
					}

					VGetTexture(filelist, true,true);
				}
				else if (!strcmp(name, "Model"))
				{
					const char* pFile = pNode->Attribute("File");
					VGetModel(Resourcepath+pFile, true);
				}
				
				
				else if (!strcmp(name, "HeightMap"))
				{
					const char* pFile = pNode->Attribute("File");
					/*int size = pNode->DoubleAttribute("Size", 5.0f);
					int w = pNode->DoubleAttribute("Width", 2.0f);
					int h = pNode->DoubleAttribute("Height", 2.0f);
					float hscale = pNode->DoubleAttribute("HeightScale", 1.0);
					int s = pNode->DoubleAttribute("SubDevided", 1.0);*/
					if (pFile) VGetHeightMap(pFile/*, size, w, h, hscale, s*/, true);
				}
				else if (!strcmp(name, "Sprite"))
				{
					const char* pFile = pNode->Attribute("File");
					if (pFile) VGetSprite(pFile, true);
				}
				/*else if (!strcmp(name, "Sound"))
				{

					const char* pFile = pNode->Attribute("File");
					const char* pTag = pNode->Attribute("Tag");
					unsigned int mode = pNode->Int64Attribute("Sound3D", 0);
					SoundRAAI* pSound = LoadSound(pFile, pTag, mode ? FMOD_3D : FMOD_2D);


					if (pSound)
					{
						if (mode)
						{
							float InnerRadius = pNode->FloatAttribute("InnerRadius", 1.0f);
							float OuterRadius = pNode->FloatAttribute("OuterRadius", 512.0f);
							pSound->GetSound()->set3DMinMaxDistance(InnerRadius, OuterRadius);
							pSound->GetSound()->setMode(FMOD_3D_LINEARROLLOFF);
						}

						m_SoundList.insert({ pTag,std::unique_ptr<SoundRAAI>(pSound) });
					}
				}*/
				
				//sumResource++;
				
				//m_LoadStatus.percent = float(sumResource) / totalResource;
			}


			
		}

		

		void ResourceManager::LoadSystemResources()
		{
			LoadResources(SYSTEM_RESOURCES_CONFIG);
		}

		LTRawData* ResourceManager::LoadLTBModel(const std::string & filename)
		{
			LTRawData* pTemp = LTBFileLoader::LoadModel(filename.c_str());
			if (pTemp == nullptr) return nullptr;

			vector<math::AABB> abb(pTemp->SkeNodes.size());
			for (size_t i = 0; i < pTemp->Meshs.size(); i++)
			{
				LTRawMesh* pMesh = &pTemp->Meshs[i];


				for (size_t j = 0; j < pMesh->Vertexs.size(); j++)
				{
					const SkeVertex& vertex = pMesh->Vertexs[j];
					vec3 local;
					for (int k = 0; k < 4; k++)
					{
						if (vertex.weights[k].Bone < 100.0f && vertex.weights[k].weight >= 0.0f)
						{
							local = pTemp->SkeNodes[vertex.weights[k].Bone].m_InvBindPose*vec4(vertex.pos, 1.0f);
							local *= vertex.weights[k].weight;
							abb[vertex.weights[k].Bone].Test(local);
							pTemp->SkeNodes[vertex.weights[k].Bone].m_Flag = 1;
						}
						else
						{
							//assert(0);
						}
					}
				}
			}

			for (size_t i = 0; i < pTemp->SkeNodes.size(); i++)
			{
				//vec3 size = abb[i].Max - abb[i].Min;
				//vec3 pos = size / 2.0f + abb[i].Min;
				//pos.x = 0;
				//pos.y = 0;


				//if(size<vec3(20)) pTemp->pSkeNodes[i]->m_Flag = 0;
				//cout << size.x <<" " << size.y << " " << size.z << endl;
				pTemp->SkeNodes[i].m_BoundBox.Min = abb[i].Min;
				pTemp->SkeNodes[i].m_BoundBox.Max = abb[i].Max;
			}

			return pTemp;
		}


		ResourceManager::OpenGLContext::OpenGLContext(IContext * pContext)
		{
			OpenGLWindows* pWindow = static_cast<OpenGLWindows*>(pContext->GetSystem<IWindow>());
			pthread = glfwCreateWindow(1, 1, "Load", nullptr, pWindow->GetGLFW());
			if (pthread == nullptr) E_ERROR("Failed to create thread loading...");
			glfwHideWindow((GLFWwindow*)pthread);
			
		}

		void ResourceManager::OpenGLContext::MakeContext()
		{
			glfwMakeContextCurrent((GLFWwindow*)pthread);
		}

		ResourceManager::OpenGLContext::~OpenGLContext()
		{
			glfwDestroyWindow((GLFWwindow*)pthread);
		}

}
}
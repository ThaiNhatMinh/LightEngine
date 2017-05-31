#pragma once
#include "..\pch.h"
#include "LTBDef.h"

struct LTBProp
{
	uint32		m_iFileVersion;
	uint32		m_nKeyFrames;		// Number of keyframes.
	uint32		m_nParentAnims;		// Number of animations (that come from us).
	uint32		m_nNodes;			// Number of nodes.
	uint32		m_nPieces;			// Number of pieces.
	uint32		m_nChildModels;		// Number of child models (including the self child model).
	uint32		m_nTris;			// Number of triangles.
	uint32		m_nVerts;			// Number of vertices.
	uint32		m_nVertexWeights;	// Number of vertex weights.
	uint32		m_nLODs;			// Number of LODs.
	uint32		m_nSockets;			// Number of sockets.
	uint32		m_nWeightSets;		// Number of weight sets.
	uint32		m_nAnimData;		// Amount of animation data
	uint32		m_nStrings;			// How many strings we're allocating.
	uint32		m_StringLengths;	// Sum of all string lengths (not including null terminator).
	uint32		m_VertAnimDataSize; // byte size of the vertex animation data.
									//file positions for fields that need to be updated after a certain phase has passed
	uint32		m_nAnimDataPos;		// the position of the animation data size in the file
	float		m_GlobalRadius;		// Global radius.. MUST enclose the whole model.
	uint32		m_iNumEnabledOBBs;	// Number of enable OBB
};

struct LTBSocket
{
	uint32 m_iNode;
	char m_Name[100];
	vec3 m_Pos;
	Quaternion m_Ort;
	vec3 m_Scale;
};

struct WeightBlend
{
	char Name[16];
	float Blend[100];
};
struct SkeNode;
struct AnimNode;
struct AnimKeyFrame;
struct Animation;
class SkeMesh;
class LTBFile
{
public:
	static FILE* pFile;
protected:
	static bool LoadSkeleton(FILE * pFile, SkeNode* pParent, vector<SkeNode*>& nodeLists);
	static void ReadData(FILE* pFile, AnimNode& node, const vector<AnimKeyFrame>&, unsigned int);
public:
	LTBFile() {};
	~LTBFile() {};
	static bool BeginLoad(const char* filename);
	static LTBProp*				LoadProp();
	static vector<SkeMesh*>		LoadMesh();
	static vector<SkeNode*>		LoadSkeleton();
	static vector<WeightBlend>	LoadWS();
	static vector<string>		LoadChildName();
	static vector<Animation*>	LoadAnimation(const vector<SkeNode*>& skenode);
	static vector<LTBSocket>	LoadSocket();
	static void					EndLoad();
};
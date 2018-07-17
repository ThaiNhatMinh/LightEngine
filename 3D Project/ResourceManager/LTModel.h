#pragma once
#include <string>
#include <vector>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include "..\typedef.h"
#include "LTRawData.h"
#include "..\Math\AABB.h"
namespace Light
{


	struct SkeNode
	{
		// Name of node
		std::string m_Name;
		// Index of Node 
		int32 m_Index;
		// Parent of node, -1 if root
		int32 m_ParentIndex;
		// transform to default vertex;
		glm::mat4 m_GlobalTransform;
		// transform vertex to local coord;
		glm::mat4 m_InvBindPose;

		uint32 m_Flag = 0;
		// BoundBox OBB;
		math::AABB m_BoundBox;
	};


	struct WeightBlend
	{
		char Name[16];
		float Blend[100];
	};


	struct AnimKeyFrame
	{
		unsigned int		m_Time;
		// A string of information about this key..
		std::string			m_pString;
	};


	// animation data in one node

	struct FrameData
	{
		glm::vec3 m_Pos;
		glm::quat m_Ort;
	};

	

	struct AnimNode
	{
		typedef std::vector<FrameData> AnimData;

		int32 Parent;
		AnimData Data;
	};

	struct Animation
	{
		std::string Name;
		math::AABB BV;
		std::vector<AnimKeyFrame> KeyFrames;
		std::vector<AnimNode> AnimNodeLists;
	};


	struct LTBSocket
	{
		unsigned int m_iNode;
		std::string name;
		glm::mat4 Transform;
	};


	class LTModel
	{
	public:
		std::vector<LTRawData>					Meshs;
		std::vector<SkeNode>						SkeNodes;
		std::vector<WeightBlend>					wb;
		std::vector<std::string>						ChildName;
		std::vector<Animation>					Anims;
		std::vector<LTBSocket>					Sockets;


	};
}

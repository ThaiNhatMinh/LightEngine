#pragma once


enum eOctreeNodes
{
	TOP_LEFT_FRONT,			// 0
	TOP_LEFT_BACK,			// 1
	TOP_RIGHT_BACK,			// etc...
	TOP_RIGHT_FRONT,
	BOTTOM_LEFT_FRONT,
	BOTTOM_LEFT_BACK,
	BOTTOM_RIGHT_BACK,
	BOTTOM_RIGHT_FRONT,
	OCTREE_MAX_NODE
};

class Octree
{
public:
	Octree() {};
	~Octree() {};

	void Build(const vector<vec3>& vertex);
private:
	vec3			m_Center;
	bool			m_IsSubDevided;
	Octree*			m_SubNode[OCTREE_MAX_NODE];
	float			m_Size;
	int				m_MaxVertex;

};
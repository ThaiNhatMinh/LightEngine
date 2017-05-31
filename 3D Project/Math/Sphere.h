#pragma once
#include "..\pch.h"


// Bounding sphere
class BVSphere
{
private:
	vec3 m_Pos;
	float m_fRadius;

public:
	BVSphere() ;
	~BVSphere() {};


	float GetRadius() const{ return m_fRadius; }
	vec3 GetPos()const { return m_Pos; }
	float& GetRadius() { return m_fRadius; }
	vec3& GetPos() { return m_Pos; }

	void Set(const vector<vec3>& v);

	// sphere
	bool Intersect(const BVSphere& s);

	// ray
	bool Intersect(const vec3& pos, const vec3& direction);
	// plane
	float Classify(const Plane& p);

};
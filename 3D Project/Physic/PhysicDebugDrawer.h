#pragma once
#include "pch.h"


class BulletDebugDrawer : public btIDebugDraw
{
public:
	DebugDrawModes              m_DebugModes;
	Debug						*m_Debug;
public:
	// btIDebugDraw interface
	BulletDebugDrawer(Context* c);
	virtual void	drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
	virtual void	reportErrorWarning(const char* warningString) override;
	virtual void	draw3dText(const btVector3& location, const char* textString) override;
	virtual void	setDebugMode(int debugMode) override;
	virtual int		getDebugMode() const override;
	virtual void	drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;

	void ReadOptions(void);
};

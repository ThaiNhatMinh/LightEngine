#pragma once

#include "..\pch.h"


class RenderAPICore : public Singleton<RenderAPICore>
{
public:
	RenderAPICore() {};
	~RenderAPICore() {};
	virtual void  onStartUp() = 0;
	virtual void  onShutDown() = 0;


	virtual void SetViewport(int x, int y, int weight, int height)=0;
	virtual void ClearBuffer()=0;
};
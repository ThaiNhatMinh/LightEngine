#pragma once

class EvtRequestCreateSprite :public IEvent
{
public:
	EvtRequestCreateSprite(const string& file, const vec3& pos);

	const vec3& GetPos();
	Actor* GetParent();
	void SetParent(Actor* p);
	void SetLoop(bool loop);
	bool isLoop();
	const string& GetFile();

public:
	EVENT_DEFINE(EvtRequestCreateSprite)
private:
	bool Loop;
	string File;
	Actor* Parent;
	vec3 Pos;
};

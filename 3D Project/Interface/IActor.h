#pragma once
class IActor
{
public:
	enum ActorState
	{
		AS_NOUPDATE = 1 << 1,
		AS_NORENDER = 1 << 2,
		AS_NORMAL = 1 << 3,
	};
public:
	
	virtual ~IActor() = default;

	virtual bool		Init(const tinyxml2::XMLElement* pData) = 0;
	virtual void		PostInit(void) = 0;
	virtual void		Destroy(void) = 0;

	virtual void		VSetName(std::string name) = 0;
	virtual string		VGetName() = 0;
	virtual void		VSetTag(string tag) = 0;
	virtual string		VGetTag() = 0;
	virtual void		VSetTransform(const mat4& toWorld) = 0;
	virtual const mat4&	VGetTransform()const = 0;
	virtual mat4		VGetGlobalTransform() = 0;
	virtual HRESULT		VOnUpdate(Scene *, float elapsedMs) = 0;
	virtual HRESULT		VPostUpdate(Scene *) = 0;
	virtual HRESULT		VPreRender(Scene *pScene) = 0;
	virtual bool		VIsVisible(Scene *pScene) const = 0;
	virtual HRESULT		VRenderChildren(Scene *pScene) = 0;
	virtual HRESULT		VPostRender(Scene *pScene) = 0;
	virtual HRESULT		VRender(Scene *pScene) = 0;
	virtual void		VSetState(ActorState state) = 0;
	virtual ActorState	VGetState() = 0;
	virtual bool		VAddChild(std::unique_ptr<Actor> kid) = 0;
	virtual bool		VRemoveChild(ActorId id) = 0;
	virtual IActor*		VGetChild(int index) = 0;
	virtual IActor*		VGetChild(const string& name) = 0;
	virtual IActor*		VGetParent() = 0;
};

class IMachineState
{
public:
    virtual void Update(float dt)=0;

    int				KeyFrameID;
	float			m_fTime;
	GLuint			m_iCurrentAnim;
	GLuint			m_iCurrentFrame;
	AnimationState	m_State;
    int             m_KeyBind;
    char            m_Flags;
};

class BaseAnimComponent
{
private:
    std::map<int,IMachineState> m_KeyToState;
    IMachineState   *m_MainState;
    IMachineState   *m_SubState;
public:
    BaseAnimComponent()
    {
        m_MainState=nullptr;
        m_SubState = nullptr;
    }
    virtual bool VInit(const tinyxml2::XMLElement* pData)
    {
        //... Read data
        // find main state IMachineState mainstate;
        m_MainState = mainstate;
        m_SubState = mainstate;
    }
    void OnKey(int key)
    {
        IMachineState state = m_KeyToState[key];
        if(!state) return;
        char bodypart = FindBS(state->m_iCurrentAnim);
        if(bodypart==upper)
        {
            m_SubState = state;
            ResetControl(m_SubState);
        }
        else
        {
             m_MainState = state;
            ResetControl(m_MainState);
        }

    }
    void VUpdate(float deltaMs)
    {
        m_MainState.Update(deltaMs);
        if(m_SubState) m_SubState.Update(deltaMs);
        
    }
};
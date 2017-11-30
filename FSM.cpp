
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
};

class BaseAnimComponent
{
private:
    std::map<int,std::unique_ptr<IMachineState>> m_KeyToState;
    IMachineState   *m_MainState;
    IMachineState   *m_SubState;
public:
    BaseAnimComponent()
    {
        m_MainState=nullptr;
        m_SubState = nullptr;
    }
    void OnKey(int key)
    {
        IMachineState* state = m_KeyToState[key];
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
};
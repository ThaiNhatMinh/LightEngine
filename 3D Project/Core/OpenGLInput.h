#pragma once

namespace Light
{
	class OpenGLInput : public IInput
	{
		friend class OpenGLWindows;

	public:
		OpenGLInput(IContext* pContext);
		virtual void				VUpdate()override;
		virtual const char*			VGetName()override;
		virtual bool				VOnKey(Key k);
		virtual bool				VOnMouse(MouseButton m);
		virtual void				VMouseDeltaPos(int& dx, int& dy);
	private:
		bool keys[1024]; // hold key status
		bool Mouse[3];
		float MouseD[2];
		int m_MousePos[2];
	};
}
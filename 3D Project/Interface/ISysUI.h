#pragma once


namespace Light
{
	class ISysUI : public ISubSystem
	{
	public:

		virtual void Update(float dt) = 0;
		virtual void Render() = 0;
	};
}
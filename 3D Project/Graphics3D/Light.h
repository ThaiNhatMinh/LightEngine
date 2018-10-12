#pragma once



namespace Light
{
	namespace render
	{
		
		struct LightParam;
		class LightData
		{
		public:
			virtual ~LightData() = default;

			virtual void SetupParam(LightParam*) = 0;
		};
	}
}
#pragma once
namespace ViewerInterop {
	public ref class RLightManagerInterop :
		public IRLightManager
	{
	private:
		RLightsManager* m_manager;
	public:
		RLightManagerInterop(RLightsManager* manager);

		virtual void SwitchMainLight(bool enable);
	
		// Inherited via IRLightManager
		virtual void HeadLightSwitch(bool v);

		virtual void SkyLightSwitch(bool v);

		virtual void ExtraLights(bool v);

		

		// Inherited via IRLightManager
		virtual Viewer::Interfaces::IRLight ^ CreateLight(Viewer::Interfaces::IRTransformNode ^transNode, System::Windows::Media::Color color, double posX, double posY, double posZ);


		// Inherited via IRLightManager

		// Inherited via IRLightManager
	};
}


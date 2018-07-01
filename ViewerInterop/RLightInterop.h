#pragma once
#include "RLightsManager.h"
namespace ViewerInterop {
	public ref class RLightInterop :Viewer::Interfaces::IRLight
	{
	protected:
		RLight* m_Light;

	public:
		RLightInterop(RLight* Light);

		// Inherited via IRNode
		virtual property Viewer::RNodeType NodeType;
		virtual property System::String ^ Name;
		virtual property Viewer::Interfaces::IRNode ^ Parent;
		virtual System::String ^ GetName();
		virtual void SetName(System::String ^name);
		virtual void SetPolygonMode(int mode);
		virtual System::Object ^ GetWrappedNativeObject();
	};
}

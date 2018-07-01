#include "stdafx.h"
#include "RLightInterop.h"

namespace ViewerInterop {
	RLightInterop::RLightInterop(RLight* Light) 
	{
		m_Light = Light;
		NodeType = Viewer::RNodeType::Light;
	
		Name = "light";

	}
	System::String ^ RLightInterop::GetName()
	{
		return Name;
		// TODO: insert return statement here
	}
	void RLightInterop::SetName(System::String ^name)
	{
		Name = name;
	}
	void RLightInterop::SetPolygonMode(int mode)
	{
		throw gcnew NotImplementedException();
	}
	System::Object ^ RLightInterop::GetWrappedNativeObject()
	{
		throw gcnew NotImplementedException();
	}
}
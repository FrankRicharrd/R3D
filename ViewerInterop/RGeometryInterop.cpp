#include "stdafx.h"
#include "RGeometryInterop.h"


namespace ViewerInterop {

	RGeometryInterop::RGeometryInterop()
	{
	}

	System::String ^ RGeometryInterop::GetName()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}

	void RGeometryInterop::SetName(System::String ^name)
	{
		throw gcnew System::NotImplementedException();
	}

	void RGeometryInterop::SetPolygonMode(int mode)
	{
		throw gcnew System::NotImplementedException();
	}

	System::Object ^ RGeometryInterop::GetWrappedNativeObject()
	{
		return nullptr;
	}

}
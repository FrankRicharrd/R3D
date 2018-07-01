#pragma once
#include "RIntelligentObject.h"
using namespace System::Collections::Generic;
namespace R3DIOInterop {
	public ref class RIntelliDrawableInstanceInterop
	{
	public:
		RIntelliDrawableInstanceInterop(RIntelliDrawableInstance* instance);

		void SetMatrix(List<double>^ matrix);

		RIntelliDrawableInstanceRef GetNativeObject() { return m_instance; }

	private:
		RIntelliDrawableInstance* m_instance;
	};
}


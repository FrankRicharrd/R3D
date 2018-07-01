#include "stdafx.h"
#include "InteropUtilities.h"


InteropUtilities::InteropUtilities()
{
}

void InteropUtilities::MarshalString(String ^ s, string & os)
{
	using namespace Runtime::InteropServices;
	const char* chars =
		(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
	os = chars;
	Marshal::FreeHGlobal(IntPtr((void*)chars));
}

IVec3 ^ InteropUtilities::ConvertVec(osg::Vec3d vec)
{
	return nullptr;// gcnew Vec3(vec.x(), vec.y(), vec.z());
}

osg::Vec3d InteropUtilities::ConvertVec(IVec3 ^ vec)
{
	return osg::Vec3d(vec->X, vec->Y, vec->Z);
}

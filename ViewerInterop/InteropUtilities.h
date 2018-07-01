#pragma once
public ref class InteropUtilities
{
public:
	InteropUtilities();

	static void MarshalString(String ^ s, string& os);

	static IVec3^ ConvertVec(osg::Vec3d vec);

	static osg::Vec3d ConvertVec(IVec3^ vec);

};


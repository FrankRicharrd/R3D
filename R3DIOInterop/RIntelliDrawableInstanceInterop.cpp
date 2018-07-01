#include "stdafx.h"
#include "RIntelliDrawableInstanceInterop.h"

namespace R3DIOInterop {
	RIntelliDrawableInstanceInterop::RIntelliDrawableInstanceInterop(RIntelliDrawableInstance* instance)
	{
		m_instance = instance;
	}

	void RIntelliDrawableInstanceInterop::SetMatrix(List<double>^ m)
	{
		osg::Matrix mat(m[0], m[1], m[2], m[3],
			m[4], m[5], m[6], m[7],
			m[8], m[9], m[10], m[11],
			m[12], m[13], m[14], m[15]);

		m_instance->setMatrix(mat);
	}


	
	

}

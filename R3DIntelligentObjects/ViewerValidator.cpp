#include "stdafx.h"
#include "ViewerValidator.h"
#include "RIntelligentObject.h"

ViewerValidator::ViewerValidator()
{
}

bool ViewerValidator::IsIoObject(osg::Group * node)
{
	RIntelligentObjectRef obj = dynamic_cast<RIntelligentObject*>(node);
	if (obj.valid())
	{
		return true;
	}
	else
		return false;
}


ViewerValidator::~ViewerValidator()
{
}

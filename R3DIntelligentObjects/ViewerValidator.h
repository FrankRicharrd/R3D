#pragma once
class VIEWER_API ViewerValidator
{
public:
	ViewerValidator();

	static bool IsIoObject(osg::Group* node);
	
	~ViewerValidator();
};


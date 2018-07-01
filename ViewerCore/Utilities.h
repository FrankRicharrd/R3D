#pragma once
class Utilities
{
public:
	Utilities();

	static osg::MatrixTransform* GetNearTransformParent(osg::Node* node);

	static osg::Group* GetNodeParentIfIObject(osg::Node* node );

	~Utilities();

	static osg::ref_ptr<osg::MatrixTransform> CreateGrid();
};


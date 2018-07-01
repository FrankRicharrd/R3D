#pragma once
class R3DAPI RPrimitivesCreator
{
public:
	RPrimitivesCreator();
	~RPrimitivesCreator();

	static osg::Geometry* createPtGeometry(osg::PrimitiveSet::Mode pointType, osg::Vec3Array* vertices, const osg::Vec4 & color);
	static osg::Geometry* createLnGeometry(osg::PrimitiveSet::Mode lineType, osg::Vec3Array* vertices, const osg::Vec4 & color);
	static osg::Geometry* createTriGeometry(osg::Vec3Array* vertices, osg::Vec3Array* normals, const osg::Vec4 & color);
	static osg::Geometry* createQuadGeometry(osg::Vec3Array* vertices, osg::Vec3Array* normals, const osg::Vec4 & color);
};


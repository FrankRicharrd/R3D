#include "stdafx.h"
#include "RPrimitivesCreator.h"


RPrimitivesCreator::RPrimitivesCreator()
{
}


RPrimitivesCreator::~RPrimitivesCreator()
{
}
inline osg::Geometry* RPrimitivesCreator::createPtGeometry(osg::PrimitiveSet::Mode pointType, osg::Vec3Array* vertices, const osg::Vec4 & color)
{
	osg::Geometry* geom = new osg::Geometry;
	geom->setVertexArray(vertices);
	geom->addPrimitiveSet(new osg::DrawArrays(pointType, 0, vertices->size()));
	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(color);
	geom->setColorArray(colors, osg::Array::BIND_OVERALL);
	osg::Vec3Array *norms = new osg::Vec3Array;
	norms->push_back(osg::Vec3(0, 0, 1));
	geom->setNormalArray(norms, osg::Array::BIND_OVERALL);
	return geom;
}

inline osg::Geometry* RPrimitivesCreator::createLnGeometry(osg::PrimitiveSet::Mode lineType, osg::Vec3Array* vertices, const osg::Vec4 & color)
{
	osg::Geometry* geom = new osg::Geometry;
	geom->setVertexArray(vertices);
	geom->addPrimitiveSet(new osg::DrawArrays(lineType, 0, vertices->size()));
	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(color);
	geom->setColorArray(colors, osg::Array::BIND_OVERALL);
	osg::Vec3Array *norms = new osg::Vec3Array;
	norms->push_back(osg::Vec3(0, 0, 1));
	geom->setNormalArray(norms, osg::Array::BIND_OVERALL);
	return geom;
}

inline osg::Geometry* RPrimitivesCreator::createTriGeometry(osg::Vec3Array* vertices, osg::Vec3Array* normals, const osg::Vec4 & color)
{
	osg::Geometry* geom = new osg::Geometry;
	geom->setVertexArray(vertices);
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, vertices->size()));
	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(color);
	geom->setColorArray(colors, osg::Array::BIND_OVERALL);
	geom->setNormalArray(normals, osg::Array::BIND_PER_VERTEX);
	return geom;
}

inline osg::Geometry* RPrimitivesCreator::createQuadGeometry(osg::Vec3Array* vertices, osg::Vec3Array* normals, const osg::Vec4 & color)
{
	osg::Geometry* geom = new osg::Geometry;
	geom->setVertexArray(vertices);
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, vertices->size()));
	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(color);
	geom->setColorArray(colors, osg::Array::BIND_OVERALL);
	geom->setNormalArray(normals, osg::Array::BIND_PER_VERTEX);
	return geom;
}
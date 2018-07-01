#include "stdafx.h"
#include "ExtraGeometries.h"


GridGeom::GridGeom()
{
}


GridGeom::~GridGeom()
{
}

void GridGeom::CreateGrid()
{

	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));


	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;

	float gridsize = 1.0f;

	int numberoflines = 10;

	float startposition = ((gridsize* numberoflines) / 2.0f);
	//horizontal xz plane
	for (size_t i = 0; i <= numberoflines; i++)
	{

		vertices->push_back(osg::Vec3f(-startposition, 0, startposition - ( gridsize * i)));

		vertices->push_back(osg::Vec3f(startposition , 0, startposition -  (gridsize * i)));

		vertices->push_back(osg::Vec3f(startposition - (gridsize *i), 0, -startposition));

		vertices->push_back(osg::Vec3f(startposition - (gridsize * i), 0, startposition));
	}

	std::cout << vertices;


	setVertexArray(vertices.get());
	setColorArray(colors.get());
	setColorBinding(osg::Geometry::BIND_OVERALL);
	addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, vertices->size()));

	



}


SpecialGeometry::SpecialGeometry()
{
}

SpecialGeometry::~SpecialGeometry()
{
}

SelectionBox::SelectionBox()
{
}

SelectionBox::~SelectionBox()
{
}

void SelectionBox::CreateAndSetVertices()
{

	//osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	//vertices->push_back(v0);
	//vertices->push_back(osg::Vec3(1.0f, 0.0f, 0.0f));
	//vertices->push_back(osg::Vec3(1.0f, 1.0f, 0.0f));
	//vertices->push_back(osg::Vec3(0.0f, 1.0f, 0.0f));

	//
	//vertices->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));
	//vertices->push_back(osg::Vec3(1.0f, 0.0f, 1.0f));
	//vertices->push_back(osg::Vec3(1.0f, 1.0f, 1.0f));
	//vertices->push_back(v7);



	//osg::ref_ptr<osg::DrawElementsUInt> indices =
	//	new osg::DrawElementsUInt(GL_QUADS, 24);
	//(*indices)[0] = 0; (*indices)[1] = 1; (*indices)[2] = 2; (*indices)[2] = 3;

	//(*indices)[0] = 2; (*indices)[1] = 1; (*indices)[2] = 5; (*indices)[2] = 6;

	//(*indices)[0] = 5; (*indices)[1] = 4; (*indices)[2] = 7; (*indices)[2] = 6;

	//(*indices)[0] = 4; (*indices)[1] = 0; (*indices)[2] = 3; (*indices)[2] = 7;

	//(*indices)[0] = 3; (*indices)[1] = 2; (*indices)[2] = 6; (*indices)[2] = 7;

	//(*indices)[0] = 0; (*indices)[1] = 1; (*indices)[2] = 5; (*indices)[2] = 4;

	auto v0 = osg::Vec3(-0.5f, -0.5f, 0.5f);
	auto v1 = osg::Vec3(0.5f, -0.5f, 0.5f);
	auto v2 = osg::Vec3(0.5f, 0.5f, 0.5f);
	auto v3 = osg::Vec3(-0.5f, 0.5f, 0.5f);

	auto v4 = osg::Vec3(-0.5f, -0.5f, -0.5f);
	auto v5 = osg::Vec3(0.5f, -0.5f, -0.5f);
	auto v6 = osg::Vec3(0.5f, 0.5f, -0.5f);
	auto v7 = osg::Vec3(-0.5f, 0.5f, -0.5f);



	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	vertices->push_back(v0);  //0
	vertices->push_back(v1);  //1
	vertices->push_back(v2); //2
	vertices->push_back(v3);  //3


	vertices->push_back(v4); //4
	vertices->push_back(v5);  //5
	vertices->push_back(v6);  //6
	vertices->push_back(v7);  //7


	vertices->push_back(v2); //2
	vertices->push_back(v1);  //1
	vertices->push_back(v5);  //5
	vertices->push_back(v6);  //6

	vertices->push_back(v4); //4
	vertices->push_back(v0);  //0
	vertices->push_back(v3);  //3
	vertices->push_back(v7);  //7

	vertices->push_back(v3);  //3
	vertices->push_back(v2); //2
	vertices->push_back(v6);  //6
	vertices->push_back(v7);  //7

	vertices->push_back(v0);  //0
	vertices->push_back(v1);  //1
	vertices->push_back(v5);  //5
	vertices->push_back(v4); //4


	setVertexArray(vertices.get());

	//addPrimitiveSet(indices.get());

}

CenterCoordinateAxis::CenterCoordinateAxis()
{
}

CenterCoordinateAxis::~CenterCoordinateAxis()
{
}

void CenterCoordinateAxis::createAxis()
{

	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));

	colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));


	colors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	colors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));



	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;

	vertices->push_back(osg::Vec3f(0, 0, 0));
	vertices->push_back(osg::Vec3f(10000, 0, 0));


	vertices->push_back(osg::Vec3f(0, 0, 0));
	vertices->push_back(osg::Vec3f(0, 10000, 0));


	vertices->push_back(osg::Vec3f(0, 0, 0));
	vertices->push_back(osg::Vec3f(0, 0, 10000));



	//osg::Vec3 sp(0, -180, 120);
	//osg::Vec3 ep(0, 480, 120);
	//osg::ref_ptr<Geometry> beam(new osg::Geometry);
	//osg::ref_ptr<osg::Vec3Array> points = new osg::Vec3Array;
	//points->push_back(sp);
	//points->push_back(ep);
	//osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array;
	//color->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));
	//beam->setVertexArray(points.get());
	//beam->setColorArray(color.get());
	//beam->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);
	//beam->addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, 2));







	setVertexArray(vertices.get());
	setColorArray(colors.get());
	setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, vertices->size()));

	osg::StateSet* ss = getOrCreateStateSet();
	ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	ss->setAttributeAndModes(new osg::LineWidth(0.5));
}

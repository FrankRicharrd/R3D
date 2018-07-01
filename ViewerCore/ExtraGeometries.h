#pragma once


class SpecialGeometry : public osg::Geometry
{
public:
	SpecialGeometry();

protected:
	~SpecialGeometry();

};




class GridGeom : public SpecialGeometry
{
public:
	GridGeom();
	void CreateGrid();

protected:
	~GridGeom();


};

class SelectionBox : public SpecialGeometry
{
public:

	SelectionBox();
	~SelectionBox();
	void CreateAndSetVertices();


};


class CenterCoordinateAxis : public SpecialGeometry
{
public:
	CenterCoordinateAxis();

	void createAxis();

protected:
	~CenterCoordinateAxis();

};


#include "stdafx.h"
#include "RDrawingEntity.h"


RVertex3D::RVertex3D() :
	m_rIndex(-1), m_x(0), m_y(0), m_z(0), m_index(-1), RDrawingEntity()
{
}

inline RVertex3D::RVertex3D(double x, double y, double z) :
	m_rIndex(-1), m_x(x), m_y(y), m_z(z), m_index(-1), RDrawingEntity()
{

}

inline RVertex3D::RVertex3D(const RVertex3D & p, const osg::CopyOp & copyop)
	: RDrawingEntity(p, copyop),
	m_edges(p.m_edges),
	m_faces(p.m_faces),
	m_index(p.m_index),
	m_rIndex(-1)
{
	for (REdgeCollection::const_iterator itr = p.m_edges.begin();
		itr != p.m_edges.end();
		++itr)
	{
		osg::Object* child = copyop(itr->get());
		if (child) AddEdge(dynamic_cast<REdge*>(child));
	}

	for (RFaceCollection::const_iterator itr = p.m_faces.begin();
		itr != p.m_faces.end();
		++itr)
	{
		osg::Object* child = copyop(itr->get());
		if (child) AddFace(dynamic_cast<RFace*>(child));
	}
}

inline bool RVertex3D::AddEdge(REdge * edge)
{
	return RVertex3D::Insertedge(m_edges.size(), edge);
}

inline bool RVertex3D::Insertedge(unsigned int index, REdge * edge)
{
	if (!edge) return false;

	if (containsEdge(edge))
		return false;

	if (index >= m_edges.size())
	{
		index = m_edges.size();      // set correct index value to be passed to the "childInserted" method
		m_edges.push_back(edge);
	}
	else
	{
		m_edges.insert(m_edges.begin() + index, edge);
	}
	return true;

}

inline bool RVertex3D::containsEdge(const REdge * node) const
{
	for (REdgeCollection::const_iterator itr = m_edges.begin();
		itr != m_edges.end();
		++itr)
	{
		if (itr->get() == node) return true;
	}
	return false;
}

bool RVertex3D::AddFace(RFace * face)
{
	return RVertex3D::Insertface(m_faces.size(), face);
}

bool RVertex3D::Insertface(unsigned int index, RFace * face)
{

	if (!face) return false;

	if (containsFace(face))
		return false;

	if (index >= m_faces.size())
	{
		index = m_faces.size();      // set correct index value to be passed to the "childInserted" method
		m_faces.push_back(face);
	}
	else
	{
		m_faces.insert(m_faces.begin() + index, face);
	}

	return true;
}

bool RVertex3D::containsFace(const RFace * node) const
{
	for (RFaceCollection::const_iterator itr = m_faces.begin();
		itr != m_faces.end();
		++itr)
	{
		if (itr->get() == node) return true;
	}
	return false;
}

//wont work 
osg::Vec3 RVertex3D::GetSmoothNormal()
{

	osg::Vec3f normal(0, 0, 0);
	for (auto f : m_faces)
	{
		normal += f->GetFaceNormal();
		normal.normalize();
	}
	return normal;
}

RVertex3D::~RVertex3D()
{


}
#include "stdafx.h"
#include "RDrawingEntity.h"


ConstRDrawingElementVisitor::ConstRDrawingElementVisitor()
{
}

ConstRDrawingElementVisitor::~ConstRDrawingElementVisitor()
{
}



RDrawingElementVisitor::RDrawingElementVisitor()
{
}

RDrawingElementVisitor::~RDrawingElementVisitor()
{
}



RDrawingEntity::RDrawingEntity()
{
}

RDrawingEntity::RDrawingEntity(const RDrawingEntity & copy, const osg::CopyOp & copyop) : Object(copy, copyop)
{
}

RDrawingEntity::~RDrawingEntity()
{
}




REdge::REdge(RVertex3D* startpoint, RVertex3D* endpoint) : m_startpoint(startpoint), m_endpoint(endpoint), m_index(-1)
{
	m_startpoint->AddEdge(this);
	m_endpoint->AddEdge(this);
}

inline REdge::REdge(const REdge& p, const osg::CopyOp& copyop)
	:RDrawingEntity(p, copyop),
	m_startpoint(p.m_startpoint),
	m_endpoint(p.m_endpoint),
	m_faces(p.m_faces)
{

}

bool REdge::AddFace(RFace * face)
{
	bool status = REdge::Insertface(m_faces.size(), face);

	if (status)
	{
		m_startpoint->AddFace(face);
		m_endpoint->AddFace(face);
	}

	return status;
}

bool REdge::Insertface(unsigned int index, RFace * face)
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

bool REdge::containsFace(const RFace * face) const
{
	for (RFaceCollection::const_iterator itr = m_faces.begin();
		itr != m_faces.end();
		++itr)
	{
		if (itr->get() == face) return true;
	}
	return false;
}

bool REdge::IsSameEdge(REdgeRef Edge)
{
	if ((Edge->GetStartpoint()->GetIndex() == m_startpoint->GetIndex()) &&
		(Edge->GetEndpoint()->GetIndex() == m_endpoint->GetIndex()))
	{
		return true;
	}
	else if ((Edge->GetEndpoint()->GetIndex() == m_startpoint->GetIndex()) &&
		(Edge->GetStartpoint()->GetIndex() == m_endpoint->GetIndex()))
	{
		return true;
	}

	return false;
}

int REdge::IsReversed(RVertex3DRef startpoint, RVertex3DRef endpoint)
{
	if (startpoint->GetIndex() == m_startpoint->GetIndex() &&
		endpoint->GetIndex() == m_endpoint->GetIndex())
	{
		return 0;
	}
	else if (startpoint->GetIndex() == m_endpoint->GetIndex() &&
		endpoint->GetIndex() == m_startpoint->GetIndex())
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

REdge::~REdge()
{
}




REdgeloop::REdgeloop(bool isouteredge) :m_isOuter(isouteredge)
{

}

inline REdgeloop::REdgeloop(const REdgeloop& p, const osg::CopyOp& copyop)
	: RDrawingEntity(p, copyop),
	m_edges(p.m_edges),
	m_face(p.m_face),
	m_isOuter(p.m_isOuter)

{
}

inline bool REdgeloop::AddEdge(REdge * edge, int direction)
{
	return REdgeloop::Insertedge(m_edges.size(), edge, direction);
}

bool REdgeloop::Insertedge(unsigned int index, REdge * edge, int direction)
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


	if (index >= m_edgeDirection.size())
	{
		index = m_edgeDirection.size();      // set correct index value to be passed to the "childInserted" method
		m_edgeDirection.push_back(direction);
	}
	else
	{
		m_edgeDirection.insert(m_edgeDirection.begin() + index, direction);
	}

	return true;
}

bool REdgeloop::containsEdge(const REdge * edge) const
{
	for (REdgeCollection::const_iterator itr = m_edges.begin();
		itr != m_edges.end();
		++itr)
	{
		if (itr->get() == edge) return true;
	}
	return false;
}

inline REdgeloop::~REdgeloop()
{
}




RFace::RFace() : m_index(-1), m_color(144,144,144,1)
{
}

inline RFace::RFace(const RFace& p, const osg::CopyOp& copyop)
	: RDrawingEntity(p, copyop),
	m_edgeloops(p.m_edgeloops),
	m_index(p.m_index)
{
}

bool RFace::AddEdgeLoop(REdgeloop * edgeloop)
{
	bool status = RFace::InsertedgeLoop(m_edgeloops.size(), edgeloop);

	return status;

}

bool RFace::InsertedgeLoop(unsigned int index, REdgeloop * edgeloop)
{
	if (!edgeloop) return false;

	if (containsEdgeLoop(edgeloop))
		return false;

	if (index >= m_edgeloops.size())
	{
		index = m_edgeloops.size();      // set correct index value to be passed to the "childInserted" method
		m_edgeloops.push_back(edgeloop);
	}
	else
	{
		m_edgeloops.insert(m_edgeloops.begin() + index, edgeloop);
	}

	for (auto e : edgeloop->GetEdges())
	{
		//AddVertex(e->GetStartpoint());
		//AddVertex(e->GetEndpoint());

		e->AddFace(this);
	}

	edgeloop->SetFace(this);


	return true;
}

bool RFace::containsEdgeLoop(const REdgeloop * edgeloop) const
{
	for (REdgeLoopCollection::const_iterator itr = m_edgeloops.begin();
		itr != m_edgeloops.end();
		++itr)
	{
		if (itr->get() == edgeloop) return true;
	}
	return false;
}

osg::Vec3 RFace::GetMidPoint()
{
	//assuming face has four edges and arranged in same direction

	/*for (REdgeLoopCollection::const_iterator itr = m_edgeloops.begin();
		itr != m_edgeloops.end();
		++itr)
	{
		auto edgeLoop = itr->get();

		if (edgeLoop->IsOuterEdge())
		{

			auto edge1 = edgeLoop->GetEdges()[0];
			auto edge2 = edgeLoop->GetEdges()[1];

			RVertex3DRef p1;
			RVertex3DRef p2;



			p1 = edge1->GetStartpoint();
			p2 = edge1->GetEndpoint();

			if (p1->GetIndex() == edge2->GetStartpoint()->GetIndex())
			{
				p1 = edge2->GetEndpoint();
			}
			else if (p2->GetIndex() == edge2->GetStartpoint()->GetIndex())
			{
				p2 = edge2->GetEndpoint();
			}
			else if (p1->GetIndex() == edge2->GetEndpoint()->GetIndex())
			{
				p1 = edge2->GetStartpoint();
			}
			else if (p2->GetIndex() == edge2->GetEndpoint()->GetIndex())
			{
				p2 = edge2->GetStartpoint();
			}


			double x = (p1->x() + p2->x()) / 2;
			double y = (p1->y() + p2->y()) / 2;
			double z = (p1->z() + p2->z()) / 2;

			return  osg::Vec3(x, y, z);

		}

	}
*/

	RVertex3DRef p1 = m_vertices[0];
	RVertex3DRef p2 = m_vertices[2];


	double x = (p1->x() + p2->x()) / 2;
	double y = (p1->y() + p2->y()) / 2;
	double z = (p1->z() + p2->z()) / 2;

	return  osg::Vec3(x, y, z);

}

osg::Vec3 RFace::GetFaceNormal()
{

	//for (REdgeLoopCollection::const_iterator itr = m_edgeloops.begin();
	//	itr != m_edgeloops.end();
	//	++itr)
	//{
	//	auto edgeLoop = itr->get();

	//	if (edgeLoop->IsOuterEdge())
	//	{

	//		auto edge1 = edgeLoop->GetEdges()[0];
	//		auto edge2 = edgeLoop->GetEdges()[1];

	//		RVertex3DRef p1;
	//		RVertex3DRef p2;
	//		RVertex3DRef p3;




	//		p1 = edge1->GetStartpoint();
	//		p2 = edge1->GetEndpoint();

	//		if (p1->GetIndex() == edge2->GetStartpoint()->GetIndex())
	//		{
	//			p3 = edge2->GetEndpoint();
	//		}
	//		else if (p2->GetIndex() == edge2->GetStartpoint()->GetIndex())
	//		{
	//			p3 = edge2->GetEndpoint();
	//		}
	//		else if (p1->GetIndex() == edge2->GetEndpoint()->GetIndex())
	//		{
	//			p3 = edge2->GetStartpoint();
	//		}
	//		else if (p2->GetIndex() == edge2->GetEndpoint()->GetIndex())
	//		{
	//			p3 = edge2->GetStartpoint();
	//		}


	//		/*if (edge1->GetStartpoint() != edge2->GetEndpoint() && edge1->GetEndpoint() != edge2->GetStartpoint())
	//		{
	//			p1 = edge1->GetStartpoint();
	//			p2 = edge2->GetEndpoint();
	//			p3 = edge1->GetEndpoint();
	//		}
	//		else
	//		{
	//			p1 = edge1->GetEndpoint();
	//			p2 = edge2->GetStartpoint();
	//			p3 = edge1->GetStartpoint();
	//		}*/


	//		//osg::Vec3 u = p2->GetOsgVec() - p1->GetOsgVec();
	//		//osg::Vec3 v = p3->GetOsgVec() - p1->GetOsgVec();


	//		osg::Vec3 u = p2->GetOsgVec() - p1->GetOsgVec();
	//		osg::Vec3 v = p1->GetOsgVec() - p3->GetOsgVec();

	//		
	//		//osg::Vec3 n = u.operator^(osg::Vec3f(v.x(), v.y(), v.z()));

	//		osg::Vec3 n = u ^ v;
	//		//osg::Vec3 n((u.y()*v.z()) - (u.z()* v.y()), (u.z()*v.x()) - (u.x()* v.z()), (u.x()* v.y() )- (u.y()* v.x()));

	//		return  n;

	//	}

	//}

	auto a = m_vertices[0];
	auto b = m_vertices[1];
	auto c = m_vertices[2];

	osg::Vec3 u = a->GetOsgVec() - b->GetOsgVec();
	osg::Vec3 v = b->GetOsgVec() - c->GetOsgVec();

	osg::Vec3 n = u ^ v;
	return n;
}

bool RFace::AddVertex(RVertex3D * vertex)
{
	bool status = RFace::InsertVertex(m_vertices.size(), vertex);

	return status;
}

bool RFace::InsertVertex(unsigned int index, RVertex3D * vertex)
{
	if (!vertex) return false;

	if (containsVertex(vertex))
		return false;

	if (index >= m_vertices.size())
	{
		index = m_vertices.size();      // set correct index value to be passed to the "childInserted" method
		m_vertices.push_back(vertex);
	}
	else
	{
		m_vertices.insert(m_vertices.begin() + index, vertex);
	}
	vertex->AddFace(this);
	return true;
}

bool RFace::containsVertex(const RVertex3D * vertex) const
{
	for (RVertex3DCollection::const_iterator itr = m_vertices.begin();
		itr != m_vertices.end();
		++itr)
	{
		if (itr->get() == vertex) return true;
	}
	return false;
}

//RVertex3DCollection RFace::GetDistinctPoints(REdgeRef edge1, REdgeRef edge2)
//{
//	RVertex3DCollection points;
//
//	points.push_back( edge1->GetStartpoint());
//	points.push_back(edge1->GetEndpoint());
//
//	if (points[0]->GetIndex() == edge2->GetStartpoint()->GetIndex())
//	{
//		p1 = edge2->GetEndpoint();
//
//
//	}
//	else if (points[0]->GetIndex() == edge2->GetStartpoint()->GetIndex())
//	{
//		p2 = edge2->GetEndpoint();
//	}
//	else if (p1->GetIndex() == edge2->GetEndpoint()->GetIndex())
//	{
//		p1 = edge2->GetStartpoint();
//	}
//	else if (p2->GetIndex() == edge2->GetEndpoint()->GetIndex())
//	{
//		p2 = edge2->GetStartpoint();
//	}
//}



RFace::~RFace()
{
}

REdge::REdge() : m_startpoint(), m_endpoint()
{
}

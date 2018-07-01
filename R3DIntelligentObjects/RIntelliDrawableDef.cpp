#include "stdafx.h"
#include "RCustomDragger.h"
#include "RDraggerCallbacks.h"
#include "RIntelligentObject.h"
#include "DraggerManager.h"

RIntelliDrawableDef::RIntelliDrawableDef(RVertex3DCollection vertices, RIntelligentObject* intelliibject) : RDrawableDefinition()
{

	for (auto v : vertices)
	{
		AddVertex(v);
	}


	m_intelliibject = intelliibject;
}

inline RIntelliDrawableDef::RIntelliDrawableDef(const RIntelliDrawableDef & copy, const osg::CopyOp & copyop) : RDrawableDefinition(copy, copyop)
{

}



void RIntelliDrawableDef::SetDraggerVisibility(bool isvisible)
{
	for (RDrawableInstanceCollection::const_iterator itr = m_instances.begin();
		itr != m_instances.end();
		++itr)
	{

		auto instance = itr->get();

		RIntelliDrawableInstance* obj = dynamic_cast<RIntelliDrawableInstance*>(instance);
		obj->SetDraggerVisibility(isvisible);
	}
}

bool RIntelliDrawableDef::AddFaceDragger(int faceIndex)
{
	if (!(faceIndex >= 0))
		return false;

	RFaceRef face = m_faces[faceIndex];

	if (!face.valid())
		return false;

	osg::ref_ptr<RFaceDragger> dragger = new RFaceDragger(face.get());
	dragger->setupDefaultGeometry();


	/*osg::ref_ptr<DraggerContainer> draggerContainer = new DraggerContainer();
	draggerContainer->setDragger(dragger);

	dragger->SetOsgParent(draggerContainer);*/

	m_draggers.push_back(dragger);

}

void RIntelliDrawableDef::UpdateDraggers()
{
	for (auto d : m_draggers)
	{
		d->UpdatePosition();
	}
}


RIOFaceRef RIntelliDrawableDef::CreateFace(std::vector<int> vertexIndex)
{
	RIOFaceRef face = new RIOFace();
	REdgeloopRef edgeLoop = new REdgeloop();



	for (size_t i = 0; i < vertexIndex.size(); i++)
	{
		face->AddVertex(m_vertices[vertexIndex[i]]);
		int p1 = 0, p2 = 0;
		if (i == vertexIndex.size() - 1)
		{
			if (vertexIndex[0] == vertexIndex[i])
				break;
			p1 = vertexIndex[i];
			p2 = vertexIndex[0];
		}
		else
		{
			p1 = vertexIndex[i];
			p2 = vertexIndex[i + 1];
		}

		REdgeRef edge = new REdge(m_vertices[p1], m_vertices[p2]);

		int e1 = GetOrCreateEdge(edge);


		if (e1 == -1)
		{
			AddEdge(edge);
			e1 = edge->GetIndex();
		}
		edge = m_edges[e1];

		int order = edge->IsReversed(m_vertices[p1], m_vertices[p2]);
		if (order == -1)
		{
			return NULL;
		}
		else if (order == 0)
		{
			edgeLoop->AddEdge(edge, 0);

		}
		else if (order == 1)
		{
			edgeLoop->AddEdge(edge, 1);
		}
		else
		{
			return NULL;
		}
	}
	face->AddEdgeLoop(edgeLoop);
	AddFace(face);
	return face;
}



int RIntelliDrawableDef::GetOrCreateEdge(REdgeRef edge)
{
	for (auto e : m_edges)
	{
		if (e->IsSameEdge(edge))
		{
			int  i = e->GetIndex();
			return i;
		}
	}

	AddEdge(edge);

	return edge->GetIndex();
}

RIntelliDrawableDef::~RIntelliDrawableDef()
{
}

RIntelliDrawableDef::RIntelliDrawableDef()
{
}

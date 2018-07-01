#include "stdafx.h"
#include "RDrawingEntity.h"

RMesh::RMesh(RGeomDefinitionRef def) : m_definition(def)
{
}

RMesh::RMesh()
{

}

RMesh::RMesh(const RMesh & copy, const osg::CopyOp & copyop) : RGeomEntity(copy, copyop)
{
}

RGeomEdgeLoopCollection RMesh::GetOuterEdgeLoops()
{
	//if (m_outerEdges.size() > 0)
	//	return m_outerEdges;

	return m_outerEdges;
}

RMesh::~RMesh()
{
}

#include "stdafx.h"
#include "RDrawingEntity.h"

RGeomEdgeLoop::RGeomEdgeLoop()
{
	
}

RGeomEdgeLoop::RGeomEdgeLoop(RGeomEntityRef parent) : m_parent(parent)
{
	
}

RGeomEdgeLoop::RGeomEdgeLoop(const RGeomEdgeLoop & copy, const osg::CopyOp & copyop) : RGeomEntity(copy, copyop)
{
}

RGeomEdgeLoop::~RGeomEdgeLoop()
{
}

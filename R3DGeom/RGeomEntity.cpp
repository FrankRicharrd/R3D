#include "stdafx.h"
#include "RDrawingEntity.h"
RGeomEntity::RGeomEntity()
{
}

RGeomEntity::RGeomEntity(const RGeomEntity & copy, const osg::CopyOp & copyop) :osg::Geometry(copy, copyop)
{
}

RGeomEntity::~RGeomEntity()
{
}

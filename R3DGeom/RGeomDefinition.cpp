#include "stdafx.h"
#include "RDrawingEntity.h"

RGeomDefinition::RGeomDefinition(const RGeomDefinition & copy, const osg::CopyOp & copyop)
	: osg::Geode(copy, copyop)
{
	m_name = copy.m_name;

	for(auto inst : copy.m_instances)
	{
		Node* child = copyop(inst.get());
		RGeomInstance* rinst = dynamic_cast<RGeomInstance*>(child);
		rinst->SetDefinition(this);
		if (rinst) m_instances.push_back(rinst);
	}



}

RGeomInstanceRef RGeomDefinition::CreateInstance()
{
	RGeomInstanceRef inst = new RGeomInstance(this);
	
	m_instances.push_back(inst);
	return inst;
}

bool RGeomDefinition::AddGeomEntity(RGeomEntityRef entity)
{
	m_geomEntities.push_back(entity.get());
	return addDrawable(entity.get());
}

bool RGeomDefinition::AddGeomInstanceChild(RGeomInstanceRef inst)
{
	m_children.push_back(inst);
	return addChild(inst);
}
//
//bool RGeomDefinition::AddGeomDefinitionAsChild(RGeomDefinitionRef defasChild)
//{
//	m_DefinitionsAschildren.push_back(defasChild);
//	return addChild(defasChild);
//}

RGeomDefinition::RGeomDefinition(): m_name("")
{
	m_mesh = new RMesh(this);
	addDrawable(m_mesh);
}

RGeomDefinition::~RGeomDefinition()
{
	for (auto inst : m_instances)
	{
		if (inst.valid())
			inst.release();
	}

	for (auto inst : m_children)
	{
		if (inst.valid())
			inst.release();
	}

	

}

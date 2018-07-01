#include "stdafx.h"
#include "RNode.h"


CRNode::CRNode(OsgNode node)
{
	m_node = node;

}

CRNode::CRNode(const CRNode & obj)
{
	m_node = obj.m_node;

}

CRNode::~CRNode()
{
}

void CRNode::SetName(string name)
{
	m_node->setName(name.c_str());
}


void CRNode::SetPolygonMode(ViewerCore::RPolygonMode  mode)
{

	
	osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode;


	switch (mode)
	{
	case ViewerCore::RPolygonMode::Wireframe:
		pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
		break;
	case ViewerCore::RPolygonMode::Shaded:
		pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL);
		break;
	default:
		return;
		break;
	}
	m_node->getOrCreateStateSet()->setAttribute(pm.get());
}

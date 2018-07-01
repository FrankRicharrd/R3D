#include "stdafx.h"
#include "RTransformNode.h"


CRTransformNode::CRTransformNode(osg::MatrixTransform* node) : CRNode(node)
{

}


CRTransformNode::~CRTransformNode()
{
	for (std::vector<CRNode*>::iterator itr = m_children.begin();
		itr != m_children.end();
		++itr)
	{
		CRNode* node = *itr;

		delete node;
	}
}

void CRTransformNode::AddChild(CRNode * node)
{
	node->SetParent(this);

	osg::ref_ptr<osg::Group> gnode = m_node->asGroup();

	if (gnode.valid())
	{
		OsgNode childNode = node->GetOsgNode();

		if (childNode.valid())
		{


			gnode->addChild(childNode);

		}

	}
	m_children.push_back(node);
}

void CRTransformNode::RemoveChild(CRNode * node)
{
	osg::ref_ptr<osg::Group> gnode = m_node->asGroup();

	if (gnode.valid())
	{

		gnode->removeChild(node->GetOsgNode());
	}

	for (std::vector<CRNode*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{

		CRNode * nn = *it;

		if (nn == node)
		{
			delete (*it);
			break;
		}
	}

}

CRNode * CRTransformNode::GetChild(unsigned int index)
{
	if (index >= m_children.size())
		return NULL;
	else
	{
		return m_children.at(index);
	}
}

osg::Vec3d CRTransformNode::GetTranslation()
{

	osg::Transform* trans = m_node->asTransform();

	if (!trans)
		return osg::Vec3d(0, 0, 0);

	OsgMatrixTransform transMat = trans->asMatrixTransform();

	const osg::Matrix mat = transMat->getMatrix();

	osg::Vec3d t = mat.getTrans();

	return t;
}

void CRTransformNode::SetTranslation(osg::Vec3d vec)
{
	OsgMatrixTransform trans = m_node->asTransform()->asMatrixTransform();

	const osg::Matrix mat = trans->getMatrix();

	osg::Matrix* newMat = new osg::Matrix(mat);

	newMat->setTrans(vec);

	trans->setMatrix(*newMat);
}






#pragma once
#include "RNode.h"
class VIEWER_API CRTransformNode :
	public CRNode
{
public:
	CRTransformNode(osg::MatrixTransform* node);
	~CRTransformNode();

	CRTransformNode* asTransformNode() { return this; }

	void AddChild(CRNode* node);
	void RemoveChild(CRNode * node);


	unsigned int GetChildrenCount() { return m_children.size(); }
	
	CRNode* GetChild(unsigned int index);

	osg::Vec3d GetTranslation();

	void SetTranslation(osg::Vec3d vec);
	
	


private: 
	std::vector<CRNode*> m_children;

};


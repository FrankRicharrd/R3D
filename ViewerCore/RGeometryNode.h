#pragma once
#include "RNode.h"
class VIEWER_API CRGeometryNode : public CRNode
{
public:
	CRGeometryNode(OsgNode node);
	
	CRGeometryNode(const CRGeometryNode& obj);
	~CRGeometryNode();

	CRGeometryNode* asGeometryNode() { return this; }


};


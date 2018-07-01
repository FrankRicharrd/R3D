#pragma once

class CRTransformNode;
class CRGeometryNode;
class VIEWER_API CRNode
{
public:
	CRNode(OsgNode node);

	CRNode(const CRNode& obj);


	~CRNode();

	virtual CRGeometryNode* asGeometryNode() { return 0; }
	virtual CRTransformNode* asTransformNode() { return 0; }


	OsgNode GetOsgNode() 
	{	
		return m_node.get();
	}
	void SetPolygonMode(ViewerCore::RPolygonMode mode);
	void SetName(string name);
	string GetName() { return m_node->getName(); }
	CRNode* GetParent() { return m_Parent; }
	void SetParent(CRNode* node) { m_Parent = node; }


protected:
	OsgNode m_node;
	CRNode* m_Parent;
};


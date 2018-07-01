#include "stdafx.h"
#include "ImpRGeometryNode.h"

namespace ViewerInterop {
	ImpRGeometryNode::ImpRGeometryNode(CRGeometryNode * node) : ImpRNode(node)
	{
		m_nodeType = RNodeType::Geometry;
	}
	/*ImpRGeometryNode::ImpRGeometryNode(CRGeometryNodeWrapper^ node) : ImpRNode(gcnew CRNodeWrapper(node->GetNativeObject()))
	{
	}*/
}
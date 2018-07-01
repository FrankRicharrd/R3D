#pragma once
#include "ImpRNode.h"
namespace ViewerInterop {
	public ref class ImpRGeometryNode :
		public ImpRNode, IRGeometryNode
	{
	public:
		//ImpRGeometryNode(CRGeometryNodeWrapper^ node);
		ImpRGeometryNode(CRGeometryNode* node);
	};
}

#pragma once

#include "ImpRNode.h"
namespace ViewerInterop {
	public ref class ImpRTransformNode :
		public ImpRNode, IRTransformNode
	{
	public:
		//ImpRTransformNode(CRTransformNodeWrapper^ node);
		ImpRTransformNode(CRTransformNode* node);


		// Inherited via IRTransformNode
		virtual void AddChild(IRNode ^childNode) sealed;

		// Inherited via IRTransformNode
		virtual IVec3 ^ GetTranslation() sealed;

		virtual void SetTranslation(IVec3 ^vec) sealed;


		


		// Inherited via IRTransformNode
		virtual property System::Collections::Generic::List<Viewer::Interfaces::IRNode ^> ^ Children;


		// Inherited via IRTransformNode
		virtual void RemoveChild(Viewer::Interfaces::IRNode ^node);

	};

}


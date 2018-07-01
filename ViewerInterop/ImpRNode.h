#pragma once
#include "WrapperObjects.h"
#include "InteropUtilities.h"
namespace ViewerInterop {
	public ref  class ImpRNode : public IRNode
	{
	public:
		//ImpRNode(CRNodeWrapper^ node);
		ImpRNode(CRNode* node);
		CRNode* GetCNode() { return m_node; }
		// Inherited via IRNode
		virtual System::String ^ __clrcall GetName() sealed;

		virtual void __clrcall SetName(System::String ^name) sealed;

		virtual void SetPolygonMode(int mode);

		virtual property System::String ^ Name
		{
			String^ get() sealed { return GetName(); };
			void set(String^ s) sealed { SetName(s); };
		}
		virtual System::Object ^ GetWrappedNativeObject();
	








		// Inherited via IRNode


		// Inherited via IRNode


		// Inherited via IRNode


		// Inherited via IRNode
		virtual property Viewer::Interfaces::IRNode ^ Parent;

		virtual property RNodeType NodeType
		{
			RNodeType get() sealed { return m_nodeType; };
		}

	protected:
		CRNode* m_node;
		RNodeType m_nodeType;



		// Inherited via IRNode

	};

}
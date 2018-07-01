#include "stdafx.h"
#include "ImpRNode.h"
namespace ViewerInterop {
	/*ImpRNode::ImpRNode(CRNodeWrapper^ node)
	{
		m_node = node->GetNativeObject();
	}*/

	ImpRNode::ImpRNode(CRNode * node)
	{
		m_node = node;
		
	}

	System::String ^ ImpRNode::GetName()
	{
		return gcnew String(m_node->GetName().c_str());
	}



	void ImpRNode::SetName(System::String ^name)
	{
		string nameStr;
		InteropUtilities::MarshalString(name, nameStr);
		m_node->SetName(nameStr);
	}

	void ImpRNode::SetPolygonMode(int mode)
	{
		m_node->SetPolygonMode((ViewerCore::RPolygonMode)mode);
	}

	System::Object ^ ImpRNode::GetWrappedNativeObject()
	{
		CRNodeWrapper^ wrapper = gcnew CRNodeWrapper(m_node);
		return wrapper;
	}

}
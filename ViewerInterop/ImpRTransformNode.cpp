#pragma make_public(CRTransformNode)
#pragma make_public(CRNode)

#include "stdafx.h"
#include "ImpRTransformNode.h"

namespace ViewerInterop {
	/*ImpRTransformNode::ImpRTransformNode(CRTransformNodeWrapper^ node) : ImpRNode(gcnew CRNodeWrapper(node->GetNativeObject()))
	{

	}
*/
	ImpRTransformNode::ImpRTransformNode(CRTransformNode * node) : ImpRNode(node)
	{
		m_nodeType = RNodeType::Transform;
		Children = gcnew System::Collections::Generic::List<Viewer::Interfaces::IRNode ^>();
	}

	void ImpRTransformNode::AddChild(IRNode ^childNode)
	{
		childNode->Parent = this;

		Children->Add(childNode);

		CRNodeWrapper^ wrapper = (CRNodeWrapper^)childNode->GetWrappedNativeObject() ;

		if (wrapper == nullptr)
			return;

		m_node->asTransformNode()->AddChild(wrapper->GetNativeObject());
	}



	IVec3 ^ ImpRTransformNode::GetTranslation()
	{
		osg::Vec3d v = m_node->asTransformNode()->GetTranslation();

		return InteropUtilities::ConvertVec(v);

	}

	void ImpRTransformNode::SetTranslation(IVec3^ vec)
	{
		osg::Vec3d v = InteropUtilities::ConvertVec(vec);

		m_node->asTransformNode()->SetTranslation(v);

	}

	void ImpRTransformNode::RemoveChild(Viewer::Interfaces::IRNode ^node)
	{
		CRNodeWrapper^ wrapper = (CRNodeWrapper^)node->GetWrappedNativeObject();

		m_node->asTransformNode()->RemoveChild(wrapper->GetNativeObject());

		Children->Remove(node);
	}

	//System::String ^ ImpRTransformNode::GetName()
	//{
	//	return ImpRNode::GetName();
	//}
	//
	//void ImpRTransformNode::SetName(System::String ^name)
	//{
	//	ImpRNode::SetName(name);
	//}
}
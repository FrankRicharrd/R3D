#include "stdafx.h"
#include "RSelectionManagerInterop.h"
#include "ImpRTransformNode.h"
using namespace System;
using namespace System::Runtime::InteropServices;
namespace ViewerInterop {

	

	RSelectionManagerInterop::RSelectionManagerInterop(SelectionManager* sel)
	{
		m_selectionManager = sel;
	}

	void RSelectionManagerInterop::SubscribeTransformNodeSelected(OnTransformNodeSelected ^ callback)
	{

		try
		{
			m_OnTransformNodeSelected = callback;

			m_ontransformNodeselectedNotifier += gcnew NotifyTransformNodeSelected(this, &RSelectionManagerInterop::NotifyOnTransformNodeSelected);

			pin_ptr<NotifyTransformNodeSelected^> tmp = &m_ontransformNodeselectedNotifier;

			m_selectionManager->NotifyOnTransformNodeSelected = (void(__cdecl *)(void))(Marshal::GetFunctionPointerForDelegate(m_ontransformNodeselectedNotifier).ToPointer());

		}
		catch (const std::exception& ex)
		{

		}

	}

	void RSelectionManagerInterop::NotifyOnTransformNodeSelected()
	{
		osg::ref_ptr<osg::MatrixTransform> node = m_selectionManager->GetSelectedTransformNode();

		if (!node.valid())
			return;

		CRTransformNode* tnode = new CRTransformNode(node);

		ImpRTransformNode^ transNode = gcnew ImpRTransformNode(tnode);

		m_OnTransformNodeSelected(transNode);
	}


}
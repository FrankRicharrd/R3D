#include "stdafx.h"
#include "PickHandler.h"
#include "SelectionManager.h"
#include "ExtraGeometries.h"
#include "FGlobalData.h"

SelectionManager::SelectionManager(PickHandler* picker, FGlobalData* data) 
{
	m_picker = picker;
	m_data = data;
	m_seltype = SelectionType::TransformNode;
}





SelectionManager::~SelectionManager()
{
}

void SelectionManager::ClearSelection()
{
	//not thread safe
	return;

	if (!m_picker.valid())
		return;

	m_picker->ClearCurrentSelection();

}

bool SelectionManager::CheckIsValidSelectionObject(osg::Node * node)
{
	

	SpecialGeometry* obj = dynamic_cast<SpecialGeometry*>(node);

	if (obj)
	{

		return false;

	}
	return true;

}

osg::ref_ptr<osg::MatrixTransform> SelectionManager::GetSelectedTransformNode()
{
	return m_data->GetSelectedNode();
}



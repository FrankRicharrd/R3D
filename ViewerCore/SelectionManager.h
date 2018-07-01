#pragma once
#include "PickHandler.h"

class VIEWER_API SelectionManager : public osg::Referenced
{
public:
	 enum SelectionType
	{
		TransformNode,
		GeometryNode,
		IntelligentObject
	};

public:
	SelectionManager(PickHandler* picker, FGlobalData* data);
	~SelectionManager();

	PickHandler* GetPicker() { return m_picker; }

	void ClearSelection();

	bool CheckIsValidSelectionObject(osg::Node* node);

	SelectionType GetSelectionType() { return m_seltype; }
	void SetSelectionType(SelectionType sel) { m_seltype = sel; }

	void(*NotifyOnTransformNodeSelected)(void);

	osg::ref_ptr<osg::MatrixTransform> GetSelectedTransformNode();


private: 
	osg::ref_ptr<PickHandler> m_picker;

	FGlobalData* m_data;

	SelectionType m_seltype;

};




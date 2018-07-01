#pragma once
class PickHandler : public  osgGA::GUIEventHandler
{
public:
	PickHandler(FGlobalData* data);
	~PickHandler();


public:
	osg::Node* getOrCreateSelectionBox();
	virtual bool handle(const osgGA::GUIEventAdapter& ea,
		osgGA::GUIActionAdapter& aa);

	void ClearCurrentSelection();

private:
	bool TransformNodeSelection(float x, float y);

	bool IntelligentObjectSelection(float x, float y);
	
	void SetSelected3DPoint(float x, float y);

protected:
	
	osg::ref_ptr<osg::MatrixTransform> _selectionBox;
	FGlobalData* m_data;
	bool m_visible;

	osg::ref_ptr<osg::Node> _selectednode;

};

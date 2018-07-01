#pragma once

#include "AuxiliaryViewUpdater.h"

class VIEWER_API ViewController
{
public:
	ViewController(FGlobalData* data);
	~ViewController();

	void ToggleView(ViewerCore::FViewType type);
	osgViewer::View* create2DView(int x, int y, int width, int height,
		const osg::Vec3& lookDir, const osg::Vec3& up,
		osg::GraphicsContext* gc, osg::Node* scene, osg::Vec4 _backgroundColor);
	void SetHomeFocus();

	void SetFocus(CRNode* node);

private: 
	FGlobalData* m_data;

};


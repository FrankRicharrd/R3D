#pragma once

#include "stdafx.h"
#include <windows.h>
#include <process.h>


//osg::ref_ptr<Fviewer> g_viewer;
//bool g_finished;
//bool initComplete = false;
//
//
//void MultiThreadRender(void*)
//{
//	while (!g_viewer->Done())
//		g_viewer->Frame();
//	g_finished = true;
//}


osg::ref_ptr<osgViewer::CompositeViewer> g_viewer;
bool g_finished;
bool initComplete = false;


void MultiThreadRender(void*)
{
	while (!g_viewer->done())
		g_viewer->frame();
	g_finished = true;
}


#include "ViewController.h"
#include "RTransformNode.h"
class VIEWER_API ViewerManager
{
public:
	ViewerManager();
	~ViewerManager();
	void DestroyCanvas();
	void CreateCanvas(void* hwnd, int width, int height);
	void Render();
	osgGA::EventQueue* getEventQueue() const;
	void mouseMoveEvent(ViewerCore::MouseButton btnType, double x , double y );
	void mousePressEvent(ViewerCore::MouseButton btnType, double x, double y);
	void mouseReleaseEvent(ViewerCore::MouseButton btnType, double x, double y);
	void mouseWheelEvent(int delta, double x, double y);
	


	ViewController* GetViewController() { return m_viewController; }

	
	FGlobalData* Getdata() { return _data; };

	CRTransformNode* CreateTransformNode(string name);

	void SetDraggerState(ViewerCore::DraggerType dragger, bool ischecked);
	

public:
	FGlobalData* _data;

	bool _selectionActive = false;
	bool _selectionFinished;

	double _selectionStart_X;
	double _selectionEnd_X;
	double _selectionStart_Y;
	double _selectionEnd_Y;




	bool _enableMultiThread = false;



	ViewController* m_viewController;

};


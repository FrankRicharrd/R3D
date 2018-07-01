#pragma once
#include "stdafx.h"
#include <osg/ref_ptr>
#include <osgViewer/api/win32/GraphicsWindowWin32>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>
#include "FileSystemsManager.h"
#include "RTransformNode.h"
#include "DraggerManager.h"
#include "SelectionManager.h"
#include "RDrawingEntity.h"

class RModelingManager;
class RMaterialManager;
class DraggerManager;
class RLightsManager;

class  FGlobalData
{
public:
	FGlobalData();
	~FGlobalData();
	//void SetViewer(Fviewer* viewer);





	osgViewer::CompositeViewer* GetViewer() { return m_viewer; }
	void SetViewer(osgViewer::CompositeViewer* viewer) { m_viewer = viewer; }

	osgViewer::View* GetActiveView()
	{
		return m_activeView;
	}
	void SetActiveView(osgViewer::View* activeView)
	{
		m_activeView = activeView;
	}

	osgViewer::View* GetMainView() { return m_mainview; }
	void SetMainView(osgViewer::View* mainview) { m_mainview = mainview; }

	osgViewer::GraphicsWindowWin32* GetGraphicsWindow() { return m_graphicsWindow; }
	void SetGraphicsWindow(osg::ref_ptr<osgViewer::GraphicsWindowWin32> graphics) { m_graphicsWindow = graphics; }

	//void SetRootNode(osg::Group* rnode) { m_rootnode = rnode; }

	osg::Vec4 GetBackgroundColor() { return m_backgroundColor; }
	void SetBackgroundColor(osg::Vec4 color) { m_backgroundColor = color; }


	osgGA::TrackballManipulator* GetTrackManipulator() { return m_trackManipulator; }
	void SetTrackManipulator(osgGA::TrackballManipulator* mani) { m_trackManipulator= mani; }

	FileSystemsManager* GetFileSystemManager() { return m_filesystemManager; };





	osg::MatrixTransform* GetSelectedNode() { return m_selectednode; }
	void SetSelectedNode(osg::MatrixTransform* selNOde);


	SelectionManager* GetSelectionManager() { return m_SelectionManager; }
	void SetSelectionManager(SelectionManager* sel) { m_SelectionManager = sel; }


	osg::Node* GetGrid() { return m_grid; }

	void SetGrid(OsgNode node) { m_grid = node; }


	//osg::Node* GetActiveDragger() { return m_activeDragger; }
	//void SetActiveDragger(osg::Node* node) { m_activeDragger = node; };
	//void AddDraggerToParentRenderer(osg::Node* node);
	//void RemoveDraggerFromParentRenderer();
	//void SetActiveDraggerToRendererParent();



	//void SetDraggerType(ViewerCore::DraggerType d) { if (m_activeDraggerType != d) { m_IsDraggerCreated = false;};  m_activeDraggerType = d; }
	//void SetDraggerEnabled(bool ischecked) { m_IsDraggerActive = ischecked; }


	DraggerManager* GetDragger();
	
	osg::Matrix GetModelViewProjectionMatrix();

	RModelingManager* GetModelingManager() { return m_modelingmanager; }
	void SetModelingManager(RModelingManager* sel) { m_modelingmanager = sel; }
	RMaterialManager* GetMaterialManager() { return m_materialmanager; }
	void SetMaterialManager(RMaterialManager* sel) { m_materialmanager = sel; }
	RLightsManager* GetLightsManager() { return m_LightsManager; }
	void SetlightsManager(RLightsManager* sel) { m_LightsManager = sel; }

	//geometry root node inside rendering rootparent
	RGeomInstanceRef GetGeomRootNode() { return m_rootGeometryNode; }
	void SetGeomRootNode(RGeomInstanceRef node) { m_rootGeometryNode = node; }

	//base rootnode inside scene
	osg::Node* GetRenderingRootParent() { return m_RenderingParentNode; }

	void SetRenderingRootParent(OsgNode node) { m_RenderingParentNode = node; }

	CRTransformNode* GetRootParentNode() { return m_RootParentNode; }

	OsgMatrixTransform GetRootParentNodeAsTransformNode() { return m_RootParentNode->GetOsgNode()->asTransform()->asMatrixTransform(); }

	void SetRootParentNode(OsgMatrixTransform node) { m_RootParentNode = new CRTransformNode(node); }

	//rootnode inside rendering rootparent
	osg::Group* GetRootNode() { return m_RootParentNode->GetOsgNode()->asGroup(); }

	osg::ref_ptr<osg::Camera> GetCamera() { return _camera; }

public:

	osg::ref_ptr<osg::Camera> _camera;



	osg::ref_ptr<osg::MatrixTransform> _rootTransform;

	osg::ref_ptr<osg::Camera> _axisCamera;



	double _nearPlane = 1.f;
	double _farPlane = 1000.f;
	double _fovy = 30.f;
	int _width = 0;
	int _height = 0;
	double _aspectRatio = (double)_width / (double)_height;
	//Fviewer* _viewer;



private:
	osg::ref_ptr<osgGA::TrackballManipulator> m_trackManipulator;
	osgViewer::CompositeViewer* m_viewer;
	osg::ref_ptr<osgViewer::View> m_mainview;
	osg::ref_ptr<osgViewer::View> m_activeView;
	osg::ref_ptr<osgViewer::GraphicsWindowWin32> m_graphicsWindow;
	osg::ref_ptr<osg::Group> m_rootnode;
	//osg::Vec4 m_backgroundColor = osg::Vec4(0.9f, 0.9f, 0.9f, 1.f);
	osg::Vec4 m_backgroundColor = osg::Vec4(1, 1, 1, 1);
	FileSystemsManager* m_filesystemManager;

	RModelingManager* m_modelingmanager;
	RMaterialManager* m_materialmanager;
	RLightsManager* m_LightsManager;

	CRTransformNode* m_RootParentNode;
	OsgNode m_RenderingParentNode;

	RGeomInstanceRef m_rootGeometryNode;
	

	osg::ref_ptr<osg::MatrixTransform> m_selectednode;


	osg::ref_ptr< DraggerManager> m_dragger;

	osg::ref_ptr<SelectionManager> m_SelectionManager;

	osg::ref_ptr<osg::Node> m_grid;



};


#include "stdafx.h"
#include "ViewerManager.h"
#include "PickHandler.h"
#include "SelectionManager.h"
#include "ExtraGeometries.h"
#include "RLightsManager.h"
#include "RModelingManager.h"

ViewerManager::ViewerManager()
{
	_data = new FGlobalData();

	_enableMultiThread = true; // currently single thread not working

	m_viewController = new ViewController(_data);
}


ViewerManager::~ViewerManager()
{
	DestroyCanvas();
}

void ViewerManager::DestroyCanvas()
{
	if (_enableMultiThread)
	{
		g_viewer->setDone(true);
		while (!g_finished) Sleep(10);
	}
}

static int ReceivesShadowTraversalMask = 0x1;
static int CastsShadowTraversalMask = 0x2;


osg::ref_ptr<osgShadow::ShadowedScene> CreateShadowScene(osg::Node* childNode)
{
	osg::ref_ptr<osgShadow::ShadowedScene> shadowedScene = new osgShadow::ShadowedScene;

	osgShadow::ShadowSettings* settings = shadowedScene->getShadowSettings();
	settings->setReceivesShadowTraversalMask(ReceivesShadowTraversalMask);
	settings->setCastsShadowTraversalMask(CastsShadowTraversalMask);

	//shadow techniques (change them for good results.
	osg::ref_ptr<osgShadow::ShadowMap> sm = new osgShadow::ShadowMap;
	shadowedScene->setShadowTechnique(sm.get());
	shadowedScene->addChild(childNode);

	osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints;
	hints->setDetailRatio(2.0f);
	osg::ref_ptr<osg::ShapeDrawable> shape;
	shape = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f,-2.0f, 0.0f), 100, 0.1f, 100.f), hints.get());
	shape->setColor(osg::Vec4(0.5f, 0.5f, 0.7f, 1.0f));
	shape->setName("base box");

	osg::ref_ptr<osg::Geode> geode_1 = new osg::Geode();
	geode_1->addDrawable(shape.get());
	shadowedScene->addChild(geode_1);




	osg::Vec4 lightpos(50.0, 100.0, 50.0, 0.0);
	osg::ref_ptr<osg::LightSource> ls = new osg::LightSource;
	ls->getLight()->setPosition(lightpos);
	ls->getLight()->setAmbient(osg::Vec4(0.2, 0.2, 0.2, 1.0));
	ls->getLight()->setDiffuse(osg::Vec4(0.8, 0.8, 0.8, 1.0));
	shadowedScene->addChild(ls.get());

	return shadowedScene;
}



void ViewerManager::CreateCanvas(void* voidhwnd, int width, int height)
{
	_data->_width = width;
	_data->_height = height;

	HWND hWnd = static_cast<HWND>(voidhwnd);


	osg::ref_ptr<osg::Referenced> windata =
		new osgViewer::GraphicsWindowWin32::WindowData(hWnd);

	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->x = 0;
	traits->y = 0;
	traits->width = width;
	traits->height = height;
	traits->windowDecoration = false;
	traits->doubleBuffer = true;
	traits->inheritedWindowData = windata;

	_data->SetGraphicsWindow(new osgViewer::GraphicsWindowWin32(traits.get()));

	_data->_camera = new osg::Camera;
	_data->_camera->setGraphicsContext(_data->GetGraphicsWindow());
	_data->_camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	_data->_camera->setClearMask(GL_DEPTH_BUFFER_BIT |
		GL_COLOR_BUFFER_BIT);
	_data->_camera->setClearColor(_data->GetBackgroundColor());

	_data->_camera->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

	_data->_aspectRatio = (double)traits->width / (double)traits->height;
	_data->_camera->setProjectionMatrixAsPerspective(_data->_fovy, _data->_aspectRatio, _data->_nearPlane, _data->_farPlane);

	g_viewer = new  osgViewer::CompositeViewer();


	_data->SetViewer(g_viewer);

	osg::ref_ptr<osg::Group> rgroupNode = new osg::Group();


	_data->SetRenderingRootParent(rgroupNode);

	OsgMatrixTransform groupNode = new osg::MatrixTransform();

	_data->SetRootParentNode(groupNode);

	_data->GetRenderingRootParent()->asGroup()->addChild(_data->GetRootNode());



	g_viewer->setKeyEventSetsDone(0);

	_data->SetTrackManipulator(new osgGA::TrackballManipulator);


	_data->GetTrackManipulator()->setAllowThrow(false);



	osg::ref_ptr<osgViewer::View> mainview = new osgViewer::View();
	mainview->setCamera(_data->_camera.get());




	mainview->setSceneData(_data->GetRenderingRootParent());

	_data->GetTrackManipulator()->setHomePosition(osg::Vec3d(50, 50, 50), osg::Vec3d(0, 0, 0), osg::Vec3d(0, 1, 0));


	mainview->setCameraManipulator(_data->GetTrackManipulator());
	mainview->addEventHandler(new osgViewer::StatsHandler);

	//mainview->getEventQueue()->keyPress('s');
	//mainview->getEventQueue()->keyPress('s');

	//_data->SetBackgroundColor(osg::Vec4(0, 0, 0, 1));
	//_data->_camera->setClearColor(_data->GetBackgroundColor());



	osg::ref_ptr<osgGA::StateSetManipulator> statesetManipulator = new osgGA::StateSetManipulator(mainview->getCamera()->getStateSet());
	mainview->addEventHandler(statesetManipulator.get());


	//---- selection handler/////
	osg::ref_ptr<PickHandler> picker = new PickHandler(_data);

	osg::ref_ptr<SelectionManager> Sel = new SelectionManager(picker, _data);

	_data->SetSelectionManager(Sel);
	_data->GetRenderingRootParent()->asGroup()->addChild(Sel->GetPicker()->getOrCreateSelectionBox());
	mainview->addEventHandler(picker.get());
	//---- selection handler/////


	_data->SetGrid(Utilities::CreateGrid());

	_data->GetRenderingRootParent()->asGroup()->addChild(_data->GetGrid());


	/*std::string strFileName = "F:\\Coding samples\\OSG\\OpenSceneGraph-Data.git\\trunk\\cessna.osg";

	 osg::Node* node =  osgDB::readNodeFile(strFileName);

	 _data->GetRenderingRootParent()->asGroup()->addChild(node);*/


	_data->SetActiveView(mainview);
	_data->SetMainView(mainview);

	g_viewer->addView(mainview);

	g_finished = false;


	bool enableShadows = true;


	RGeomDefinitionRef mainDef = new RGeomDefinition();
	mainDef->SetName("MainRootParent");
	auto mainInst = mainDef->CreateInstance();
	_data->SetGeomRootNode(mainInst);

	if (enableShadows)
	{
		auto shadowScene = CreateShadowScene(mainInst);
		_data->GetRootNode()->asGroup()->addChild(shadowScene);
		//_data->GetLightsManager()->InitLights(shadowScene);
	}
	else
	{
		_data->GetRootNode()->asGroup()->addChild(mainInst);
		_data->GetLightsManager()->InitLights(_data->GetRootNode()->asGroup());
	}


	if (_enableMultiThread)
	{
		_beginthread(MultiThreadRender, 0, NULL);
	}
	else
	{
		g_viewer->realize();

	}




	initComplete = true;
}

void ViewerManager::Render()
{
	g_viewer->frame();
}

void ViewerManager::mouseMoveEvent(ViewerCore::MouseButton btnType, double x, double y)
{
	if (!initComplete)
	{
		return;
	}

	//if (_selectionActive && btnType & ViewerCore::MouseButton::Left)
	//{
	//	_selectionEnd_X = x;
	//	_selectionEnd_Y = y;

	//	// Ensures that new paint events are created while the user moves the
	//	// mouse.
	//	//this->update();
	//}
	//else
	//{
	this->getEventQueue()->mouseMotion(static_cast<float>(x),
		static_cast<float>(y));
	//}
}

void ViewerManager::mousePressEvent(ViewerCore::MouseButton btnType, double x, double y)
{
	if (!initComplete)
	{
		return;
	}

	// Selection processing
	//if (_selectionActive && btnType == ViewerCore::MouseButton::Left)
	//{
	//	_selectionStart_X = x;
	//	_selectionStart_Y = y;

	//	_selectionEnd_X = _selectionStart_X;
	//	_selectionEnd_Y = _selectionStart_Y;

	//	_selectionFinished = false;           // As long as this is set, the rectangle will be drawn
	//}

	//// Normal processing
	//else
	//{
		// 1 = left mouse button
		// 2 = middle mouse button
		// 3 = right mouse button

//
//	int w = _data->GetGraphicsWindow()->getTraits()->width;
//	int h = _data->GetGraphicsWindow()->getTraits()->height;
//
//	osg::Matrix projMat = _data->GetCamera()->getProjectionMatrix();
//	osg::Matrix viewMat = _data->GetCamera()->getViewMatrix();
//	osg::Matrix projInv = osg::Matrix::inverse(projMat);
//	osg::Matrix viewInv = osg::Matrix::inverse(viewMat);
//
//
//	/*var mX = (event.clientX / jQuery(container).width()) * 2 - 1;
//	var mY = -(event.clientY / jQuery(container).height()) * 2 + 1;
//*/
//	osg::Vec4d ndc;
//	/*ndc[0] = x / w * 2. - 1.;
//	ndc[1] = y / h * 2. - 1.;*/
//	ndc[0] = (x / w) * 2 - 1;
//	ndc[1] = -(y / h) * 2 + 1;
//	ndc[2] = -1.;
//	ndc[3] = 0;
//	
//	auto mat = _data->GetCamera()->getViewport()->computeWindowMatrix() * projInv;
//
//
//	osg::Matrix MVPW(_data->GetCamera()->getViewMatrix() *
//		_data->GetCamera()->getProjectionMatrix() *
//		_data->GetCamera()->getViewport()->computeWindowMatrix());
//
//	osg::Matrixd inverseMVPW = osg::Matrixd::inverse(MVPW);
//
//	osg::Vec3 posIn2D = osg::Vec3(0, 0, 0) * MVPW;
//
//	osg::Vec3 near_point = osg::Vec3(x, y, 0.0f) * inverseMVPW;
//	osg::Vec3 far_point = osg::Vec3(x, y, 1.0f) * inverseMVPW;
//	osg::Vec3 ss = osg::Vec3(x, y, -1.0f) * inverseMVPW;
//
// 
//	//auto e =  mat.ptr();
//
//	//auto w1 = 1 / (e[3] * ndc.x() + e[7] * ndc.y() + e[11] * ndc.z() + e[15]);
//
//	//auto cx = (e[0] * ndc.x() + e[4] * ndc.y() + e[8] * ndc.z() + e[12]) * w1;
//	//auto cy = (e[1] * ndc.x() + e[5] * ndc.y() + e[9] * ndc.z() + e[13]) * w1;
//	//auto cz = (e[2] * ndc.x() + e[6] * ndc.y() + e[10] * ndc.z() + e[14]) * w1;
//
//
//	//osg::Vec3 oo(cx, cy, cz);
//
//
//
//
//
//	
//	osg::Vec4d oc2 = ndc * mat;
//	//osg::Vec4d oc1 = ndc * viewInv * projInv;
//	//osg::Vec4d oc2 = ndc * viewInv* projInv;
//	
//	_data->GetModelingManager()->DrawSphere(osg::Vec3(oc2.x(), oc2.y(), oc2.z()), 5);
//
	unsigned int button = btnType;

	this->getEventQueue()->mouseButtonPress(static_cast<float>(x),
		static_cast<float>(y),
		button);
	//}
}

void ViewerManager::mouseReleaseEvent(ViewerCore::MouseButton btnType, double x, double y)
{
	if (!initComplete)
	{
		return;
	}

	// Selection processing: Store end position and obtain selected objects
	// through polytope intersection.
	//if (_selectionActive && btnType == ViewerCore::MouseButton::Left)
	//{

	//	_selectionEnd_X = x;
	//	_selectionEnd_Y = y;
	//	_selectionFinished = true; // Will force the painter to stop drawing the
	//							   // selection rectangle

	////	this->processSelection(); // write selection logics here
	//}

	//// Normal processing
	//else
	//{
		// 1 = left mouse button
		// 2 = middle mouse button
		// 3 = right mouse button

	unsigned int button = btnType;



	this->getEventQueue()->mouseButtonRelease(static_cast<float>(x),
		static_cast<float>(y),
		button);
	//}
}

void ViewerManager::mouseWheelEvent(int delta, double x, double y)
{
	if (!initComplete)
	{
		return;
	}

	/*if (_selectionActive)
		return;*/

		//accept the event if not working

		//_data->GetTrackManipulator()->setCenter()


	 //auto c = _data->GetTrackManipulator()->getCenter();

	 //_data->GetTrackManipulator()->setCenter(osg::Vec3(250, 250, 250));

	osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0 ? osgGA::GUIEventAdapter::SCROLL_UP
		: osgGA::GUIEventAdapter::SCROLL_DOWN;

	this->getEventQueue()->mouseScroll(motion);




}


CRTransformNode * ViewerManager::CreateTransformNode(string name)
{
	OsgMatrixTransform trans = new osg::MatrixTransform;

	CRTransformNode* transNode = new CRTransformNode(trans);
	transNode->SetName(name);

	return transNode;
}

void ViewerManager::SetDraggerState(DraggerType dragger, bool ischecked)
{

	_data->GetDragger()->SetIsActive(ischecked);
	_data->GetDragger()->SetActiveDraggerType(dragger);

	if (ischecked)
	{
		_data->GetDragger()->SetDraggerToRendererParent();
	}
	else
	{
		_data->GetDragger()->RemoveDraggerFromParentRenderer();
	}
}


osgGA::EventQueue* ViewerManager::getEventQueue() const
{
	osgGA::EventQueue* eventQueue = _data->GetGraphicsWindow()->getEventQueue();

	if (eventQueue)
		return eventQueue;
	else
		throw std::runtime_error("Unable to obtain valid event queue");
}

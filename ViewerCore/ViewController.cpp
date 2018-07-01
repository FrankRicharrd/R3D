#include "stdafx.h"
#include "ViewController.h"
#include "CalculateBoundingBox.h"

ViewController::ViewController(FGlobalData* data)
{
	m_data = data;
}


ViewController::~ViewController()
{
}

osgViewer::View* ViewController::create2DView(int x, int y, int width, int height,
	const osg::Vec3& lookDir, const osg::Vec3& up,
	osg::GraphicsContext* gc, osg::Node* scene, osg::Vec4 _backgroundColor)
{

	width = m_data->GetActiveView()->getCamera()->getViewport()->width();
	height = m_data->GetActiveView()->getCamera()->getViewport()->height();


	osgViewer::View* view = new osgViewer::View;


	view->getCamera()->setClearColor(_backgroundColor);
	view->getCamera()->setGraphicsContext(gc);
	view->getCamera()->setViewport(x, y, width, height);


	view->setSceneData(scene);




	osg::Vec3 center = scene->getBound().center();
	double radius = scene->getBound().radius();
	view->getCamera()->setViewMatrixAsLookAt(center - lookDir*(radius*3.0), center, up);
	view->getCamera()->setProjectionMatrixAsPerspective(
		30.0f, static_cast<double>(width) / static_cast<double>(height), 1.0f, 10000.0f);



	return view;
}

void ViewController::SetHomeFocus()
{
	m_data->GetTrackManipulator()->setHomePosition(osg::Vec3d(100, 100, 100), osg::Vec3d(0, 0, 0), osg::Vec3d(0, 1, 0));

}

void ViewController::SetFocus(CRNode * node)
{

	//osg::BoundingSphere bound = node->GetOsgNode()->computeBound();

	//CalculateBoundingBox bbox;
	//node->GetOsgNode()->accept(bbox);

	//osg::BoundingBox bb = bbox.getBoundBox();

	//if (!bb.valid())
	//	return;

	//osg::Vec3f center = bb.center();

	//float r = bb.radius();



	////m_data->GetTrackManipulator()->setCenter(center);
	//m_data->GetTrackManipulator()->setByMatrix(osg::Matrix::lookAt(osg::Vec3d(100 + r, 0, 0 ), osg::Vec3d(0, 0, 0), osg::Vec3d(0, 1, 0)));
}


void ViewController::ToggleView(ViewerCore::FViewType type)
{

	int vv = (int)type;

	//m_data->SetBackgroundColor(osg::Vec4(0.9f/vv, 0.9f/vv, 0.9f/vv, 1.f));


	if (m_data->GetActiveView() == nullptr)
		return;

	m_data->GetViewer()->removeView(m_data->GetActiveView());


	switch (type)
	{
	case ViewerCore::FViewType::Top:
		m_data->SetActiveView(create2DView(0, 0, 0, 0, -osg::Z_AXIS, osg::Y_AXIS, m_data->GetGraphicsWindow(), m_data->GetRenderingRootParent(), m_data->GetBackgroundColor())); //correct
		break;
	case ViewerCore::FViewType::Bottom:
		m_data->SetActiveView(create2DView(0, 0, 0, 0, osg::Z_AXIS, osg::Y_AXIS, m_data->GetGraphicsWindow(), m_data->GetRenderingRootParent(), m_data->GetBackgroundColor()));

		break;
	case ViewerCore::FViewType::Front:
		m_data->SetActiveView(create2DView(0, 0, 0, 0, osg::Y_AXIS, osg::Z_AXIS, m_data->GetGraphicsWindow(), m_data->GetRenderingRootParent(), m_data->GetBackgroundColor())); //correct

		break;
	case ViewerCore::FViewType::Back:
		m_data->SetActiveView(create2DView(0, 0, 0, 0, -osg::Y_AXIS, osg::Z_AXIS, m_data->GetGraphicsWindow(), m_data->GetRenderingRootParent(), m_data->GetBackgroundColor()));

		break;
	case ViewerCore::FViewType::Left:
		m_data->SetActiveView(create2DView(0, 0, 0, 0, osg::X_AXIS, osg::Z_AXIS, m_data->GetGraphicsWindow(), m_data->GetRenderingRootParent(), m_data->GetBackgroundColor())); //correct

		break;
	case ViewerCore::FViewType::Right:
		m_data->SetActiveView(create2DView(0, 0, 0, 0, -osg::X_AXIS, osg::Z_AXIS, m_data->GetGraphicsWindow(), m_data->GetRenderingRootParent(), m_data->GetBackgroundColor()));
		break;
	case ViewerCore::FViewType::Perspective:
		m_data->SetActiveView(m_data->GetMainView());
		break;
	default:
		break;
	}


	m_data->GetViewer()->addView(m_data->GetActiveView());


	if (type != ViewerCore::FViewType::Perspective)
	{
		m_data->GetActiveView()->addEventHandler(new AuxiliaryViewUpdater);
	}

}


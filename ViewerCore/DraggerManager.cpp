#include "stdafx.h"
#include "DraggerManager.h"
#include "SelectionManager.h"
using namespace ViewerCore;
class FGlobalData;
DraggerManager::DraggerManager(FGlobalData* data) : m_IsActive(false), m_IsInUse(false)
{
	m_data = data;
}


DraggerManager::~DraggerManager()
{
}

osgManipulator::Dragger* DraggerManager::InitDragger(ViewerCore::DraggerType dtype)
{
	osgManipulator::Dragger* dragger = 0;

	switch (dtype)
	{
	case ViewerCore::DraggerType::TabBoxDragger:
	{
		osgManipulator::TabBoxDragger* d = new osgManipulator::TabBoxDragger();
		d->setupDefaultGeometry();
		d->addConstraint(new PlaneConstraint());
		dragger = d;
	}
	break;
	case ViewerCore::DraggerType::TabPlaneDragger:
	{
		osgManipulator::TabPlaneDragger* d = new osgManipulator::TabPlaneDragger();
		d->setupDefaultGeometry();
		d->addConstraint(new PlaneConstraint());
		dragger = d;
	}
	break;
	case ViewerCore::DraggerType::TabBoxTrackballDragger:
	{
		osgManipulator::TabBoxTrackballDragger* d = new osgManipulator::TabBoxTrackballDragger();
		d->setupDefaultGeometry();
		dragger = d;
	}
	break;
	case ViewerCore::DraggerType::TrackballDragger:
	{
		osgManipulator::TrackballDragger* d = new osgManipulator::TrackballDragger();
		d->setupDefaultGeometry();
		//d->setAxisLineWidth(5.0f);
		//d->setPickCylinderHeight(0.1f);
		dragger = d;
	}
	break;
	case ViewerCore::DraggerType::Translate1DDragger:
	{
		osgManipulator::Translate1DDragger* d = new osgManipulator::Translate1DDragger();
		d->setupDefaultGeometry();
		dragger = d;
	}
	break;
	case ViewerCore::DraggerType::Translate2DDragger:
	{
		osgManipulator::Translate2DDragger* d = new osgManipulator::Translate2DDragger();
		d->setupDefaultGeometry();

		dragger = d;
	}
	break;
	case ViewerCore::DraggerType::TranslateAxisDragger:
	{
		osgManipulator::TranslateAxisDragger* d = new osgManipulator::TranslateAxisDragger();
		d->setupDefaultGeometry();
		d->setAxisLineWidth(5.0f);
		d->setPickCylinderRadius(0.05f);
		d->setConeHeight(0.2f);
		dragger = d;

	}
	break;
	case ViewerCore::DraggerType::TabPlaneTrackballDragger:
	{
		osgManipulator::TabPlaneTrackballDragger* d = new osgManipulator::TabPlaneTrackballDragger();
		d->setupDefaultGeometry();
		dragger = d;
	}
	break;

	case ViewerCore::DraggerType::TranslatePlaneDragger:
	{
		osgManipulator::TranslatePlaneDragger* d = new osgManipulator::TranslatePlaneDragger();
		d->setupDefaultGeometry();
		dragger = d;
	}
	break;
	case ViewerCore::DraggerType::Scale1DDragger:
	{
		osgManipulator::Scale1DDragger* d = new osgManipulator::Scale1DDragger();
		d->setupDefaultGeometry();
		dragger = d;
	}
	break;
	case ViewerCore::DraggerType::Scale2DDragger:
	{
		osgManipulator::Scale2DDragger* d = new osgManipulator::Scale2DDragger();
		d->setupDefaultGeometry();
		dragger = d;
	}
	break;
	case ViewerCore::DraggerType::RotateCylinderDragger:
	{
		osgManipulator::RotateCylinderDragger* d = new osgManipulator::RotateCylinderDragger();
		d->setupDefaultGeometry();
		dragger = d;
	}
	break;
	case ViewerCore::DraggerType::RotateSphereDragger:
	{
		osgManipulator::RotateSphereDragger* d = new osgManipulator::RotateSphereDragger();
		d->setupDefaultGeometry();
		dragger = d;
	}
	break;

	default:
	{
		osgManipulator::TabBoxDragger* d = new osgManipulator::TabBoxDragger();
		d->setupDefaultGeometry();
		dragger = d;
	}
	break;
	}

	return dragger;
}

osg::Node* DraggerManager::CreateDraggerAndSetToData(osg::MatrixTransform* scene, ViewerCore::DraggerType dragType, bool fixedSizeInScreen)
{
	scene->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);


	osg::Node* draggerNodeToAdd;

	osgManipulator::Dragger* dragger = InitDragger(dragType);

	if (fixedSizeInScreen)
	{
		DraggerContainer* draggerContainer = new DraggerContainer;
		draggerContainer->setDragger(dragger);
		draggerNodeToAdd = draggerContainer;
	}
	else
	{
		draggerNodeToAdd = dragger;
	}

	float scale = scene->getBound().radius() * 1.6;
	osg::Vec3f center = scene->getBound().center();

	osg::Matrixd mat = osg::Matrix::scale(scale, scale, scale) * osg::Matrix::translate(center);

	dragger->setMatrix(mat);

	if (dynamic_cast<osgManipulator::TabPlaneDragger*>(dragger))
	{
		dragger->addTransformUpdating(scene, osgManipulator::DraggerTransformCallback::HANDLE_TRANSLATE_IN_LINE);
	}
	else
	{
		dragger->addTransformUpdating(scene);
	}

	// we want the dragger to handle it's own events automatically
	dragger->setHandleEvents(true);

	// if we don't set an activation key or mod mask then any mouse click on
	// the dragger will activate it, however if do define either of ActivationModKeyMask or
	// and ActivationKeyEvent then you'll have to press either than mod key or the specified key to
	// be able to activate the dragger when you mouse click on it.  Please note the follow allows
	// activation if either the ctrl key or the 'a' key is pressed and held down.
	/*dragger->setActivationModKeyMask(osgGA::GUIEventAdapter::MODKEY_CTRL);
	dragger->setActivationKeyEvent('a');

	dragger->addEventCallback(new DraggerEventHandler());
	*/
	dragger->setDraggerActive(true);
	//dragger->setActivationMouseButtonMask(osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON);
	//dragger->addEventCallback(new DraggerEventHandler());

	//return root;

	dragger->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);


	return draggerNodeToAdd;
}

void DraggerManager::AddDraggerToParentRenderer(osg::Node * node)
{
	m_activeDragger = node;
	m_data->GetRenderingRootParent()->asGroup()->addChild(m_activeDragger);
	m_IsInUse = true;
	m_data->GetSelectionManager()->ClearSelection();
}
void DraggerManager::RemoveDraggerFromParentRenderer()
{
	if (!m_activeDragger.valid())
		return;
	m_data->GetRenderingRootParent()->asGroup()->removeChild(m_activeDragger);

	m_IsInUse = false;
}


void DraggerManager::SetDraggerToRendererParent()
{
	if (!m_IsActive)
		return;
	RemoveDraggerFromParentRenderer();

	if (!m_data->GetSelectedNode())
		return;

	osg::Node* dragger = CreateDraggerAndSetToData(m_data->GetSelectedNode(), m_activeDraggerType, true);

	AddDraggerToParentRenderer(dragger);

}
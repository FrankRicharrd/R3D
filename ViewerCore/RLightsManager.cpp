#include "stdafx.h"
#include "RLightsManager.h"



RLight::RLight()
{
}

RLight::RLight(osg::ref_ptr<osg::Light> light, osg::Vec3 position, osg::StateSet* state) : m_light(light)
{
	parentState = state;
	lightMarker = new osg::Geode();
	lightSource = new osg::LightSource();

	lightMarker->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(), 1)));
	osg::Material *material = new osg::Material();
	material->setDiffuse(osg::Material::FRONT, osg::Vec4(0.0, 0.0, 0.0, 1.0));
	material->setEmission(osg::Material::FRONT, light->getDiffuse());

	lightMarker->getOrCreateStateSet()->setAttribute(material);

	lightSource->setLight(light);
	lightSource->setLocalStateSetModes(osg::StateAttribute::ON);
	lightSource->setStateSetModes(*state, osg::StateAttribute::ON);

	lightTransform = new osg::PositionAttitudeTransform();
	lightTransform->addChild(lightSource);
	lightTransform->addChild(lightMarker);
	lightTransform->setPosition(position);

	lightTransform->setScale(osg::Vec3(2, 2, 2));

	addChild(lightTransform);

}

inline RLight::RLight(const RLight & p, const osg::CopyOp & copyop) : osg::MatrixTransform(p, copyop)
{
}

void RLight::ON()
{
	GetLightSource()->setStateSetModes(*parentState, osg::StateAttribute::ON);
}

void RLight::OFF()
{
	GetLightSource()->setStateSetModes(*parentState, osg::StateAttribute::OFF);
}

RLight::~RLight()
{
}


osg::ref_ptr<osg::Light>  RLightsManager::createSpotLight(osg::Vec4 color, osg::Vec3 direction)
{

	//data from http://glprogramming.com/red/chapter05.html

	osg::ref_ptr<osg::Light> light = new osg::Light();
	// each light must have a unique number
	light->setLightNum(uniqueLightNumber++);
	// we set the light's position via a PositionAttitudeTransform object
	light->setAmbient(osg::Vec4(0.2, 0.2, 0.2, 1.0));
	light->setDiffuse(color);
	light->setSpecular(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	light->setPosition(osg::Vec4(0.0, 0.0, 0.0, 1.0));
	light->setDirection(direction);
	light->setSpotCutoff(45);
	light->setSpotExponent(2);

	light->setConstantAttenuation(1.5);


	return light.release();
}


osg::ref_ptr<osg::Light>  RLightsManager::createLight(osg::Vec4 color)
{
	osg::ref_ptr<osg::Light> light = new osg::Light();
	// each light must have a unique number
	light->setLightNum(uniqueLightNumber++);
	// we set the light's position via a PositionAttitudeTransform object
	light->setPosition(osg::Vec4(0.0, 0.0, 0.0, 1.0));
	light->setDiffuse(color);
	light->setSpecular(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	light->setAmbient(osg::Vec4(0.0, 0.0, 0.0, 1.0));

	return light.release();
}

osg::Material *RLightsManager::createSimpleMaterial(osg::Vec4 color)
{
	osg::Material *material = new osg::Material();
	material->setDiffuse(osg::Material::FRONT, osg::Vec4(0.0, 0.0, 0.0, 1.0));
	material->setEmission(osg::Material::FRONT, color);

	return material;
}

RLightsManager::RLightsManager(FGlobalData * data) : m_data(data),
m_hasHeadlight(false),
m_hasSkylight(false)
{



}


RLightsManager::~RLightsManager()
{
}

osg::Node* RLightsManager::createLightSource(unsigned int num,
	const osg::Vec3& trans,
	const osg::Vec4& color)
{
	osg::ref_ptr<osg::Light> light = new osg::Light;
	light->setLightNum(num);
	light->setDiffuse(color);
	light->setPosition(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));

	osg::ref_ptr<osg::LightSource> lightSource = new
		osg::LightSource;
	lightSource->setLight(light);

	osg::ref_ptr<osg::MatrixTransform> sourceTrans =
		new osg::MatrixTransform;
	sourceTrans->setMatrix(osg::Matrix::translate(trans));
	sourceTrans->addChild(lightSource.get());
	return sourceTrans.release();

}

void RLightsManager::SwitchMainLight(bool enable)
{
	if (enable)
	{

		m_data->GetRootNode()->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);

	}
	else
	{

		m_data->GetRootNode()->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	}
}

void RLightsManager::HeadLightSwitch(bool enable)
{
	m_hasHeadlight = enable;
	if (enable)
		m_data->GetMainView()->setLightingMode(osg::View::LightingMode::HEADLIGHT);
	else
	{
		m_data->GetMainView()->setLightingMode(osg::View::LightingMode::NO_LIGHT);

		m_data->GetRootNode()->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);

		if (m_hasSkylight)
			m_data->GetMainView()->setLightingMode(osg::View::LightingMode::SKY_LIGHT);
	}
}

void RLightsManager::SkyLightSwitch(bool enable)
{
	m_hasSkylight = enable;
	if (enable)
		m_data->GetMainView()->setLightingMode(osg::View::LightingMode::SKY_LIGHT);
	else
	{
		m_data->GetMainView()->setLightingMode(osg::View::LightingMode::NO_LIGHT);
		m_data->GetRootNode()->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);

		if (m_hasHeadlight)
			m_data->GetMainView()->setLightingMode(osg::View::LightingMode::HEADLIGHT);
	}
}

void RLightsManager::ExtraLightsSwitch(bool enable)
{
	auto parentNodeState = m_data->GetRootNode()->getOrCreateStateSet();

	for (auto l : m_extraLights)
	{
		if (enable)
		{
			l->ON();
		}
		else
		{
			l->OFF();
		}
	}
}

CameraUpdateCallback::CameraUpdateCallback(FGlobalData* data) : m_data(data)
{
}

void CameraUpdateCallback::operator()(osg::RenderInfo & renderInfo) const
{
}

void CameraUpdateCallback::operator()(const osg::Camera & cam) const
{
	RLightCollection lights = m_data->GetLightsManager()->GetLightsWRTCamera();

	osg::Vec3 eye, center, up;

	cam.getViewMatrixAsLookAt(eye, center, up);

	osg::Vec3 norm = center - eye;



	for (auto l : lights)
	{



	}
}

CameraUpdateCallback::~CameraUpdateCallback()
{
}



void RLightsManager::InitLights(osg::ref_ptr<osg::Group> rootNode)
{

	osg::ref_ptr<osg::Group> groupNode = rootNode;
	//groupNode = m_data->GetRootNode()->asGroup();
	{
		osg::Vec3 sp1pos(-500, 500, 500);
		osg::Vec3 sp2pos(500, 500, -500);

		osg::Vec3 center(0, 0, 0);

		osg::Vec3 sp1dir = center - sp1pos;
		osg::Vec3 sp2dir = center - sp2pos;


		RLightRef sp1 = new RLight(createSpotLight(osg::Vec4(1, 1, 1, 1), sp1dir), sp1pos, groupNode->getOrCreateStateSet());
		groupNode->addChild(sp1.get());

		RLightRef sp2 = new RLight(createSpotLight(osg::Vec4(1, 1, 1, 1), sp2dir), sp2pos, groupNode->getOrCreateStateSet());
		groupNode->addChild(sp2.get());

		m_extraLights.push_back(sp1);
		m_extraLights.push_back(sp2);


	}

	RLightRef sp3;
	{
	
		osg::Vec3 sp2pos(500, 500, 500);

		osg::Vec3 center(0, 0, 0);

	
		osg::Vec3 sp2dir = center - sp2pos;

		sp3 = new RLight(createSpotLight(osg::Vec4(1, 1, 1, 1), sp2dir), sp2pos, groupNode->getOrCreateStateSet());
		groupNode->addChild(sp3.get());
		m_extraLights.push_back(sp3);

	}

	SwitchMainLight(true);
}

RLightRef RLightsManager::Createlight(osg::Group * parentNode, osg::Vec4 color, osg::Vec3 position)
{
	bool isModelLight = false;
	osg::Group* groupNode;

	if (parentNode == NULL)
	{
		groupNode = m_data->GetRenderingRootParent()->asGroup();

	}
	else
	{
		groupNode = parentNode;

		isModelLight = true;
	}

	RLightRef light = new RLight(createLight(color), position, groupNode->getOrCreateStateSet());

	groupNode->addChild(light.get());

	if (isModelLight)
		m_modelLights.push_back(light);
	else
		m_OtherLights.push_back(light);

	return light;

}

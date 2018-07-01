#pragma once

class RLight;
typedef osg::ref_ptr<RLight> RLightRef;
typedef std::vector<RLightRef> RLightCollection;

class VIEWER_API RLight : public osg::MatrixTransform
{
public:
	RLight(osg::ref_ptr<osg::Light>, osg::Vec3 position, osg::StateSet* state);
	RLight(const RLight& p, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);

	osg::ref_ptr<osg::LightSource> GetLightSource() { return lightSource; }

	void ON();
	void OFF();


	META_Object(R3DViewer, RLight);

private:



protected:
	~RLight();
	RLight();

	osg::ref_ptr<osg::PositionAttitudeTransform> lightTransform;
	osg::ref_ptr<osg::Geode> lightMarker;
	osg::ref_ptr<osg::LightSource> lightSource;
	osg::ref_ptr<osg::StateSet> parentState;
	osg::ref_ptr<osg::Light> m_light;
};


class CameraUpdateCallback : public osg::Camera::DrawCallback
{
public :
	CameraUpdateCallback(FGlobalData* data);
	

	virtual void operator () (osg::RenderInfo& renderInfo) const;

	/** Functor method, provided for backwards compatibility, called by operator() (osg::RenderInfo& renderInfo) method.*/
	virtual void operator () (const osg::Camera& /*camera*/) const;

private:
	FGlobalData*  m_data;

protected:
	~CameraUpdateCallback();

};




class VIEWER_API RLightsManager
{
public:
	RLightsManager(FGlobalData * data);
	~RLightsManager();

	void SwitchMainLight(bool enable);
	void HeadLightSwitch(bool enable);
	void SkyLightSwitch(bool enable);
	void ExtraLightsSwitch(bool enable);

	void InitLights(osg::ref_ptr<osg::Group> rootNode);

	RLightRef Createlight(osg::Group* parentNode, osg::Vec4 color, osg::Vec3 position);

	RLightCollection GetLightsWRTCamera() { return m_lightsWRTcamera; }

private:
	FGlobalData * m_data;
	bool m_hasHeadlight;
	bool m_hasSkylight;

	osg::ref_ptr<osg::Light> createLight(osg::Vec4 color);
	osg::ref_ptr<osg::Light> createSpotLight(osg::Vec4 color,osg::Vec3 direction);

	

	int uniqueLightNumber = 0;
	
	int const LIGHTS = 3;

	osg::PositionAttitudeTransform *cubeTransform;
	osg::PositionAttitudeTransform *lightTransform[3]; //3 means LIGHTS
	osg::StateSet *lightStateSet;


	RLightCollection m_modelLights;
	RLightCollection m_OtherLights;;

	RLightCollection m_lightsWRTcamera;;


	osg::Node* light0;
	osg::Node* light1;

	std::vector<RLightRef> m_extraLights;

	
	osg::Material *createSimpleMaterial(osg::Vec4 color);

	osg::Node* createLightSource(unsigned int num,
		const osg::Vec3& trans,
		const osg::Vec4& color);
};


#pragma once
#include "RIntelligentObject.h"

typedef osg::ref_ptr<RIOTranslateDragger> RIOTranslateDraggerRef;
typedef std::vector<RIOTranslateDraggerRef> RIOTranslateDraggerCollection;



class R3DEXPORT RIOTranslateDraggerCallback : public osgManipulator::DraggerCallback
{
public:
	enum HandleCommandMask
	{
		HANDLE_TRANSLATE_IN_LINE = 1 << 0,
		HANDLE_TRANSLATE_IN_PLANE = 1 << 1,
		HANDLE_SCALED_1D = 1 << 2,
		HANDLE_SCALED_2D = 1 << 3,
		HANDLE_SCALED_UNIFORM = 1 << 4,
		HANDLE_ROTATE_3D = 1 << 5,
		HANDLE_ALL = 0x8ffffff
	};
public:
	RIOTranslateDraggerCallback(RIntelliDrawableInstanceCollection instances, RIntelliDrawableInstanceCollection instancesFixed, int handleCommandMask = HANDLE_ALL);
	~RIOTranslateDraggerCallback();


	virtual bool receive(const osgManipulator::MotionCommand&);
	virtual bool receive(const osgManipulator::TranslateInLineCommand& command);
	virtual bool receive(const osgManipulator::TranslateInPlaneCommand& command);
	virtual bool receive(const osgManipulator::Scale1DCommand& command);
	virtual bool receive(const osgManipulator::Scale2DCommand& command);
	virtual bool receive(const osgManipulator::ScaleUniformCommand& command);
	virtual bool receive(const osgManipulator::Rotate3DCommand& command);


	bool(*BeforeUpdatingConstraints)(double, double, double);

	bool(*OnRelease)(void);

protected:

	unsigned int _handleCommandMask;

	//osg::observer_ptr<osg::MatrixTransform> _transform;
	osg::Matrix _startMotionMatrix;

	osg::Matrix _localToWorld;
	osg::Matrix _worldToLocal;

	osg::Vec3 _previousTranslation;

private:

	void UpdateDependentEntities(osg::Vec3 translation);
	void SetFixedStatusToInstances(bool isFixed);

	RIntelliDrawableInstanceCollection m_instances;
	RIntelliDrawableInstanceCollection m_instancesFixed;


};





class R3DEXPORT RIOTranslateDragger : public osgManipulator::Dragger
{
public:
	RIOTranslateDragger(osg::Vec3 normal, osg::Vec3 position);

	META_OSGMANIPULATOR_Object(R3DViewer, RIOTranslateDragger);


	/** Handle pick events on dragger and generate TranslateInLine commands. */
	virtual bool handle(const osgManipulator::PointerInfo& pi, const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);

	/** Setup default geometry for dragger. */
	void setupDefaultGeometry();

	/** Set/Get color for dragger. */
	inline void setColor(const osg::Vec4& color) { _color = color; setMaterialColor(_color, *this); }
	inline const osg::Vec4& getColor() const { return _color; }

	/** Set/Get pick color for dragger. Pick color is color of the dragger when picked.
	It gives a visual feedback to show that the dragger has been picked. */
	inline void setPickColor(const osg::Vec4& color) { _pickColor = color; }
	inline const osg::Vec4& getPickColor() const { return _pickColor; }
	inline void setCheckForNodeInNodePath(bool onOff) { _checkForNodeInNodePath = onOff; }

private:
	osg::ref_ptr<osgManipulator::LineProjector >   _projector;
	osg::Vec3d                      _startProjectedPoint;

	osg::Vec4                       _color;
	osg::Vec4                       _pickColor;
	bool _checkForNodeInNodePath;

	
	int m_conesize;
	osg::Vec3 m_Position;
	osg::Vec3 m_normal;


	osg::ref_ptr<osg::Cone> m_cone1;
	osg::ref_ptr<osg::Cone> m_cone2;
	double _draggerSize;

	osg::ref_ptr<osg::MatrixTransform> m_geometryTransform;

	bool m_active;

protected:
	RIOTranslateDragger();
	~RIOTranslateDragger();
};


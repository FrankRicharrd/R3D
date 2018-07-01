#pragma once
//create only one custom dragger then create draggercallbacks for each entity types. and add to the respective dragger.


#include "RDrawingEntity.h"


class VIEWER_API RFaceDragger : public osgManipulator::Dragger
{
public:
	RFaceDragger(RFace* face);



	META_OSGMANIPULATOR_Object(R3DViewer, RFaceDragger);

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

	RFaceRef GetFace() { return m_face; }

	void SetConeSize(int a) { m_conesize = a; }
	//void SetConePosition(osg::Vec3 pos) { m_conePosition = pos; }
	void UpdatePosition();


private:
	osg::ref_ptr<osgManipulator::LineProjector >   _projector;
	osg::Vec3d                      _startProjectedPoint;

	osg::Vec4                       _color;
	osg::Vec4                       _pickColor;
	bool _checkForNodeInNodePath;
	
	RFace* m_face;

	int m_conesize;
	osg::Vec3 m_conePosition;

	osg::ref_ptr<osg::Cone> m_cone1;
	osg::ref_ptr<osg::Cone> m_cone2;
	double _draggerSize;

	osg::ref_ptr<osg::MatrixTransform> m_geometryTransform;

	bool m_active;


protected:
	RFaceDragger();
	virtual ~RFaceDragger();
};







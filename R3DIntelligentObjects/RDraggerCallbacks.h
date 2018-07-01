#pragma once

class RFaceDraggerCallback : public osgManipulator::DraggerCallback
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
	RFaceDraggerCallback(RFace* face, osg::MatrixTransform* transform,  int handleCommandMask= HANDLE_ALL);
	~RFaceDraggerCallback();


//	DraggerTransformCallback(osg::MatrixTransform* transform, int handleCommandMask = HANDLE_ALL);

	virtual bool receive(const osgManipulator::MotionCommand&);
	virtual bool receive(const osgManipulator::TranslateInLineCommand& command);
	virtual bool receive(const osgManipulator::TranslateInPlaneCommand& command);
	virtual bool receive(const osgManipulator::Scale1DCommand& command);
	virtual bool receive(const osgManipulator::Scale2DCommand& command);
	virtual bool receive(const osgManipulator::ScaleUniformCommand& command);
	virtual bool receive(const osgManipulator::Rotate3DCommand& command);

	osg::MatrixTransform* getTransform() { return _transform.get(); }
	const osg::MatrixTransform* getTransform() const { return _transform.get(); }


private:
	RFace* m_face;
protected:

	unsigned int _handleCommandMask;

	osg::observer_ptr<osg::MatrixTransform> _transform;
	osg::Matrix _startMotionMatrix;

	osg::Matrix _localToWorld;
	osg::Matrix _worldToLocal;

	osg::Vec3 _previousTranslation;
	

};


#include "stdafx.h"
#include "RDrawingEntity.h"
#include "RDraggerCallbacks.h"
#include "RIntelligentObject.h"

RFaceDraggerCallback::RFaceDraggerCallback(RFace* face, osg::MatrixTransform* transform, int handleCommandMask) : m_face(face), _handleCommandMask(handleCommandMask)

, _transform(transform)
{
}

RFaceDraggerCallback::~RFaceDraggerCallback()
{
}
bool RFaceDraggerCallback::receive(const osgManipulator::MotionCommand& command)
{
	return false;

	
}


bool RFaceDraggerCallback::receive(const osgManipulator::TranslateInLineCommand& command)
{
	/*if ((_handleCommandMask&HANDLE_TRANSLATE_IN_LINE) != 0) return receive(static_cast<const osgManipulator::MotionCommand&>(command));
	return false;*/

	if (!_transform) return false;

	switch (command.getStage())
	{
	case osgManipulator::MotionCommand::START:
	{
		
		OutputDebugStringA("Inside Start \n");

		// Save the current matrix
		_startMotionMatrix = _transform->getMatrix();

		// Get the LocalToWorld and WorldToLocal matrix for this node.
		osg::NodePath nodePathToRoot;
		osgManipulator::computeNodePathToRoot(*_transform, nodePathToRoot);
		_localToWorld = osg::computeLocalToWorld(nodePathToRoot);
		_worldToLocal = osg::Matrix::inverse(_localToWorld);
		_previousTranslation = osg::Vec3(0, 0, 0);
		return true;
	}
	case osgManipulator::MotionCommand::MOVE:
	{
		OutputDebugStringA("Inside Move \n");

		//OSG_NOTICE<<"MotionCommand::MOVE "<<command.getMotionMatrix()<<std::endl;

		// Transform the command's motion matrix into local motion matrix.
		osg::Matrix localMotionMatrix = _localToWorld * command.getWorldToLocal()
			* command.getMotionMatrix()
			* command.getLocalToWorld() * _worldToLocal;

		// Transform by the localMotionMatrix
		//_transform->setMatrix(localMotionMatrix * _startMotionMatrix);

		 osg::Vec3d trans = command.getTranslation();

		 osg::Vec3 _currentTranslation = osg::Vec3(localMotionMatrix(3, 0), localMotionMatrix(3, 1), localMotionMatrix(3, 2));

		 osg::Vec3 diff = _currentTranslation - _previousTranslation;

		 m_face->TranslateFace(diff);

		 _previousTranslation = _currentTranslation;

		

		return true;
	}
	case osgManipulator::MotionCommand::FINISH:
	{
		RIntelliDrawableDef* def = dynamic_cast<RIntelliDrawableDef*>(m_face->GetDefinition().get());

		if (def != NULL)
		{
			def->UpdateDraggers();
		}

		return true;
	}
	case osgManipulator::MotionCommand::NONE:
	default:
		return false;
	}

}

bool RFaceDraggerCallback::receive(const osgManipulator::TranslateInPlaneCommand& command)
{
	if ((_handleCommandMask&HANDLE_TRANSLATE_IN_PLANE) != 0) return receive(static_cast<const osgManipulator::MotionCommand&>(command));
	return false;
}

bool RFaceDraggerCallback::receive(const osgManipulator::Scale1DCommand& command)
{
	if ((_handleCommandMask&HANDLE_SCALED_1D) != 0) return receive(static_cast<const osgManipulator::MotionCommand&>(command));
	return false;
}

bool RFaceDraggerCallback::receive(const osgManipulator::Scale2DCommand& command)
{
	if ((_handleCommandMask&HANDLE_SCALED_2D) != 0) return receive(static_cast<const osgManipulator::MotionCommand&>(command));
	return false;
}

bool RFaceDraggerCallback::receive(const osgManipulator::ScaleUniformCommand& command)
{
	if ((_handleCommandMask&HANDLE_SCALED_UNIFORM) != 0) return receive(static_cast<const osgManipulator::MotionCommand&>(command));
	return false;
}

bool RFaceDraggerCallback::receive(const osgManipulator::Rotate3DCommand& command)
{
	if ((_handleCommandMask&HANDLE_ROTATE_3D) != 0) return receive(static_cast<const osgManipulator::MotionCommand&>(command));
	return false;
}
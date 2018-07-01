#include "stdafx.h"

#include  "RDrawingEntity.h"
#include "RIntelligentObject.h"
#include "RIOTranslateDragger.h"


RIOTranslateDraggerCallback::RIOTranslateDraggerCallback(RIntelliDrawableInstanceCollection instances, RIntelliDrawableInstanceCollection instancesFixed, int handleCommandMask) : osgManipulator::DraggerCallback(),
m_instancesFixed(instancesFixed),
m_instances(instances), _handleCommandMask(handleCommandMask)
{

}

RIOTranslateDraggerCallback ::~RIOTranslateDraggerCallback()
{
}

bool RIOTranslateDraggerCallback::receive(const osgManipulator::MotionCommand &)
{
	return false;
}

bool RIOTranslateDraggerCallback::receive(const osgManipulator::TranslateInLineCommand & command)
{
	/*if ((_handleCommandMask&HANDLE_TRANSLATE_IN_LINE) != 0) return receive(static_cast<const osgManipulator::MotionCommand&>(command));
	return false;*/

	if (!m_instances[0]) return false;

	switch (command.getStage())
	{
	case osgManipulator::MotionCommand::START:
	{


		OutputDebugStringA("Inside Start \n");
		SetFixedStatusToInstances(true);

		// Save the current matrix
		_startMotionMatrix = m_instances[0]->getMatrix();

		// Get the LocalToWorld and WorldToLocal matrix for this node.
		osg::NodePath nodePathToRoot;
		osgManipulator::computeNodePathToRoot(*m_instances[0], nodePathToRoot);
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

		bool needUpdation = true;

		if (BeforeUpdatingConstraints != NULL)
		{
			needUpdation = BeforeUpdatingConstraints(diff.x(), diff.y(), diff.z());
		}

		if (needUpdation)
			UpdateDependentEntities(diff);

		_previousTranslation = _currentTranslation;



		return true;
	}
	case osgManipulator::MotionCommand::FINISH:
	{
		SetFixedStatusToInstances(false);

		if (OnRelease != NULL)
			OnRelease();


		/*osg::ref_ptr<RIntelliDrawableInstance> instance = new RIntelliDrawableInstance(m_instances[0]->GetIODefinition());
		m_instances[0]->GetIODefinition()->AddInstance(instance);
		m_instances[0]->GetIODefinition()->GetIOObject()->AddInstance(instance);*/

			/*RIntelliDrawableDef* def = dynamic_cast<RIntelliDrawableDef*>(m_face->GetDefinition().get());

			if (def != NULL)
			{
				def->UpdateDraggers();
			}
	*/
			return true;
	}
	case osgManipulator::MotionCommand::NONE:
	default:
		return false;
	}

}

void RIOTranslateDraggerCallback::UpdateDependentEntities(osg::Vec3 translation)
{
	//trigger events here
	for (auto instance : m_instances)
	{
		osg::Matrix curr = instance->getMatrix();
		osg::Matrix res = curr * osg::Matrix::translate(translation);
		instance->setMatrix(res);
		instance->UpdateConstraints();
	}

	//trigger event here
}
void RIOTranslateDraggerCallback::SetFixedStatusToInstances(bool isFixed)
{
	for (auto i : m_instancesFixed)
	{
		i->SetFixMode(isFixed);
	}
}
bool RIOTranslateDraggerCallback::receive(const osgManipulator::TranslateInPlaneCommand & command)
{
	if ((_handleCommandMask&HANDLE_TRANSLATE_IN_PLANE) != 0) return receive(static_cast<const osgManipulator::MotionCommand&>(command));
	return false;
}


bool RIOTranslateDraggerCallback::receive(const osgManipulator::Scale1DCommand & command)
{
	if ((_handleCommandMask&HANDLE_SCALED_1D) != 0) return receive(static_cast<const osgManipulator::MotionCommand&>(command));
	return false;
}

bool RIOTranslateDraggerCallback::receive(const osgManipulator::Scale2DCommand & command)
{
	if ((_handleCommandMask&HANDLE_SCALED_2D) != 0) return receive(static_cast<const osgManipulator::MotionCommand&>(command));
	return false;
}

bool RIOTranslateDraggerCallback::receive(const osgManipulator::ScaleUniformCommand & command)
{
	if ((_handleCommandMask&HANDLE_SCALED_UNIFORM) != 0) return receive(static_cast<const osgManipulator::MotionCommand&>(command));
	return false;
}

bool RIOTranslateDraggerCallback::receive(const osgManipulator::Rotate3DCommand & command)
{
	if ((_handleCommandMask&HANDLE_ROTATE_3D) != 0) return receive(static_cast<const osgManipulator::MotionCommand&>(command));
	return false;
}












///////////////////////////// RIOTranslateDragger///////////////////

RIOTranslateDragger::RIOTranslateDragger(osg::Vec3 normal, osg::Vec3 position) : m_Position(position), m_normal(normal)
{

	int f = 1;

	osg::Vec3 midpoint = position;

	osg::Vec3 n = m_normal;

	osg::Vec3 fn(n.x() * f, n.y()*f, n.z()*f);

	osg::Vec3 s(midpoint.x() - fn.x(), midpoint.y() - fn.y(), midpoint.z() - fn.z());
	osg::Vec3 e(midpoint.x() + fn.x(), midpoint.y() + fn.y(), midpoint.z() + fn.z());

	osg::Vec3 nn = s - e;


	_projector = new osgManipulator::LineProjector(s, e);

	osg::Vec3 lineDir = _projector->getLineEnd() - _projector->getLineStart();
	float lineLength = lineDir.length();
	lineDir.normalize();
	setColor(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	setPickColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	setHandleEvents(true);
	setDraggerActive(true);

}

bool RIOTranslateDragger::handle(const osgManipulator::PointerInfo & pointer, const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa)
{

	// Check if the dragger node is in the nodepath.
	if (_checkForNodeInNodePath)
	{
		if (!pointer.contains(this)) return false;
	}

	switch (ea.getEventType())
	{
		// Pick start.
	case (osgGA::GUIEventAdapter::PUSH):
	{
		m_active = true;

		// Get the LocalToWorld matrix for this node and set it for the projector.
		osg::NodePath nodePathToRoot;
		computeNodePathToRoot(*this, nodePathToRoot);
		osg::Matrix localToWorld = osg::computeLocalToWorld(nodePathToRoot);
		_projector->setLocalToWorld(localToWorld);

		if (_projector->project(pointer, _startProjectedPoint))
		{
			// Generate the motion command.
			osg::ref_ptr<osgManipulator::TranslateInLineCommand> cmd = new osgManipulator::TranslateInLineCommand(_projector->getLineStart(),
				_projector->getLineEnd());
			cmd->setStage(osgManipulator::MotionCommand::START);
			cmd->setLocalToWorldAndWorldToLocal(_projector->getLocalToWorld(), _projector->getWorldToLocal());

			// Dispatch command.
			dispatch(*cmd);

			// Set color to pick color.
			setMaterialColor(_pickColor, *this);

			aa.requestRedraw();

		}
		return true;
	}

	// Pick move.
	case (osgGA::GUIEventAdapter::DRAG):
	{
		osg::Vec3d projectedPoint;
		if (_projector->project(pointer, projectedPoint))
		{
			// Generate the motion command.
			osg::ref_ptr<osgManipulator::TranslateInLineCommand> cmd = new osgManipulator::TranslateInLineCommand(_projector->getLineStart(),
				_projector->getLineEnd());
			cmd->setStage(osgManipulator::MotionCommand::MOVE);
			cmd->setLocalToWorldAndWorldToLocal(_projector->getLocalToWorld(), _projector->getWorldToLocal());
			cmd->setTranslation(projectedPoint - _startProjectedPoint);

			// Dispatch command.
			dispatch(*cmd);

			aa.requestRedraw();
		}
		return true;
	}

	// Pick finish.
	case (osgGA::GUIEventAdapter::RELEASE):
	{
		m_active = false;
		osg::Vec3d projectedPoint;
		if (_projector->project(pointer, projectedPoint))
		{
			osg::ref_ptr<osgManipulator::TranslateInLineCommand> cmd = new osgManipulator::TranslateInLineCommand(_projector->getLineStart(),
				_projector->getLineEnd());

			cmd->setStage(osgManipulator::MotionCommand::FINISH);
			cmd->setLocalToWorldAndWorldToLocal(_projector->getLocalToWorld(), _projector->getWorldToLocal());

			// Dispatch command.
			dispatch(*cmd);

			// Reset color.
			setMaterialColor(_color, *this);

			aa.requestRedraw();


		}


		return true;
	}
	default:
		return false;
	}

	return false;
}

void RIOTranslateDragger::setupDefaultGeometry()
{

	// Get the line length and direction.
	osg::Vec3 lineDir = _projector->getLineEnd() - _projector->getLineStart();
	float lineLength = lineDir.length();
	lineDir.normalize();

	double posFact = (0.10f * m_conesize) * 0.25;

	osg::Vec3 midpoint = m_Position;
	osg::Vec3 n = m_normal;

	m_geometryTransform = new osg::MatrixTransform();
	osg::Geode* geode = new osg::Geode;
	m_geometryTransform->addChild(geode);
	osg::Cone* m_cone1 = new osg::Cone(osg::Vec3(0, 0, 0), 0.025f * 10, 0.10f * 10);


	geode->addDrawable(new osg::ShapeDrawable(m_cone1));


	osg::Quat rotation;

	n.normalize();

	if (n.z() == -1)
	{
		rotation.makeRotate(n, osg::Vec3(0.0f, 0.0f, 1.0f));
	}
	else
	{
		rotation.makeRotate(osg::Vec3(0.0f, 0.0f, 1.0f), n);
	}



	osg::Matrix transMatr = osg::Matrix::translate(midpoint);
	osg::Matrix rotatMat;
	rotatMat.setRotate(rotation);
	osg::Matrix resMat = rotatMat * transMatr;

	m_geometryTransform->setMatrix(resMat);

	setMaterialColor(_color, *this);
	addChild(m_geometryTransform);



}
RIOTranslateDragger::RIOTranslateDragger()
{
}
RIOTranslateDragger::~RIOTranslateDragger()
{
}

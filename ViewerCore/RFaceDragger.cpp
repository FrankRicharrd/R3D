#include "stdafx.h"
#include "RDrawingEntity.h"
#include "RCustomDragger.h"


inline RFaceDragger::RFaceDragger(RFace* face) : osgManipulator::Dragger(), _checkForNodeInNodePath(true), m_face(face), m_conesize(1), _draggerSize(240), m_active(false)
{
	int f = 1;

	osg::Vec3 midpoint = face->GetMidPoint();


	m_conePosition = midpoint;

	osg::Vec3 n = face->GetFaceNormal();

	osg::Vec3 fn(n.x() * f, n.y()*f, n.z()*f);

	osg::Vec3 s(midpoint.x() - fn.x(), midpoint.y() - fn.y(), midpoint.z() - fn.z());
	osg::Vec3 e(midpoint.x() + fn.x(), midpoint.y() + fn.y(), midpoint.z() + fn.z());

	osg::Vec3 nn = s - e;

	_projector = new osgManipulator::LineProjector (s, e);

	osg::Vec3 lineDir = _projector->getLineEnd() - _projector->getLineStart();
	float lineLength = lineDir.length();
	lineDir.normalize();

	//_projector = new osgManipulator::LineProjector(midpoint, osg::Vec3d(midpoint.x() + 1, midpoint.y(), midpoint.z()));

	setColor(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	setPickColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	setHandleEvents(true);
	setDraggerActive(true);


}

RFaceDragger::RFaceDragger()
{
}

bool RFaceDragger::handle(const osgManipulator::PointerInfo & pointer, const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa)
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

void RFaceDragger::setupDefaultGeometry()
{

	int index = m_face->GetIndex();

	// Get the line length and direction.
	osg::Vec3 lineDir = _projector->getLineEnd() - _projector->getLineStart();
	float lineLength = lineDir.length();
	lineDir.normalize();

	double posFact = (0.10f * m_conesize) * 0.25;

	osg::Vec3 midpoint = m_face->GetMidPoint();
	osg::Vec3 n = m_face->GetFaceNormal();

	osg::Vec3 fn(n.x() * posFact, n.y()*posFact, n.z()*posFact);

	osg::Vec3 s(midpoint.x() - fn.x(), midpoint.y() - fn.y(), midpoint.z() - fn.z());
	osg::Vec3 e(midpoint.x() + fn.x(), midpoint.y() + fn.y(), midpoint.z() + fn.z());



	//osg::Geode* geode = new osg::Geode;
	//// Create a left cone.
	//{
	//	m_cone1 = new osg::Cone(s, 0.025f * m_conesize, 0.10f * m_conesize);
	//
	//	osg::Quat rotation;

	//	if (n.z() == 1)
	//		rotation.makeRotate(lineDir, osg::Vec3(0.0f, 0.0f, -1.0f));
	//	else
	//		rotation.makeRotate(lineDir, osg::Vec3(0.0f, 0.0f, 1.0f));
	//	m_cone1->setRotation(rotation);

	//	geode->addDrawable(new osg::ShapeDrawable(m_cone1));
	//}

	//// Create a right cone.
	//{
	//	//double height = (0.10f * m_conesize ) * 0.25;

	//	m_cone2 = new osg::Cone(e, 0.025f * m_conesize, 0.10f * m_conesize);
	//	osg::Quat rotation;
	//	rotation.makeRotate(osg::Vec3(0.0f, 0.0f, 1.0f), lineDir);
	//	m_cone2->setRotation(rotation);

	//	geode->addDrawable(new osg::ShapeDrawable(m_cone2));
	//}

	//setMaterialColor(_color, *this);
	//addChild(geode);

	m_geometryTransform = new osg::MatrixTransform();
	osg::Geode* geode = new osg::Geode;
	m_geometryTransform->addChild(geode);
	osg::Cone* m_cone1 = new osg::Cone(osg::Vec3(0, 0, 0), 0.025f * 10, 0.10f * 10);

	//osg::Cone* m_cone2 = new osg::Cone(osg::Vec3(0, 0, 0), 0.025f * 10, 0.10f * 10);

	//osg::Vec3 coneNormal(1, 0, 0);

	//osg::Quat rotation1;
	//rotation1.makeRotate(coneNormal, osg::Vec3(0.0f, 0.0f, 1.0f));
	//m_cone1->setRotation(rotation1);

	//osg::Quat rotation2;
	//rotation2.makeRotate(osg::Vec3(0.0f, 0.0f, 1.0f), coneNormal);
	//m_cone2->setRotation(rotation2);

	geode->addDrawable(new osg::ShapeDrawable(m_cone1));
	//geode->addDrawable(new osg::ShapeDrawable(m_cone2));



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


	//getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	// Create an invisible cylinder for picking the line.
	//{
	//	osg::Cylinder* cylinder = new osg::Cylinder((_projector->getLineStart() + _projector->getLineEnd()) / 2, 0.015f * lineLength, lineLength);
	//	osg::Quat rotation;
	//	rotation.makeRotate(osg::Vec3(0.0f, 0.0f, 1.0f), lineDir);
	//	cylinder->setRotation(rotation);
	//	osg::Drawable* cylinderGeom = new osg::ShapeDrawable(cylinder);

	//	osgManipulator::setDrawableToAlwaysCull(*cylinderGeom);

	//	geode->addDrawable(cylinderGeom);
	//}

	//osg::Geode* lineGeode = new osg::Geode;
	//// Create a line.
	//{
	//	osg::Geometry* geometry = new osg::Geometry();

	//	osg::Vec3Array* vertices = new osg::Vec3Array(2);
	//	(*vertices)[0] = _projector->getLineStart();
	//	(*vertices)[1] = _projector->getLineEnd();

	//	geometry->setVertexArray(vertices);
	//	geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));

	//	lineGeode->addDrawable(geometry);
	//}

	//geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	// Turn of lighting for line and set line width.
	/*lineGeode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	osg::LineWidth* linewidth = new osg::LineWidth();
	linewidth->setWidth(2.0f);
	lineGeode->getOrCreateStateSet()->setAttributeAndModes(linewidth, osg::StateAttribute::ON);*/


	//getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);


	// Add line and cones to the scene.
	//addChild(lineGeode);

}

void RFaceDragger::UpdatePosition()
{
	return;

	if (m_active)
		return;

	//double posFact = (0.10f * m_conesize) * 0.25;
	osg::Vec3 midpoint = m_face->GetMidPoint();
	osg::Vec3 n = m_face->GetFaceNormal();

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


	//osg::Vec3 n = m_face->GetFaceNormal();

	//osg::Vec3 fn(n.x() * posFact, n.y()*posFact, n.z()*posFact);

	//osg::Vec3 s(midpoint.x() - fn.x(), midpoint.y() - fn.y(), midpoint.z() - fn.z());
	//osg::Vec3 e(midpoint.x() + fn.x(), midpoint.y() + fn.y(), midpoint.z() + fn.z());

	//osg::Quat rotation;

	//if (n.z() == 1)
	//	rotation.makeRotate(n, osg::Vec3(0.0f, 0.0f, -1.0f));
	//else
	//	rotation.makeRotate(n, osg::Vec3(0.0f, 0.0f, 1.0f));
	//m_cone1->setRotation(rotation);

	//osg::Quat rotation1;
	//rotation1.makeRotate(osg::Vec3(0.0f, 0.0f, 1.0f), n);
	//m_cone2->setRotation(rotation1);



}



RFaceDragger::~RFaceDragger()
{
}
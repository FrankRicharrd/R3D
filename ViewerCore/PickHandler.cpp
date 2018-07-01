#include "stdafx.h"
#include "PickHandler.h"
#include "CalculateBoundingBox.h"
#include "ExtraGeometries.h"
#include "RDrawingEntity.h"

PickHandler::PickHandler(FGlobalData* data) : m_data(data)
{
	m_visible = false;
}


PickHandler::~PickHandler()
{
}

osg::Node* PickHandler::getOrCreateSelectionBox()
{
	if (!_selectionBox)
	{
		osg::ref_ptr<osg::Geode> geode = new osg::Geode;

		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
		colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));



		osg::ref_ptr<SelectionBox> selBox = new SelectionBox();
		selBox->CreateAndSetVertices();


		selBox->setColorArray(colors.get());
		selBox->setColorBinding(osg::Geometry::BIND_OVERALL);



		selBox->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 24));


		geode->addDrawable(selBox);

		_selectionBox = new osg::MatrixTransform;
		_selectionBox->setNodeMask(0x1);
		_selectionBox->addChild(geode.get());


		osg::StateSet* ss = _selectionBox->getOrCreateStateSet();
		ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		ss->setAttributeAndModes(new osg::PolygonMode(
			osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE));

		_selectionBox->setNodeMask(m_visible ? 0xffffffff : 0x0);
	}
	return _selectionBox.get();
}bool PickHandler::handle(const osgGA::GUIEventAdapter& ea,
	osgGA::GUIActionAdapter& aa)
{
	return false;
	if (ea.getEventType() != osgGA::GUIEventAdapter::RELEASE || ea.getButton() != osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
	{
		return false;
	}

	SelectionManager::SelectionType type = m_data->GetSelectionManager()->GetSelectionType();

	if (type == SelectionManager::SelectionType::TransformNode)
	{
		ClearCurrentSelection();
		return TransformNodeSelection(ea.getX(), ea.getY());
	}
	else if (type == SelectionManager::SelectionType::IntelligentObject)
	{
		ClearCurrentSelection();

		SetSelected3DPoint(ea.getX(), ea.getY());

	}

	return false;
}

void PickHandler::ClearCurrentSelection()
{
	m_visible = false;

	_selectionBox->setNodeMask(m_visible ? 0xffffffff : 0x0);

	m_data->SetSelectedNode(NULL);

	if (_selectednode.valid())
	{
		RDrawableDefinitionRef def = dynamic_cast<RDrawableDefinition*>(_selectednode.get());
		if (def.valid())
			def->Selected(false);
	}
}

bool PickHandler::TransformNodeSelection(float x, float y)
{
	osg::ref_ptr<osg::Camera> cam = m_data->GetActiveView()->getCamera();


	if (cam.valid())
	{
		osg::ref_ptr<osgUtil::LineSegmentIntersector>
			intersector =
			new osgUtil::LineSegmentIntersector(
				osgUtil::Intersector::WINDOW, x, y
			);



		osgUtil::IntersectionVisitor iv(intersector.get());

		iv.setTraversalMask(~0x1);

		cam->accept(iv);

		if (intersector->containsIntersections())
		{
			osgUtil::LineSegmentIntersector::Intersection result = *(intersector->getIntersections().begin());



			if (!m_data->GetSelectionManager()->CheckIsValidSelectionObject(result.drawable.get()))
			{
				return false;
			}


			osg::MatrixTransform* transNode = NULL;

			osg::Node* selNode = Utilities::GetNodeParentIfIObject(result.drawable);

			if (!selNode)
			{
				transNode = Utilities::GetNearTransformParent(result.drawable);
			}
			else
			{
				transNode = Utilities::GetNearTransformParent(selNode);
			}

			if (!transNode)
				return false;


			CalculateBoundingBox bbox;
			transNode->accept(bbox);

			osg::BoundingBox bb = bbox.getBoundBox();


			osg::BoundingBox bbd = result.drawable->getBoundingBox();


			m_data->SetSelectedNode(transNode);

			if (m_data->GetDragger()->GetIsInUse() || m_data->GetDragger()->GetIsActive())
			{
				m_visible = false;

				_selectionBox->setNodeMask(m_visible ? 0xffffffff : 0x0);

				m_data->SetSelectedNode(NULL);
				return false;
			}

			m_visible = true;


			m_data->GetRenderingRootParent()->asGroup()->removeChild(_selectionBox);
			m_data->GetRenderingRootParent()->asGroup()->addChild(getOrCreateSelectionBox());


			_selectionBox->setNodeMask(m_visible ? 0xffffffff : 0x0);

			auto bbcenter = bb.center();
			auto bbdCenter = bbd.center();

			osg::Vec3 worldCenter = bbd.center() * osg::computeLocalToWorld(result.nodePath);
			//osg::Vec3 ww = bbd.center() * osg::computeLocalToWorld(result.nodePath);

			_selectionBox->setMatrix(
				osg::Matrix::scale(bb.xMax() - bb.xMin(),
					bb.yMax() - bb.yMin(),
					bb.zMax() - bb.zMin()) *
				osg::Matrix::translate(worldCenter));
		
			if (m_data->GetSelectionManager()->NotifyOnTransformNodeSelected != NULL)
			{
				m_data->GetSelectionManager()->NotifyOnTransformNodeSelected();
			}
		
		}
		else
		{
			m_visible = false;

			_selectionBox->setNodeMask(m_visible ? 0xffffffff : 0x0);

			m_data->SetSelectedNode(NULL);

		}
	}

}

bool PickHandler::IntelligentObjectSelection(float x, float y)
{
	osg::ref_ptr<osg::Camera> cam = m_data->GetActiveView()->getCamera();


	if (cam.valid())
	{
		osg::ref_ptr<osgUtil::LineSegmentIntersector>
			intersector =
			new osgUtil::LineSegmentIntersector(
				osgUtil::Intersector::WINDOW, x, y
			);
		osgUtil::IntersectionVisitor iv(intersector.get());

		iv.setTraversalMask(~0x1);
		//viewer->getCamera()->accept(iv);

		cam->accept(iv);



		if (intersector->containsIntersections())
		{
			osgUtil::LineSegmentIntersector::Intersection result = *(intersector->getIntersections().begin());

			osg::ref_ptr<osg::Drawable>  sel = result.drawable.get();

			RDrawableDefinitionRef drdef = dynamic_cast<RDrawableDefinition*>(sel.get());

			if (!drdef.valid())
				return false;

			drdef->Selected(true);
			_selectednode = drdef;

			return true;
		}



	}

	return false;
}

void PickHandler::SetSelected3DPoint(float x, float y)
{
	osg::ref_ptr<osg::Camera> cam = m_data->GetActiveView()->getCamera();

	if (cam.valid())
	{

		///*m.preMult(getCamera()->getViewport()->computeWindowMatrix());
		//m.preMult(getCamera()->getProjectionMatrix());
		//m.preMult(getCamera()->getViewMatrix());

		osg::Matrix windowMat = cam->getViewport()->computeWindowMatrix();
		osg::Matrix proMat = cam->getProjectionMatrix();
		osg::Matrix viewmat = cam->getViewMatrix();

		osg::Matrix mvpw = viewmat * proMat * windowMat;
		osg::Matrix inmvpw = osg::Matrix::inverse(mvpw);


		osg::Vec3 wp1(x, y, 0);

		osg::Vec3 p1 = wp1 * inmvpw;

		osg::Vec3 wp2(x, y, 1);

		osg::Vec3 p2 = wp2 * inmvpw;


		osg::Vec3 p3 = inmvpw* wp1;



		osg::Vec3 p4 = inmvpw* wp2;



		std::string str = std::to_string(p1.x()) + "," + std::to_string(p1.y()) + "," + std::to_string(p1.z());
		OutputDebugStringA(str.c_str());

		std::string str1 = std::to_string(p2.x()) + "," + std::to_string(p2.y()) + "," + std::to_string(p2.z());
		OutputDebugStringA(str1.c_str());
	}
}


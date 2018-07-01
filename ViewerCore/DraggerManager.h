#pragma once

#include "stdafx.h"


class PlaneConstraint : public osgManipulator::Constraint
{
public:
	PlaneConstraint() {}

	virtual bool constrain(osgManipulator::TranslateInLineCommand& command) const
	{
		OSG_NOTICE << "PlaneConstraint TranslateInLineCommand " << command.getTranslation() << std::endl;
		return true;
	}
	virtual bool constrain(osgManipulator::TranslateInPlaneCommand& command) const
	{
		//command.setTranslation(osg::Vec3(0.0f,0.0f,0.0f));
		OSG_NOTICE << "PlaneConstraint TranslateInPlaneCommand " << command.getTranslation() << std::endl;
		return true;
	}
	virtual bool constrain(osgManipulator::Scale1DCommand& command) const
	{
		//command.setScale(1.0f);
		OSG_NOTICE << "PlaneConstraint Scale1DCommand" << command.getScale() << std::endl;
		return true;
	}
	virtual bool constrain(osgManipulator::Scale2DCommand& command) const
	{
		//command.setScale(osg::Vec2d(1.0,1.0));
		OSG_NOTICE << "PlaneConstraint Scale2DCommand " << command.getScale() << std::endl;
		return true;
	}
	virtual bool constrain(osgManipulator::ScaleUniformCommand& command) const
	{
		OSG_NOTICE << "PlaneConstraint ScaleUniformCommand" << command.getScale() << std::endl;
		return true;
	}
};



// The DraggerContainer node is used to fix the dragger's size on the screen
class VIEWER_API DraggerContainer : public osg::Group
{
public:
	DraggerContainer() : _draggerSize(240.0f), _active(true) {}

	DraggerContainer(const DraggerContainer& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
		: osg::Group(copy, copyop),
		_dragger(copy._dragger), _draggerSize(copy._draggerSize), _active(copy._active)
	{}

	META_Node(osgManipulator, DraggerContainer);

	void setDragger(osgManipulator::Dragger* dragger)
	{
		_dragger = dragger;
		if (!containsNode(dragger)) addChild(dragger);
	}

	osgManipulator::Dragger* getDragger() { return _dragger.get(); }
	const osgManipulator::Dragger* getDragger() const { return _dragger.get(); }

	void setDraggerSize(float size) { _draggerSize = size; }
	float getDraggerSize() const { return _draggerSize; }

	void setActive(bool b) { _active = b; }
	bool getActive() const { return _active; }

	void traverse(osg::NodeVisitor& nv)
	{
		if (_dragger.valid())
		{
			if (_active && nv.getVisitorType() == osg::NodeVisitor::CULL_VISITOR)
			{
				osgUtil::CullVisitor* cv = static_cast<osgUtil::CullVisitor*>(&nv);

				float pixelSize = cv->pixelSize(_dragger->getBound().center(), 0.48f);
				if (pixelSize != _draggerSize)
				{
					float pixelScale = pixelSize > 0.0f ? _draggerSize / pixelSize : 1.0f;
					osg::Vec3d scaleFactor(pixelScale, pixelScale, pixelScale);

					osg::Vec3 trans = _dragger->getMatrix().getTrans();
					_dragger->setMatrix(osg::Matrix::scale(scaleFactor) * osg::Matrix::translate(trans));
				}
			}
		}
		osg::Group::traverse(nv);
	}

protected:
	osg::ref_ptr<osgManipulator::Dragger> _dragger;
	float _draggerSize;
	bool _active;
};

class DraggerEventHandler : public osgGA::GUIEventHandler

{
public:
	DraggerEventHandler()

	{


	}



	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		osgViewer::View* view = static_cast<osgViewer::View*>(&aa);
		if (view)
		{
			auto eventTyp = ea.getEventType();

			if (eventTyp == osgGA::GUIEventAdapter::PUSH)
			{

				int a = 0;
				int b = 0;
				int c = a + b*b;
			}
		}
		return false;
	}

	~DraggerEventHandler() {

	}

private:

};


class DraggerManager : public osg::Referenced
{



public:
	DraggerManager(FGlobalData* data);
	~DraggerManager();





	bool GetIsActive() { return m_IsActive; }
	void SetIsActive(bool isactive) { m_IsActive = isactive; }

	bool GetIsInUse() { return m_IsInUse; }
	void SetIsInUse(bool isInUse) { m_IsInUse = isInUse; }

	void SetActiveDraggerType(ViewerCore::DraggerType draggerTyp) { m_activeDraggerType = draggerTyp; }

	void SetDraggerToRendererParent();

	void RemoveDraggerFromParentRenderer();

private:
	osgManipulator::Dragger* InitDragger(ViewerCore::DraggerType dtype);
	void AddDraggerToParentRenderer(osg::Node * node);
	osg::Node* CreateDraggerAndSetToData(osg::MatrixTransform* scene, ViewerCore::DraggerType dragType, bool fixedSizeInScreen);
private:
	FGlobalData* m_data;
	bool m_IsActive;
	bool m_IsInUse;


	osg::ref_ptr<osg::Node> m_activeDragger;

	ViewerCore::DraggerType m_activeDraggerType;

};
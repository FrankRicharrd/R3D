#include "stdafx.h"
#include "RDrawingEntity.h"
#include "RCustomDragger.h"
#include "RDraggerCallbacks.h"
#include "RIntelligentObject.h"
#include "RIOConstrain.h"

RIntelliDrawableInstance::RIntelliDrawableInstance()
{
}

inline RIntelliDrawableInstance::RIntelliDrawableInstance(RIntelliDrawableDef * definition) : RDrawableInstance(definition), m_IOIndex(-1), m_isFixed(false)
{
}

inline RIntelliDrawableInstance::RIntelliDrawableInstance(const RIntelliDrawableInstance & copy, const osg::CopyOp & copyop) : RDrawableInstance(copy, copyop)
{
}



void RIntelliDrawableInstance::SetDraggerVisibility(bool isvisible)
{
	RIntelliDrawableDef* def = dynamic_cast<RIntelliDrawableDef*>(GetDefinition().get());

	for (DraggerCollection::const_iterator itr = def->GetDraggers().begin();
		itr != def->GetDraggers().end();
		++itr)
	{
		auto draggers = itr->get();

		if (!isvisible)
		{
			draggers->setHandleEvents(false);
			draggers->setDraggerActive(false);
			
			//remove dragger callbacks here.

			removeChild(draggers);
		}
		else
		{

			RFaceRef face = draggers->GetFace();

		/*	osg::ref_ptr<osgManipulator::GridConstraint> grid = new osgManipulator::GridConstraint(*this, face->GetMidPoint(), osg::Vec3(1, 0, 0));

			draggers->addConstraint(grid);*/

			osg::ref_ptr<RFaceDraggerCallback> draggercallback = new RFaceDraggerCallback(face.get(), this);

			draggers->addDraggerCallback(draggercallback);
			//draggers->addTransformUpdating(this);
			draggers->setHandleEvents(true);
			draggers->setDraggerActive(true);
			
			addChild(draggers);
		}
	}
}

void RIntelliDrawableInstance::UpdateConstraints()
{
	for (RIOConstrainRef con : m_Constrains)
	{
		con->Update(this);
	}
}

bool RIntelliDrawableInstance::AddConstrain(RIOConstrain * constrain)
{
	return RIntelliDrawableInstance::InsertConstrain(m_Constrains.size(), constrain);
}

bool RIntelliDrawableInstance::InsertConstrain(unsigned int index, RIOConstrain * constrain)
{
	if (!constrain) return false;

	if (containsConstrain(constrain))
		return false;

	if (index >= m_Constrains.size())
	{
		index = m_Constrains.size();      // set correct index value to be passed to the "childInserted" method
		m_Constrains.push_back(constrain);
	}
	else
	{
		m_Constrains.insert(m_Constrains.begin() + index, constrain);
	}

	return true;
}

bool RIntelliDrawableInstance::containsConstrain(const RIOConstrain * constrain) const
{
	for (RIOConstrainCollection::const_iterator itr = m_Constrains.begin();
		itr != m_Constrains.end();
		++itr)
	{
		if (itr->get() == constrain) return true;
	}
	return false;
}

RIntelliDrawableInstance::~RIntelliDrawableInstance()
{
}
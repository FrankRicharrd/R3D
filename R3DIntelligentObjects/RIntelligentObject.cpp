#include "stdafx.h"
#include "EnumManager.h"
#include "RCustomDragger.h"
#include "RIntelligentObject.h"
#include "RIOTranslateDragger.h"
#include "RIOConstrain.h"


void RIOUpdateCallback ::operator()(osg::Node* node,
	osg::NodeVisitor* nv)
{
	RIntelligentObjectRef ioNode = dynamic_cast<RIntelligentObject*>(node);
	
	ioNode->UpdateTempInstances();

	traverse(node, nv);
}



RIntelligentObject::RIntelligentObject()
{
	setDataVariance(osg::Object::DYNAMIC);
	setUpdateCallback(new RIOUpdateCallback());
}

RIntelligentObject::RIntelligentObject(const RIntelligentObject & copy, const osg::CopyOp & copyop) : osg::Group(copy, copyop)
{
	
}
bool RIntelligentObject::AddDefinition(RIntelliDrawableDef * def)
{
	return RIntelligentObject::InsertDefinition(m_definitions.size(), def);
}
bool RIntelligentObject::InsertDefinition(unsigned int index, RIntelliDrawableDef * def)
{
	if (!def) return false;

	if (containsDefinition(def))
		return false;

	if (index >= m_definitions.size())
	{
		index = m_definitions.size();      // set correct index value to be passed to the "childInserted" method
		m_definitions.push_back(def);
	}
	else
	{
		m_definitions.insert(m_definitions.begin() + index, def);
	}
	return true;
}
bool RIntelligentObject::containsDefinition(const RIntelliDrawableDef * def) const
{
	for (RIntelliDrawableDefCollection::const_iterator itr = m_definitions.begin();
		itr != m_definitions.end();
		++itr)
	{
		if (itr->get() == def) return true;
	}
	return false;
}
bool RIntelligentObject::AddInstance(RIntelliDrawableInstance * instance)
{
	auto def = dynamic_cast<RIntelliDrawableDef*>(instance->GetDefinition().get());

	if (!containsDefinition(def))
		AddDefinition(def);

	return RIntelligentObject::InsertInstance(m_drawableInstances.size(), instance);
}

void RIntelligentObject::Redraw(bool initDraw)
{
	for (auto d : m_definitions)
	{
		d->Redraw(initDraw);
	}
}

osg::ref_ptr< RIOTranslateDragger> RIntelligentObject::AddTranslateDragger(osg::Vec3 normal, osg::Vec3 position, RIntelliDrawableInstanceCollection attachedInstances, RIntelliDrawableInstanceCollection fixedInstances)
{
	osg::ref_ptr< RIOTranslateDragger> translatedragger = new RIOTranslateDragger(normal, position);
	osg::ref_ptr<RIOTranslateDraggerCallback> callback = new RIOTranslateDraggerCallback(attachedInstances, fixedInstances);
	translatedragger->addDraggerCallback(callback);
	translatedragger->setupDefaultGeometry();
	m_translateDraggers.push_back(translatedragger);

	return translatedragger;
}

void RIntelligentObject::SetDraggerVisibility(bool visible)
{

	for (RIOTranslateDraggerCollection::const_iterator itr = m_translateDraggers.begin();
		itr != m_translateDraggers.end();
		++itr)
	{
		auto draggers = itr->get();

		if (!visible)
		{
			draggers->setHandleEvents(false);
			draggers->setDraggerActive(false);

			//remove dragger callbacks here.

			removeChild(draggers);
		}
		else
		{

			draggers->setHandleEvents(true);
			draggers->setDraggerActive(true);

			addChild(draggers);
		}
	}

}

bool RIntelligentObject::AddInternalConstrain(RIOConstrain * constrain)
{
	return RIntelligentObject::InsertInternalConstrain(m_internalConstrains.size(), constrain);
}

bool RIntelligentObject::AddTempInstance(RIntelliDrawableInstance * instance)
{
	return RIntelligentObject::InsertTempInstanceToAdd(m_TempInstancesToAdd.size(), instance);
}

bool RIntelligentObject::RemoveTempInstance(RIntelliDrawableInstance * instance)
{
	return RIntelligentObject::InsertTempInstanceToRemove(m_TempInstancesToRemove.size(), instance);
}


bool RIntelligentObject::containsInstance(const RIntelliDrawableInstance * instance) const
{
	for (RIntelliDrawableInstanceCollection::const_iterator itr = m_drawableInstances.begin();
		itr != m_drawableInstances.end();
		++itr)
	{
		if (itr->get() == instance) return true;
	}
	return false;
}

bool RIntelligentObject::InsertInternalConstrain(unsigned int index, RIOConstrain * constrain)
{
	if (!constrain) return false;

	if (containsInternalConstrain(constrain))
		return false;

	if (index >= m_internalConstrains.size())
	{
		index = m_internalConstrains.size();      // set correct index value to be passed to the "childInserted" method
		m_internalConstrains.push_back(constrain);
	}
	else
	{
		m_internalConstrains.insert(m_internalConstrains.begin() + index, constrain);
	}
	return true;
}

bool RIntelligentObject::containsInternalConstrain(const RIOConstrain * constrain) const
{
	for (RIOConstrainCollection::const_iterator itr = m_internalConstrains.begin();
		itr != m_internalConstrains.end();
		++itr)
	{
		if (itr->get() == constrain) return true;
	}
	return false;
}

bool RIntelligentObject::InsertTempInstanceToAdd(unsigned int index, RIntelliDrawableInstance * instance)
{
	if (!instance) return false;

	if (containsTempInstanceToAdd(instance))
		return false;

	if (index >= m_TempInstancesToAdd.size())
	{
		index = m_TempInstancesToAdd.size();      // set correct index value to be passed to the "childInserted" method
		m_TempInstancesToAdd.push_back(instance);
	}
	else
	{
		m_TempInstancesToAdd.insert(m_TempInstancesToAdd.begin() + index, instance);
	}

	return true;
}

bool RIntelligentObject::containsTempInstanceToAdd(const RIntelliDrawableInstance * instance) const
{
	for (RIntelliDrawableInstanceCollection::const_iterator itr = m_TempInstancesToAdd.begin();
		itr != m_TempInstancesToAdd.end();
		++itr)
	{
		if (itr->get() == instance) return true;
	}
	return false;
}

bool RIntelligentObject::InsertTempInstanceToRemove(unsigned int index, RIntelliDrawableInstance * instance)
{
	if (!instance) return false;

	if (containsTempInstanceToRemove(instance))
		return false;

	if (index >= m_TempInstancesToRemove.size())
	{
		index = m_TempInstancesToRemove.size();      // set correct index value to be passed to the "childInserted" method
		m_TempInstancesToRemove.push_back(instance);
	}
	else
	{
		m_TempInstancesToRemove.insert(m_TempInstancesToRemove.begin() + index, instance);
	}

	return true;
}

bool RIntelligentObject::containsTempInstanceToRemove(const RIntelliDrawableInstance * instance) const
{
	for (RIntelliDrawableInstanceCollection::const_iterator itr = m_TempInstancesToRemove.begin();
		itr != m_TempInstancesToRemove.end();
		++itr)
	{
		if (itr->get() == instance) return true;
	}
	return false;
}

void RIntelligentObject::UpdateTempInstances()
{

	for (auto i : m_TempInstancesToAdd)
	{
		AddInstance(i);
	}

	// remove instances here.

	m_TempInstancesToAdd.clear();
	m_TempInstancesToRemove.clear();
}



bool RIntelligentObject::InsertInstance(unsigned int index, RIntelliDrawableInstance * instance)
{
	if (!instance) return false;

	if (containsInstance(instance))
		return false;

	if (index >= m_drawableInstances.size())
	{
		index = m_drawableInstances.size();      // set correct index value to be passed to the "childInserted" method
		m_drawableInstances.push_back(instance);
	}
	else
	{
		m_drawableInstances.insert(m_drawableInstances.begin() + index, instance);
	}
	instance->SetIOIndex(index);
	
	bool status  = addChild(instance); // adding instance to osg

	

	return true;
}

RIntelligentObject::~RIntelligentObject()
{
}

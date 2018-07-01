#include "stdafx.h"
#include "RIntelligentObject.h"
#include "RIntelligentObjectManager.h"


RIntelligentObjectManager::RIntelligentObjectManager()
{

}


RIntelligentObjectManager::~RIntelligentObjectManager()
{
}

bool RIntelligentObjectManager::AddIntelligentObject(RIntelligentObject * obj)
{
	bool status = InsertIntelligentObject(m_intelligentObjects.size(), obj);

	return status;
}

bool RIntelligentObjectManager::InsertIntelligentObject(unsigned int index, RIntelligentObject * obj)
{
	if (!obj) return false;

	if (containsIntelligentObject(obj))
		return false;

	if (index >= m_intelligentObjects.size())
	{
		index = m_intelligentObjects.size();      // set correct index value to be passed to the "childInserted" method
		m_intelligentObjects.push_back(obj);
	}
	else
	{
		m_intelligentObjects.insert(m_intelligentObjects.begin() + index, obj);
	}
	return true;
}

bool RIntelligentObjectManager::containsIntelligentObject(const RIntelligentObject * obj) const
{
	for (RIntelligentObjectCollection::const_iterator itr = m_intelligentObjects.begin();
		itr != m_intelligentObjects.end();
		++itr)
	{
		if (itr->get() == obj) return true;
	}
	return false;
}

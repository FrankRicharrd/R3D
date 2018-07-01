#pragma once

#include "RIntelligentObject.h"

class R3DEXPORT RIntelligentObjectManager 
{
public:
	RIntelligentObjectManager();

	
	bool AddIntelligentObject(RIntelligentObject* vertex);
	bool InsertIntelligentObject(unsigned int index, RIntelligentObject*vertex);
	bool containsIntelligentObject(const RIntelligentObject* vertex) const;

	RIntelligentObjectCollection& GetIntelligentObjects() { return m_intelligentObjects; }

private:
	RIntelligentObjectCollection m_intelligentObjects;

	//osg::Node* m_node;

protected:
	~RIntelligentObjectManager();
};


#pragma once
#include "RDrawingEntity.h"
#include "RCustomDragger.h"
#include "RIODrawingEntities.h"

class RIOConstrain;
class RIOTranslateDragger;
class RIntelliDrawableDef;
class RIntelliDrawableInstance;
class RIntelligentObject;

typedef osg::ref_ptr<RIntelliDrawableDef> RIntelliDrawableDefRef;
typedef osg::ref_ptr<RIntelliDrawableInstance> RIntelliDrawableInstanceRef;
typedef osg::ref_ptr<RIntelligentObject> RIntelligentObjectRef;
typedef osg::ref_ptr<RIOTranslateDragger> RIOTranslateDraggerRef;
typedef osg::ref_ptr<RIOConstrain> RIOConstrainRef;



typedef std::vector<RIntelliDrawableDefRef> RIntelliDrawableDefCollection;
typedef std::vector<RIntelliDrawableInstanceRef> RIntelliDrawableInstanceCollection;
typedef std::vector<osg::ref_ptr<RFaceDragger>> DraggerCollection;
typedef std::vector<RIntelligentObjectRef> RIntelligentObjectCollection;
typedef std::vector<RIOConstrainRef> RIOConstrainCollection;
typedef std::vector<RIOTranslateDraggerRef> RIOTranslateDraggerCollection;



class R3DEXPORT RIntelliDrawableDef : public RDrawableDefinition
{
public:
	RIntelliDrawableDef(RVertex3DCollection vertices, RIntelligentObject* intelliibject);
	RIntelliDrawableDef(const RIntelliDrawableDef& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);

	META_Object(R3DViewer, RIntelliDrawableDef);

	void SetDraggerVisibility(bool isvisible);

	bool AddFaceDragger(int faceIndex);

	DraggerCollection& GetDraggers() { return m_draggers; }

	void UpdateDraggers();

	//ccw vertices index of a face's outer edgeloop, this adds the created face to definition
	RIOFaceRef CreateFace(std::vector<int> vertexIndex);

	RIntelligentObjectRef GetIOObject() { return m_intelliibject; }

private:

	DraggerCollection m_draggers;

	int GetOrCreateEdge(REdgeRef edge);
	RIntelligentObject* m_intelliibject;

protected:
	~RIntelliDrawableDef();
	RIntelliDrawableDef();
};



class R3DEXPORT RIntelliDrawableInstance : public RDrawableInstance
{
public:

	RIntelliDrawableInstance(RIntelliDrawableDef* definition);

	RIntelliDrawableInstance(const RIntelliDrawableInstance& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);

	META_Object(R3DViewer, RIntelliDrawableInstance);


	void SetDraggerVisibility(bool isvisible);

	void UpdateConstraints();

	int GetIOIndex() { return m_IOIndex; }
	void SetIOIndex(int i) { m_IOIndex = i; }
	bool AddConstrain(RIOConstrain* constrain);

	bool IsFixed() { return m_isFixed; }

	void SetFixMode(bool isfixed) { m_isFixed = isfixed; }

	RIntelliDrawableDefRef GetIODefinition() { return dynamic_cast<RIntelliDrawableDef*>(GetDefinition().get()); }


private:
	int m_IOIndex;
	RIOConstrainCollection m_Constrains;
	bool m_isFixed;
	bool InsertConstrain(unsigned int index, RIOConstrain *constrain);
	bool containsConstrain(const RIOConstrain* constrain) const;
protected:

	~RIntelliDrawableInstance();
	RIntelliDrawableInstance();
};

class R3DEXPORT RIOUpdateCallback : public osg::NodeCallback
{
public:
	RIOUpdateCallback() {}
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

};


class R3DEXPORT RIntelligentObject : public osg::Group
{
public:
	RIntelligentObject();
	RIntelligentObject(const RIntelligentObject& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);
	META_Node(R3DViewer, RIntelligentObject);

	RIntelliDrawableDefCollection& GetDefinitions() { return m_definitions; }

	bool AddDefinition(RIntelliDrawableDef* def);
	bool InsertDefinition(unsigned int index, RIntelliDrawableDef*def);
	bool containsDefinition(const RIntelliDrawableDef* def) const;

	bool AddInstance(RIntelliDrawableInstance* instance);

	void Redraw(bool initDraw);

	osg::ref_ptr<RIOTranslateDragger> AddTranslateDragger(osg::Vec3 normal, osg::Vec3 position, RIntelliDrawableInstanceCollection attachedInstances, RIntelliDrawableInstanceCollection fixedInstances);

	void SetDraggerVisibility(bool visible);

	bool AddInternalConstrain(RIOConstrain* constrain);

	bool AddTempInstance(RIntelliDrawableInstance* instance);
	bool RemoveTempInstance(RIntelliDrawableInstance* instance);

	void UpdateTempInstances();

private:
	RIOConstrainCollection m_internalConstrains;
	RIOTranslateDraggerCollection m_translateDraggers;
	RIntelliDrawableDefCollection m_definitions;
	RIntelliDrawableInstanceCollection m_drawableInstances;
	
	RIntelliDrawableInstanceCollection m_TempInstancesToAdd;
	RIntelliDrawableInstanceCollection m_TempInstancesToRemove;

	bool InsertInstance(unsigned int index, RIntelliDrawableInstance *instance);
	bool containsInstance(const RIntelliDrawableInstance* instance) const;
	bool InsertInternalConstrain(unsigned int index, RIOConstrain *constrain);
	bool containsInternalConstrain(const RIOConstrain* constrain) const;

	bool InsertTempInstanceToAdd(unsigned int index, RIntelliDrawableInstance *instance);
	bool containsTempInstanceToAdd(const RIntelliDrawableInstance* instance) const;

	bool InsertTempInstanceToRemove(unsigned int index, RIntelliDrawableInstance *instance);
	bool containsTempInstanceToRemove(const RIntelliDrawableInstance* instance) const;



protected:
	~RIntelligentObject();
};


#pragma once
#include "RIntelligentObject.h"



class RIOFaceConstrain;

typedef std::pair<RIntelliDrawableInstanceRef, RIOFaceRef> RIOFaceAndInstancePair;
typedef osg::ref_ptr<RIOFaceConstrain> RIOFaceConstrainRef;
typedef osg::ref_ptr<RIOConstrain> RIOConstrainRef;
typedef std::vector<RIOConstrainRef> RIOConstrainCollection;

class R3DEXPORT RIOConstrain : public osg::Node
{
public:
	enum RIOConstrainType
	{
		None,
		Coincident,
		Parallel,
		Perpendicular,
		Tangent,
		Concentric,
		Lock,
		//distance towards the positive normal of primary entity
		PositiveDistance,
		//distance towards the negative normal of primary entity
		NegativeDistance,
		Angle
	};

public:
	META_OSGMANIPULATOR_Object(R3DViewer, RIOConstrain);
	virtual void Update(RIntelliDrawableInstance* updateFromInstance) {};

	virtual void AddConstrainToEntities() {};

	void SetConstrainType(RIOConstrainType type) { m_constrainType = type; }

protected:
	RIOConstrainType m_constrainType;
	~RIOConstrain();
	RIOConstrain();
};

class R3DEXPORT RIOFaceConstrain : public RIOConstrain
{
public:
	//enity1 should be primary entity. with which objects are constrained
	RIOFaceConstrain(RIOFaceAndInstancePair entity1, RIOFaceAndInstancePair entity2);
	META_OSGMANIPULATOR_Object(R3DViewer, RIOFaceConstrain);
	void Update(RIntelliDrawableInstance* updateFromInstance);

	RIOConstrainType GetConstrainType() { return m_constrainType; }
	void SetConstrainType(RIOConstrainType type) { m_constrainType = type; }

	void SetConstrainDistance(double dist) { m_distance = dist; }
	

protected:
	RIOFaceConstrain();
	~RIOFaceConstrain();

private:

	RIOFaceAndInstancePair m_entity1;
	RIOFaceAndInstancePair m_entity2;
	void AddConstrainToEntities();
	void ProcessEntity(RIOFaceAndInstancePair & entity, RIOFaceAndInstancePair & updatedEntity);
	double m_distance;

	bool IsPrimaryEntity(RIOFaceAndInstancePair& entity);

	double distanceBtwnPointandPlace(osg::Vec3 point, osg::Vec3 pointOnPlace, osg::Vec3 normalofplace);

};









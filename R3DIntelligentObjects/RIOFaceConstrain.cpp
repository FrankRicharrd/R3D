#include "stdafx.h"
#include "RIOConstrain.h"

RIOFaceConstrain::RIOFaceConstrain()
{
}

RIOFaceConstrain::~RIOFaceConstrain()
{
}

inline void RIOFaceConstrain::Update(RIntelliDrawableInstance* updateFromInstance)
{
	if (m_entity1.first->GetIOIndex() == updateFromInstance->GetIOIndex())
	{
		//process entity2 here

		ProcessEntity(m_entity2, m_entity1);


	}
	else if (m_entity2.first->GetIOIndex() == updateFromInstance->GetIOIndex())
	{
		//process entity1 here

		ProcessEntity(m_entity1, m_entity2);
	}
}

inline void RIOFaceConstrain::AddConstrainToEntities()
{
	m_entity1.first->AddConstrain(this);
	m_entity2.first->AddConstrain(this);
}

void RIOFaceConstrain::ProcessEntity(RIOFaceAndInstancePair & entity, RIOFaceAndInstancePair & updatedEntity)
{
	osg::Matrix localtoworld1 = entity.first->GetLocaltoWorldMatrix();
	osg::Matrix localtoworld2 = updatedEntity.first->GetLocaltoWorldMatrix();



	if (entity.first->IsFixed())
	{
		//move face of entity

		if (IsPrimaryEntity(updatedEntity))
		{
			if (m_constrainType == RIOConstrainType::NegativeDistance)
			{
				osg::Vec3 mp2 = localtoworld2 * updatedEntity.second->GetMidPoint();
				osg::Vec3 mp1 = localtoworld1 *entity.second->GetMidPoint();

				double currentDistance = distanceBtwnPointandPlace(mp1, mp2, updatedEntity.second->GetFaceNormal());

				double distanceToMove = m_distance + currentDistance;

				osg::Vec3 n = updatedEntity.second->GetFaceNormal();
				n.normalize();

				osg::Vec3 vec = n * distanceToMove;

				entity.second->TranslateFace(vec);

			}
			else if (m_constrainType == RIOConstrainType::PositiveDistance)
			{

				//double distanceToMove = currentDistance - m_distance;


			}
		}
		else if (IsPrimaryEntity(entity))
		{
			if (m_constrainType == RIOConstrainType::NegativeDistance)
			{
				auto amp2 = updatedEntity.second->GetMidPoint();
				auto amp1 = entity.second->GetMidPoint();

				osg::Vec4 vec4of2(amp2, 1);
				osg::Vec4 vec4of1(amp1, 1);


				osg::Vec4 mp2 = vec4of2 * localtoworld2;
				osg::Vec4 mp1 = vec4of1 * localtoworld1;

				osg::Vec3 v2(mp2.x(), mp2.y(), mp2.z());
				osg::Vec3 v1(mp1.x(), mp1.y(), mp1.z());

				double currentDistance = distanceBtwnPointandPlace(v2, v1, entity.second->GetFaceNormal());

				double distanceToMove = m_distance + currentDistance;

				osg::Vec3 n = entity.second->GetFaceNormal();
				n.normalize();

				osg::Vec3 vec = n * distanceToMove;

				entity.second->TranslateFace(vec);

			}
			else if (m_constrainType == RIOConstrainType::PositiveDistance)
			{

				//double distanceToMove = currentDistance - m_distance;
				auto amp2 = updatedEntity.second->GetMidPoint();
				auto amp1 = entity.second->GetMidPoint();

				osg::Vec4 vec4of2(amp2, 1);
				osg::Vec4 vec4of1(amp1, 1);


				osg::Vec4 mp2 = vec4of2 * localtoworld2;
				osg::Vec4 mp1 = vec4of1 * localtoworld1;

				osg::Vec3 v2(mp2.x(), mp2.y(), mp2.z());
				osg::Vec3 v1(mp1.x(), mp1.y(), mp1.z());

				double currentDistance = distanceBtwnPointandPlace(v2, v1, entity.second->GetFaceNormal());

				double distanceToMove = currentDistance - m_distance;

				osg::Vec3 n = entity.second->GetFaceNormal();
				n.normalize();

				osg::Vec3 vec = n * distanceToMove;

				entity.second->TranslateFace(vec);

			}
		}
		else
		{
			DebugBreak();
		}


	}
	else
	{
		// translate instance of entity
	}
}

bool RIOFaceConstrain::IsPrimaryEntity(RIOFaceAndInstancePair & entity)
{
	if (m_entity1.first->GetIOIndex() == entity.first->GetIOIndex())
		return true;
	else
		return false;
}

double RIOFaceConstrain::distanceBtwnPointandPlace(osg::Vec3 point, osg::Vec3 pointOnPlace, osg::Vec3 normalofplace)
{
	normalofplace.normalize();

	osg::Vec3 n((point.x() - pointOnPlace.x()), (point.y() - pointOnPlace.y()), (point.z() - pointOnPlace.z()));


	double distance = n*normalofplace;

	return distance;
}

RIOFaceConstrain::RIOFaceConstrain(RIOFaceAndInstancePair entity1, RIOFaceAndInstancePair entity2) :
	m_entity1(entity1), m_entity2(entity2), m_distance(0)
{
	AddConstrainToEntities();
}
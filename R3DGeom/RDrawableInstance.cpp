#include "stdafx.h"
#include "RDrawingEntity.h"
#include "RPrimitivesCreator.h"


RDrawableInstance::RDrawableInstance(RDrawableDefinition* definition) : m_definition(definition), m_Defindex(-1)
{
	definition->AddInstance(this);
	osg::ref_ptr<osg::Geode> geo = new osg::Geode();
	geo->addDrawable(definition);
	addChild(geo);
}

inline RDrawableInstance::RDrawableInstance(const RDrawableInstance & copy, const osg::CopyOp & copyop)
	: osg::MatrixTransform(copy, copyop),
	m_definition(copy.m_definition)
{
}
osg::Matrix RDrawableInstance::GetLocaltoWorldMatrix()
{
	osg::NodePath nodePathToRoot;
	osgManipulator::computeNodePathToRoot(*this, nodePathToRoot);
	osg::Matrix _localToWorld = osg::computeLocalToWorld(nodePathToRoot);
	return _localToWorld;
}


bool RDrawableInstance::Redraw(bool initDraw)
{
	if (initDraw)
	{
		if (true)
		{
			//m_lineGeom = RPrimitivesCreator::createLnGeometry(osg::PrimitiveSet::LINES, m_definition->GetDrawableVertices().get(), osg::Vec4(0, 0, 0, 1));
			osg::ref_ptr<osg::Geode> geode = new osg::Geode();

			m_lineGeom = new osg::Geometry();

			int numEdgeVertices = m_definition->GetEdges().size() * 2;

			osg::ref_ptr<osg::DrawElementsUInt> edgeElement(new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, numEdgeVertices));
			osg::DrawElementsUInt::iterator edgeIndex_itr(edgeElement->begin());

			for (auto e : m_definition->GetEdges())
			{
				*(edgeIndex_itr++) = e->GetStartpoint()->GetIndex();
				*(edgeIndex_itr++) = e->GetEndpoint()->GetIndex();
			}

			
			m_lineGeom->setVertexArray(m_definition->GetDrawableVertices().get());

			osg::Vec4Array *colors = new osg::Vec4Array;
			colors->push_back(osg::Vec4(0.0f, .0f, .0f, 1.0f));
			m_lineGeom->setColorArray(colors);
			m_lineGeom->setColorBinding(osg::Geometry::BIND_OVERALL);


			auto  m_material = new osg::Material();
			m_material->setAmbient(osg::Material::FRONT, osg::Vec4(0.0, 0.0, 0.0, 1.0));
			m_material->setDiffuse(osg::Material::FRONT, osg::Vec4(0.0, 0.0, 0.0, 1.0));
			m_material->setSpecular(osg::Material::FRONT, osg::Vec4(0.0225, 0.0225, 0.0225, 1.0));
			m_material->setEmission(osg::Material::FRONT, osg::Vec4(0.0, 0.0, 0.0, 1.0));
			m_material->setShininess(osg::Material::FRONT, 12.8);

			m_lineGeom->getOrCreateStateSet()->setAttribute(m_material);


			/*setNormalArray(norms);
			setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
			*/
			osg::StateSet* ss = m_lineGeom->getOrCreateStateSet();
			ss->setAttributeAndModes(new osg::LineWidth(2));

			m_lineGeom->addPrimitiveSet(edgeElement.get());
			geode->addDrawable(m_lineGeom);
			addChild(geode);
		}
		//else
		//{
		//	m_lineGeom = new osg::Geometry();
		//	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

		//	int numEdgeVertices = m_definition->GetEdges().size() * 2;

		///*	osg::ref_ptr<osg::DrawElementsUInt> edgeElement(new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, numEdgeVertices));
		//	osg::DrawElementsUInt::iterator edgeIndex_itr(edgeElement->begin());

		//	for (auto e : m_definition->GetEdges())
		//	{
		//		*(edgeIndex_itr++) = e->GetStartpoint()->GetIndex();
		//		*(edgeIndex_itr++) = e->GetEndpoint()->GetIndex();
		//	}*/

		//	osg::ref_ptr<osg::Vec3Array> ver = new osg::Vec3Array();
		//	osg::ref_ptr<osg::Vec4Array> col = new osg::Vec4Array();

		//	for (auto e : m_definition->GetEdges())
		//	{
		//		ver->push_back(e->GetStartpoint()->GetOsgVec());
		//		ver->push_back(e->GetEndpoint()->GetOsgVec());// ->GetIndex();
		//	
		//		col->push_back(osg::Vec4(1.0f, .0f, .0f, 1.0f));
		//	
		//	}



		//	osg::Vec4Array *colors = new osg::Vec4Array;
		//	colors->push_back(osg::Vec4(1.0f, .0f, .0f, 1.0f));


		//	m_lineGeom->setVertexArray(ver.get());

		//	m_lineGeom->setColorArray(col);
		//	m_lineGeom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

		//	/*setNormalArray(norms);
		//	setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
		//	*/
		//	osg::StateSet* ss = m_lineGeom->getOrCreateStateSet();
		//	//ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		//	ss->setAttributeAndModes(new osg::LineWidth(2));

		//	m_lineGeom->addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, ver->size()));
		//	geode->addDrawable(m_lineGeom);
		//	addChild(geode);
		//}

	}
	else
	{
		
		m_lineGeom->dirtyDisplayList();
		m_lineGeom->dirtyBound();
	}


	return true;
}

RDrawableInstance::RDrawableInstance()
{
}

RDrawableInstance::~RDrawableInstance()
{
}




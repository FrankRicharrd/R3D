#include "stdafx.h"
#include "RDrawingEntity.h"

class DynamicQuadCallback : public osg::Drawable::UpdateCallback
{
public:
	virtual void update(osg::NodeVisitor*, osg::Drawable*
		drawable);
};
void DynamicQuadCallback::update(osg::NodeVisitor*,
	osg::Drawable* drawable)
{
	osg::Geometry* quad = static_cast<osg::Geometry*>(drawable);
	if (!quad) return;

	osg::Vec3Array* vertices = static_cast<osg::Vec3Array*>(
		quad->getVertexArray());
	if (!vertices) return;

	osg::Quat quat(osg::PI*0.01, osg::X_AXIS);
	vertices->back() = quat * vertices->back();

	quad->dirtyDisplayList();
	quad->dirtyBound();
}

RDrawableDefinition::RDrawableDefinition() : m_color(0, 0, 0, 0), m_smoothenNormals(true)
{
	m_drawNormalArray = new osg::Vec3Array();
	m_drawVertexCol = new osg::Vec3Array();
	setUseDisplayList(false);
	setDataVariance(osg::Object::DYNAMIC);




	//setUpdateCallback(new DynamicQuadCallback);
}

inline RDrawableDefinition::RDrawableDefinition(const RDrawableDefinition & copy, const osg::CopyOp & copyop)
	: osg::Geometry(copy, copyop),
	m_vertices(copy.m_vertices),
	m_edges(copy.m_edges),
	m_edgeloops(copy.m_edgeloops),
	m_faces(copy.m_faces),
	m_instances(copy.m_instances)
{
}

bool RDrawableDefinition::AddInstance(RDrawableInstance * instance)
{
	return RDrawableDefinition::InsertInstance(m_instances.size(), instance);
}

bool RDrawableDefinition::containsInstance(const RDrawableInstance * instance) const
{
	for (RDrawableInstanceCollection::const_iterator itr = m_instances.begin();
		itr != m_instances.end();
		++itr)
	{
		if (itr->get() == instance) return true;
	}
	return false;
}



bool RDrawableDefinition::InsertInstance(unsigned int index, RDrawableInstance * instance)
{
	if (!instance) return false;

	if (containsInstance(instance))
		return false;

	if (index >= m_instances.size())
	{
		index = m_instances.size();      // set correct index value to be passed to the "childInserted" method
		m_instances.push_back(instance);
	}
	else
	{
		m_instances.insert(m_instances.begin() + index, instance);
	}
	instance->SetIndexInDefinition(index);
	return true;
}
inline bool RDrawableDefinition::AddEdge(REdge * edge)
{
	return RDrawableDefinition::Insertedge(m_edges.size(), edge);
}

inline bool RDrawableDefinition::Insertedge(unsigned int index, REdge * edge)
{
	if (!edge) return false;

	if (containsEdge(edge))
		return false;

	if (index >= m_edges.size())
	{
		index = m_edges.size();      // set correct index value to be passed to the "childInserted" method
		m_edges.push_back(edge);
	}
	else
	{
		m_edges.insert(m_edges.begin() + index, edge);
	}

	edge->SetIndex(m_edges.size() - 1);
	return true;

}

inline bool RDrawableDefinition::containsEdge(const REdge * node) const
{
	for (REdgeCollection::const_iterator itr = m_edges.begin();
		itr != m_edges.end();
		++itr)
	{
		if (itr->get() == node) return true;
	}
	return false;
}

bool RDrawableDefinition::AddVertex(RVertex3D * vertex)
{
	return RDrawableDefinition::InsertVertex(m_vertices.size(), vertex);
}

bool RDrawableDefinition::InsertVertex(unsigned int index, RVertex3D * vertex)
{
	if (!vertex) return false;

	if (containsVertex(vertex))
		return false;

	if (index >= m_vertices.size())
	{
		index = m_vertices.size();      // set correct index value to be passed to the "childInserted" method
		m_vertices.push_back(vertex);
	}
	else
	{
		m_vertices.insert(m_vertices.begin() + index, vertex);
	}

	vertex->SetIndex(m_vertices.size() - 1);

	return true;
}

bool RDrawableDefinition::containsVertex(const RVertex3D * vertex) const
{
	for (RVertex3DCollection::const_iterator itr = m_vertices.begin();
		itr != m_vertices.end();
		++itr)
	{
		if (itr->get() == vertex) return true;
	}
	return false;
}

bool RDrawableDefinition::AddFace(RFace * face)
{
	return RDrawableDefinition::InsertFace(m_faces.size(), face);
}

bool RDrawableDefinition::InsertFace(unsigned int index, RFace * face)
{
	if (!face) return false;

	if (containsFace(face))
		return false;

	if (index >= m_faces.size())
	{
		index = m_faces.size();      // set correct index value to be passed to the "childInserted" method
		m_faces.push_back(face);
	}
	else
	{
		m_faces.insert(m_faces.begin() + index, face);
	}
	face->SetIndex(m_faces.size() - 1);
	face->SetDefinition(this);
	return true;
}

bool RDrawableDefinition::containsFace(const RFace * face) const
{
	for (RFaceCollection::const_iterator itr = m_faces.begin();
		itr != m_faces.end();
		++itr)
	{
		if (itr->get() == face) return true;
	}
	return false;
}

void RDrawableDefinition::Selected(bool isSelected)
{
	//
}

bool RDrawableDefinition::Redraw(bool initDraw)
{

	if (initDraw)
	{

		m_material = new osg::Material;

		m_material->setAmbient(osg::Material::FRONT, osg::Vec4(0.24725, 0.2245, 0.0645, 1.0));
		m_material->setDiffuse(osg::Material::FRONT, osg::Vec4(0.34615, 0.3143, 0.0903, 1.0));
		m_material->setSpecular(osg::Material::FRONT, osg::Vec4(0.797357, 0.723991, 0.208006, 1.0));
		m_material->setEmission(osg::Material::FRONT, osg::Vec4(0.0, 0.0, 0.0, 0.0));
		m_material->setShininess(osg::Material::FRONT, 83.2);

		getOrCreateStateSet()->setAttribute(m_material);

		m_drawNormalArray = new osg::Vec3Array();
		m_drawVertexCol = new osg::Vec3Array();
		osg::Vec4Array *colors = new osg::Vec4Array;

		//smoothening not working
		m_smoothenNormals = false;
		//optimised with multiple vertices
		if (true)
		{
			for (auto f : m_faces)
			{
				int count = f->GetVertices().size();
				osg::ref_ptr<osg::DrawElementsUInt> faceElements;

				if (count == 3)
				{
					faceElements = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 3);
				}
				else if (count == 4)
				{
					faceElements = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 4);
				}
				else
				{
					return false;
				}
				int vcount = 0;
				for (auto v : f->GetVertices())
				{
					int index = 0;
					int smooth = 0;
					if (v->GetRenderIndex() == -1)
					{
						index = m_drawVertexCol->size();
						m_drawVertexCol->push_back(v->GetOsgVec());
						v->SetRenderIndex(index);
						osg::Vec3 n = f->GetFaceNormal();
						n.normalize();
						m_drawNormalArray->push_back(n);
						smooth = 0;
					}
					else
					{
						index = v->GetRenderIndex();
						if (m_smoothenNormals)
						{
							(*m_drawNormalArray)[index] = v->GetSmoothNormal();
							smooth = 1;
						}
						else
						{
							index = m_drawVertexCol->size();
							m_drawVertexCol->push_back(v->GetOsgVec());
							osg::Vec3 n = f->GetFaceNormal();
							n.normalize();
							m_drawNormalArray->push_back(n);
						}
					}
					f->GetVertexRenderIndex().push_back(index);
					f->GetSmoothenVertexIndex().push_back(smooth);
					faceElements->push_back(index);
					vcount++;
				}
				if (!HasColor())
					colors->push_back(f->GetColor());
				addPrimitiveSet(faceElements.get());
			}
		}


		if (false)
		{



			for (auto f : m_faces)
			{
				int count = f->GetVertices().size();

				if (count == 3)
				{
					osg::ref_ptr<osg::DrawElementsUInt> faceElements(new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 3));
					osg::DrawElementsUInt::iterator faceElementsItr(faceElements->begin());

					for (auto v : f->GetVertices())
					{
						*(faceElementsItr++) = v->GetIndex();
					}

					if (!HasColor())
						colors->push_back(f->GetColor());

					addPrimitiveSet(faceElements.get());

				}
				else if (count == 4)
				{
					osg::ref_ptr<osg::DrawElementsUInt> faceElements(new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 4));
					osg::DrawElementsUInt::iterator faceElementsItr(faceElements->begin());

					for (auto v : f->GetVertices())
					{
						*(faceElementsItr++) = v->GetIndex();
					}

					if (!HasColor())
						colors->push_back(f->GetColor());

					addPrimitiveSet(faceElements.get());
				}

			}
		}


		if (false)
		{
			for (auto f : m_faces)
			{
				int count = f->GetVertices().size();
				osg::ref_ptr<osg::DrawElementsUInt> faceElements;

				if (count == 3)
				{
					faceElements = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 3);
				}
				else if (count == 4)
				{
					faceElements = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 4);
				}
				else
				{
					return false;
				}
				osg::Vec3 n = f->GetFaceNormal();
				n.normalize();
				for (auto v : f->GetVertices())
				{
					faceElements->push_back(m_drawVertexCol->size());
					m_drawVertexCol->push_back(v->GetOsgVec());
					m_drawNormalArray->push_back(n);
				}
				if (!HasColor())
					colors->push_back(f->GetColor());
				addPrimitiveSet(faceElements.get());
			}

		}



		setVertexArray(m_drawVertexCol);

		setNormalArray(m_drawNormalArray);

		setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

		setColorArray(colors);

		if (HasColor())
		{
			colors->push_back(GetColor());
			setColorBinding(osg::Geometry::BIND_OVERALL);
		}
		else
			setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);
	}
	else
	{
		/*int size = m_drawVertexCol->size();
		m_drawVertexCol->clear();
		m_drawNormalArray->clear();

		m_drawVertexCol->reserve(size);
		m_drawNormalArray->reserve(size);*/

		for (auto f : m_faces)
		{
			auto n = f->GetFaceNormal();
			n.normalize();
			int vcount = 0;
			for (int i : f->GetVertexRenderIndex())
			{
				auto v = f->GetVertices()[vcount]->GetOsgVec();

				m_drawVertexCol->insert(m_drawVertexCol->begin() + i, v);
				m_drawNormalArray->insert(m_drawNormalArray->begin() + i, n);
				vcount++;
			}
		}

		m_drawNormalArray->dirty();
		m_drawVertexCol->dirty();
	}

	for (auto inst : m_instances)
	{
		inst->Redraw(initDraw);
	}

	return true;
}


bool RDrawableDefinition::AddEdgeLoop(REdgeloop * edgeloop)
{
	return RDrawableDefinition::InsertedgeLoop(m_edgeloops.size(), edgeloop);
}

bool RDrawableDefinition::InsertedgeLoop(unsigned int index, REdgeloop * edgeloop)
{
	if (!edgeloop) return false;

	if (containsEdgeLoop(edgeloop))
		return false;

	if (index >= m_edgeloops.size())
	{
		index = m_edgeloops.size();      // set correct index value to be passed to the "childInserted" method
		m_edgeloops.push_back(edgeloop);
	}
	else
	{
		m_edgeloops.insert(m_edgeloops.begin() + index, edgeloop);
	}
	return true;
}

bool RDrawableDefinition::containsEdgeLoop(const REdgeloop * edgeloop) const
{
	for (REdgeLoopCollection::const_iterator itr = m_edgeloops.begin();
		itr != m_edgeloops.end();
		++itr)
	{
		if (itr->get() == edgeloop) return true;
	}
	return false;
}


RDrawableDefinition::~RDrawableDefinition()
{
}
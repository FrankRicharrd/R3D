#include "stdafx.h"
#include "RModelingManager.h"
#include "RModelingManagerInterop.h"
#include "ImpRTransformNode.h"
#include "ObjectFactory.h"
#include "RDrawingEntity.h"
#include "ImpRGeometryNode.h"
#include <algorithm>
#include <osg/BlendFunc>
#include "RGeomInstanceInterop.h"
using namespace System::IO;
using namespace System::Xml;
using namespace System::Xml::Serialization;
namespace ViewerInterop {
	RModelingManagerInterop::RModelingManagerInterop(RModelingManager*  modeling)
	{
		m_modelingManager = modeling;
		CalculateLines = true;
		/*ShareVbos = false;*/
		CreateOneMeshPerDefinition = false;
	}

	Viewer::Interfaces::IRTransformNode ^ RModelingManagerInterop::CreateSphere(double radius, int rings, int sector)
	{
		auto geomNode = m_modelingManager->CreateSphere(radius, rings, sector);

		m_modelingManager->GetData()->GetGeomRootNode()->addChild(geomNode);
		return gcnew RGeomInstanceInterop(geomNode);
	}

	void RModelingManagerInterop::CreateCustomData(System::String ^fileName, System::Object ^v)
	{
		createcustomData(v);

		return;
	}



	void RModelingManagerInterop::CreateCustomDataPerDef(RExternalData::RevitData ^ rdata)
	{
		RGeomDefinitionRef mainDef = new RGeomDefinition();
		mainDef->SetName("ModelRoot");

		auto n = mainDef->getName();


		for each (auto def in rdata->Definitions)
		{
			RGeomDefinitionRef rdef = new RGeomDefinition();
			osg::ref_ptr<osg::Vec3Array> m_drawVertexCol = new osg::Vec3Array();
			osg::ref_ptr<osg::Vec3Array> m_drawNormalArray = new osg::Vec3Array();
			osg::ref_ptr<osg::Vec4Array> ColorArray = new osg::Vec4Array();

			string name;
			InteropUtilities::MarshalString(def->Name, name);
			rdef->SetName(name);

			RMeshRef defMesh = rdef->GetMesh();

			bool isDummyElement = true;

			if (def->MeshFace != nullptr || def->MeshFace->Count > 0)
			{
				for each (auto meshFace in def->MeshFace)
				{
					isDummyElement = false;
					auto rf = createmeshPerDef(meshFace->Mesh, defMesh, meshFace->MaterialID, rdata, m_drawVertexCol, m_drawNormalArray, ColorArray);
				}
			}

			for each (auto f in def->Faces)
			{
				if (f->Mesh == nullptr)
					continue;
				isDummyElement = false;
				auto rf = createmeshPerDef(f->Mesh, defMesh, f->MaterialID, rdata, m_drawVertexCol, m_drawNormalArray, ColorArray);

			}


#pragma region Instances
			for each (auto inst in def->Instances)
			{

				RGeomInstanceRef ri = rdef->CreateInstance();



				osg::Matrix mat(inst->m0, inst->m1, inst->m2, inst->m3,
					inst->m4, inst->m5, inst->m6, inst->m7,
					inst->m8, inst->m9, inst->m10, inst->m11,
					inst->m12, inst->m13, inst->m14, inst->m15);

				osg::Matrix mt;
				mt.setTrans(osg::Vec3d(inst->m3, inst->m7, inst->m11));



				ri->setMatrix(mat);

				mainDef->AddGeomInstanceChild(ri);
			}

			if (!isDummyElement && (def->Instances == nullptr || def->Instances->Count == 0))
			{
				bool status = mainDef->AddGeomInstanceChild(rdef->CreateInstance());
			}
#pragma endregion




		}

		auto mainInst = mainDef->CreateInstance();

		//90 degree rotation on x axis
		osg::Matrix mm(
			1, 0, 0, 0,
			0, 0, -1, 0,
			0, 1, 0, 0,
			0, 0, 0, 1);

		mainInst->setMatrix(mm);

		n = mainInst->GetDefinitionName();

		if (m_modelingManager->GetData()->GetGeomRootNode().valid())
		{
			m_modelingManager->GetData()->GetGeomRootNode()->addChild(mainInst);
		}

	

	}


	void RModelingManagerInterop::createcustomData(System::Object ^ v)
	{
		RExternalData::RevitData ^ rdata = (RExternalData::RevitData^)v;

		if (rdata == nullptr)
		{
			throw gcnew Exception("data null");
		}


		CreateCustomDataPerDef(rdata);

		return;

#pragma region Old
		RGeomDefinitionRef mainDef = new RGeomDefinition();
		mainDef->SetName("MainParent");



		for each (auto def in rdata->Definitions)
		{
			RGeomDefinitionRef rdef = new RGeomDefinition();
			osg::ref_ptr<osg::Vec3Array> m_drawVertexCol = new osg::Vec3Array();
			osg::ref_ptr<osg::Vec3Array> m_drawNormalArray = new osg::Vec3Array();
			string name;
			InteropUtilities::MarshalString(def->Name, name);
			rdef->SetName(name);

			RMeshRef defMesh = rdef->GetMesh();


			if (def->Faces->Count == 0)
			{
				if (def->MeshFace != nullptr || def->MeshFace->Count > 0)
				{
					for each (auto mesh in def->MeshFace)
					{

						auto rf = createmesh(mesh->Mesh, defMesh, mesh->MaterialID, rdata, m_drawVertexCol, m_drawNormalArray);

						if (!CreateOneMeshPerDefinition)
						{
							if (rf.valid())
								bool status = rdef->AddGeomEntity(rf);
						}
					}
				}
				else
				{

				}
				for each (auto inst in def->Instances)
				{
					/*auto m = gcnew System::Collections::Generic::List<double>();

					m = inst->Matrix;
					*/
					RGeomInstanceRef ri = rdef->CreateInstance();



					osg::Matrix mat(inst->m0, inst->m1, inst->m2, inst->m3,
						inst->m4, inst->m5, inst->m6, inst->m7,
						inst->m8, inst->m9, inst->m10, inst->m11,
						inst->m12, inst->m13, inst->m14, inst->m15);

					osg::Matrix mt;
					mt.setTrans(osg::Vec3d(inst->m3, inst->m7, inst->m11));



					ri->setMatrix(mat);

					mainDef->AddGeomInstanceChild(ri);
				}

				if (def->Instances == nullptr || def->Instances->Count == 0)
				{
					bool status = mainDef->AddGeomInstanceChild(rdef->CreateInstance());
				}
			}
			else
			{
				for each (auto f in def->Faces)
				{
					if (f->Mesh == nullptr)
						continue;

					auto rf = createmesh(f->Mesh, defMesh, f->MaterialID, rdata, m_drawVertexCol, m_drawNormalArray);

					if (!CreateOneMeshPerDefinition)
					{
						if (rf.valid())
						{
							bool status = rdef->AddGeomEntity(rf);

							for (auto el : rf->GetOuterEdgeLoops())
							{
								bool status = rdef->AddGeomEntity(el);

								System::Diagnostics::Debug::WriteLine("Drawn");
							}
						}
					}
				}

				for each (auto inst in def->Instances)
				{

					RGeomInstanceRef ri = rdef->CreateInstance();



					osg::Matrix mat(inst->m0, inst->m1, inst->m2, inst->m3,
						inst->m4, inst->m5, inst->m6, inst->m7,
						inst->m8, inst->m9, inst->m10, inst->m11,
						inst->m12, inst->m13, inst->m14, inst->m15);

					osg::Matrix mt;
					mt.setTrans(osg::Vec3d(inst->m3, inst->m7, inst->m11));



					ri->setMatrix(mat);

					mainDef->AddGeomInstanceChild(ri);
				}

				if (def->Instances == nullptr || def->Instances->Count == 0)
				{
					bool status = mainDef->AddGeomInstanceChild(rdef->CreateInstance());
				}
			}

			if (CreateOneMeshPerDefinition)
			{

				//todo make all edges in single drawable.
				for (auto el : defMesh->GetOuterEdgeLoops())
				{
					bool status = rdef->AddGeomEntity(el);

					System::Diagnostics::Debug::WriteLine("Drawn");
				}
			}

		}


		auto mainInst = mainDef->CreateInstance();

		//90 degree rotation on x axis
		osg::Matrix mm(
			1, 0, 0, 0,
			0, 0, -1, 0,
			0, 1, 0, 0,
			0, 0, 0, 1);

		mainInst->setMatrix(mm);

		m_modelingManager->GetData()->GetRootParentNodeAsTransformNode()->addChild(mainInst);
#pragma endregion


	}

	RMeshRef RModelingManagerInterop::createmeshPerDef(RExternalData::RMesh ^ mesh, RMeshRef defMesh, String^ materialID, RExternalData::RevitData ^ rdata,
		osg::ref_ptr<osg::Vec3Array> m_drawVertexCol, osg::ref_ptr<osg::Vec3Array> m_drawNormalArray, osg::ref_ptr<osg::Vec4Array> colorArray)
	{
		RMeshRef rf = defMesh;
		int vertexInitialCount = m_drawVertexCol->size();

		osg::Vec4 col;

		auto mat = GetMaterial(rdata, materialID);
		if (mat != nullptr)
		{
			double transparency = 1;
			//0 completely transparent and 1 means opaque
			if (mat->Transparency > 0)
			{
				rf->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
				rf->getOrCreateStateSet()->setAttributeAndModes(new osg::BlendFunc);

				transparency = (1.00 - ((double)mat->Transparency / 100.00));
			}

			double r = mat->Color->Red;
			double g = mat->Color->Green;
			double b = mat->Color->Blue;

			col.set(r, g, b, transparency);

			auto material = CreateOsgMaterial(mat);

			rf->getOrCreateStateSet()->setAttributeAndModes(material, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

		}
		else
		{
			col.set(0.5, 0.5, 0.5, 1);
		}



		int count = 0;
		for each (auto v in mesh->Vertices)
		{
			m_drawVertexCol->push_back(osg::Vec3(v->x, v->y, v->z));
			colorArray->push_back(col);


			if (mesh->NormalDistribution == RExternalData::RDistributionOfNormals::OnEachFacet)
			{
				/*int facetCount = 0;
				for each (auto i in mesh->Facets)
				{
				auto n = mesh->Normals[0];

				m_drawNormalArray->push_back(osg::Vec3(n->x, n->y, n->z));

				facetCount++;
				}*/
			}
			else if (mesh->NormalDistribution == RExternalData::RDistributionOfNormals::AtEachPoint)
			{
				auto n = mesh->Normals[count];
				m_drawNormalArray->push_back(osg::Vec3(n->x, n->y, n->z));
			}
			else if (mesh->NormalDistribution == RExternalData::RDistributionOfNormals::OnePerFace)
			{
				auto n = mesh->Normals[0];
				m_drawNormalArray->push_back(osg::Vec3(n->x, n->y, n->z));
			}

			count++;
		}

		osg::ref_ptr<osg::DrawElementsUInt> faceElements;
		faceElements = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, mesh->Facets->Count * 3);
		for each (auto i in mesh->Facets)
		{
			faceElements->push_back(i->V1 + vertexInitialCount);
			faceElements->push_back(i->V2 + vertexInitialCount);
			faceElements->push_back(i->V3 + vertexInitialCount);
		}


		rf->addPrimitiveSet(faceElements);
		rf->setVertexArray(m_drawVertexCol);
		rf->setNormalArray(m_drawNormalArray);
		rf->setColorArray(colorArray);

		rf->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
		rf->setColorBinding(osg::Geometry::BIND_PER_VERTEX);



		return rf;
	}

	RMeshRef RModelingManagerInterop::createmesh(RExternalData::RMesh ^ mesh, RMeshRef defMesh, String^ materialID, RExternalData::RevitData ^ rdata, osg::ref_ptr<osg::Vec3Array> m_drawVertexCol, osg::ref_ptr<osg::Vec3Array> m_drawNormalArray)
	{
		RMeshRef rf = defMesh;

		if (!CreateOneMeshPerDefinition)
			rf = new RMesh();

		int vertexInitialCount = m_drawVertexCol->size();

		if (!ShareVbos)
		{
			m_drawVertexCol = new osg::Vec3Array();
			m_drawNormalArray = new osg::Vec3Array();
			vertexInitialCount = 0;

		}
		for each (auto v in mesh->Vertices)
		{
			m_drawVertexCol->push_back(osg::Vec3(v->x, v->y, v->z));
		}


		if (ShareVbos)
		{
			if (mesh->NormalDistribution == RExternalData::RDistributionOfNormals::OnEachFacet)
			{
				/*int facetCount = 0;
				for each (auto i in mesh->Facets)
				{
					auto n = mesh->Normals[0];

					m_drawNormalArray->push_back(osg::Vec3(n->x, n->y, n->z));

					facetCount++;
				}*/
			}
			else if (mesh->NormalDistribution == RExternalData::RDistributionOfNormals::AtEachPoint)
			{
				for each (auto n in mesh->Normals)
				{
					m_drawNormalArray->push_back(osg::Vec3(n->x, n->y, n->z));
				}
			}
			else if (mesh->NormalDistribution == RExternalData::RDistributionOfNormals::OnePerFace)
			{
				for each (auto var in mesh->Vertices)
				{
					auto n = mesh->Normals[0];

					m_drawNormalArray->push_back(osg::Vec3(n->x, n->y, n->z));
				}
			}
		}
		else
		{
			for each (auto n in mesh->Normals)
			{
				m_drawNormalArray->push_back(osg::Vec3(n->x, n->y, n->z));
			}
		}



		std::set<string> allEdges;

		if (CalculateLines)
		{
			for each (auto i in mesh->Facets)
			{
				int v1 = i->V1;
				int v2 = i->V2;
				int v3 = i->V3;

				string e1 = std::to_string(v1) + "_" + std::to_string(v2);
				string e2 = std::to_string(v2) + "_" + std::to_string(v3);
				string e3 = std::to_string(v3) + "_" + std::to_string(v1);

				allEdges.insert(e1);
				allEdges.insert(e2);
				allEdges.insert(e3);
			}
		}

		std::vector<int> outerEdgeIndices;


		osg::ref_ptr<osg::DrawElementsUInt> faceElements;
		faceElements = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, mesh->Facets->Count * 3);
		for each (auto i in mesh->Facets)
		{
			faceElements->push_back(i->V1 + vertexInitialCount);
			faceElements->push_back(i->V2 + vertexInitialCount);
			faceElements->push_back(i->V3 + vertexInitialCount);

			if (CalculateLines)
			{
				string oe1 = std::to_string(i->V2) + "_" + std::to_string(i->V1);
				string oe2 = std::to_string(i->V3) + "_" + std::to_string(i->V2);
				string oe3 = std::to_string(i->V1) + "_" + std::to_string(i->V3);

				bool e1 = allEdges.find(oe1) == allEdges.end() ? false : true;
				bool e2 = allEdges.find(oe2) == allEdges.end() ? false : true;
				bool e3 = allEdges.find(oe3) == allEdges.end() ? false : true;

				if (!e1)
				{
					outerEdgeIndices.push_back(i->V1 + vertexInitialCount);
					outerEdgeIndices.push_back(i->V2 + vertexInitialCount);
				}
				if (!e2)
				{
					outerEdgeIndices.push_back(i->V2 + vertexInitialCount);
					outerEdgeIndices.push_back(i->V3 + vertexInitialCount);
				}
				if (!e3)
				{
					outerEdgeIndices.push_back(i->V3 + vertexInitialCount);
					outerEdgeIndices.push_back(i->V1 + vertexInitialCount);
				}
			}

		}



		rf->addPrimitiveSet(faceElements);

		rf->setVertexArray(m_drawVertexCol);

		rf->setNormalArray(m_drawNormalArray);

		if (ShareVbos)
		{
			//if (mesh->NormalDistribution == RExternalData::RDistributionOfNormals::AtEachPoint)
			rf->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
			/*	else if (mesh->NormalDistribution == RExternalData::RDistributionOfNormals::OnEachFacet)
					rf->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);
				else if (mesh->NormalDistribution == RExternalData::RDistributionOfNormals::OnePerFace)
					rf->setNormalBinding(osg::Geometry::BIND_OVERALL);*/
		}
		else
		{

			if (mesh->NormalDistribution == RExternalData::RDistributionOfNormals::AtEachPoint)
				rf->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
			else if (mesh->NormalDistribution == RExternalData::RDistributionOfNormals::OnEachFacet)
				rf->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);
			else if (mesh->NormalDistribution == RExternalData::RDistributionOfNormals::OnePerFace)
				rf->setNormalBinding(osg::Geometry::BIND_OVERALL);
		}
		osg::Vec4Array *colors = new osg::Vec4Array;

		auto mat = GetMaterial(rdata, materialID);
		if (mat != nullptr)
		{
			double transparency = 1;
			//0 completely transparent and 1 means opaque
			if (mat->Transparency > 0)
			{
				rf->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
				rf->getOrCreateStateSet()->setAttributeAndModes(new osg::BlendFunc);

				transparency = (1.00 - ((double)mat->Transparency / 100.00));

			}

			/*double r = max(0, min(255, (int)floor(mat->Color->Red* 256.0)));
			double g = max(0, min(255, (int)floor(mat->Color->Green* 256.0)));
			double b = max(0, min(255, (int)floor(mat->Color->Blue* 256.0)));
*/
			double r = mat->Color->Red;
			double g = mat->Color->Green;
			double b = mat->Color->Blue;



			osg::Vec4 col(r, g, b, transparency);
			colors->push_back(col);

			osg::Material *material = new osg::Material();

			material->setDiffuse(osg::Material::FRONT, col);

			material->setSpecular(osg::Material::FRONT, osg::Vec4(0.1, 0.1, 0.1, 1.0));

			material->setAmbient(osg::Material::FRONT, osg::Vec4(0, 0, 0, 1.0));

			material->setEmission(osg::Material::FRONT, osg::Vec4(0.0, 0.0, 0.0, 1.0));

			material->setShininess(osg::Material::FRONT, mat->Shininess);

			material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);

			rf->getOrCreateStateSet()->setAttributeAndModes(material, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

			/*	rf->setColorArray(colors);
				rf->setColorBinding(osg::Geometry::BIND_OVERALL);*/
		}
		else
		{
			osg::Vec4 col(0.5, 0.5, 0.5, 0);
			colors->push_back(col);

			rf->setColorArray(colors);
			rf->setColorBinding(osg::Geometry::BIND_OVERALL);
		}

		if (outerEdgeIndices.size() > 0 && CalculateLines)
		{
			RGeomEdgeLoopRef el = new RGeomEdgeLoop(rf);

			el->setVertexArray(m_drawVertexCol.get());

			osg::ref_ptr<osg::DrawElementsUInt> lineElements;
			lineElements = new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, outerEdgeIndices.size());

			lineElements->insert(lineElements->end(), outerEdgeIndices.begin(), outerEdgeIndices.end());

			int siz = lineElements->size();

			System::Diagnostics::Debug::WriteLine("size " + siz);

			el->addPrimitiveSet(lineElements);
			osg::Vec4Array *cols = new osg::Vec4Array;
			osg::Vec4 cole(0, 0, 0, 0);
			cols->push_back(cole);
			el->setColorArray(cols);
			el->setColorBinding(osg::Geometry::BIND_OVERALL);

			el->getOrCreateStateSet()->setAttribute(
				new osg::LineWidth(2));

			osg::Material *lm = new osg::Material();

			lm->setDiffuse(osg::Material::FRONT, cole);

			lm->setSpecular(osg::Material::FRONT, osg::Vec4(0.1, 0.1, 0.1, 1.0));

			lm->setAmbient(osg::Material::FRONT, osg::Vec4(0, 0, 0, 1.0));

			lm->setEmission(osg::Material::FRONT, osg::Vec4(0.0, 0.0, 0.0, 1.0));

			//lm->setShininess(osg::Material::FRONT, mat->Shininess);

			lm->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);

			el->getOrCreateStateSet()->setAttributeAndModes(lm, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

			rf->AddEdgeLoop(el);




		}

		return rf;
	}

	osg::ref_ptr<osg::Material> RModelingManagerInterop::CreateOsgMaterial(RExternalData::RMaterial^ mat)
	{
		osg::ref_ptr<osg::Material> material = new osg::Material();
		material->setDiffuse(osg::Material::FRONT, osg::Vec4(0.1, 0.1, 0.1, 1.0));
		material->setSpecular(osg::Material::FRONT, osg::Vec4(0.1, 0.1, 0.1, 1.0));
		material->setAmbient(osg::Material::FRONT, osg::Vec4(0, 0, 0, 1.0));
		material->setEmission(osg::Material::FRONT, osg::Vec4(0.0, 0.0, 0.0, 1.0));
		material->setShininess(osg::Material::FRONT, mat->Shininess);
		material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);

		return material.release();
	}

	RExternalData::RMaterial ^ RModelingManagerInterop::GetMaterial(RExternalData::RevitData ^ rdata, String ^ id)
	{
		for each (auto var in rdata->Materials)
		{
			if (var->ID == id)
				return var;

		}
		return nullptr;
	}

	System::String ^ RModelingManagerInterop::GetNumberOfDrawables()
	{

		int count = GetDrawables(m_modelingManager->GetData()->GetRootParentNodeAsTransformNode());

		return count.ToString();
	}

	int RModelingManagerInterop::GetDrawables(osg::ref_ptr<osg::Group> group)
	{
		if (!group.valid())
			return 0;

		int totalC = 0;

		for (size_t i = 0; i < group->getNumChildren(); i++)
		{
			osg::Node* node = group->getChild(i);

			if (!node)
				continue;

			if (node->asGroup())
			{
				int c = GetDrawables(node->asGroup());
				totalC = totalC + c;
			}
			else if (node->asDrawable())
			{
				totalC++;
			}
		}

		return totalC;
	}

}
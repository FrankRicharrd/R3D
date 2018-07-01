#include "stdafx.h"
#include "RTransformNode.h"
#include "RGeometryNode.h"
#include "RNode.h"
#include <RIntelligentObject.h>
#include "RIntelligentObjectManagerInterop.h"
#include "RIODrawingEntities.h"

using namespace ViewerInterop;

namespace R3DIOInterop
{
	RIntelligentObjectManagerInterop::RIntelligentObjectManagerInterop(Viewer::Interfaces::IRTransformNode^ rootParentNode)
	{
		m_rootParentNode = rootParentNode;

		//CRNodeWrapper^ node = (CRNodeWrapper^)rootParentNode->GetWrappedNativeObject();

		/*CRNode* cnode = node->GetNativeObject();

		 OsgNode onode = cnode->GetOsgNode();*/

		 m_intelligentObjectManager = new RIntelligentObjectManager();
	}

	void RIntelligentObjectManagerInterop::CreateObj(osg::ref_ptr<RIntelligentObject> obj)
	{
		RVertex3DCollection vertices;
		RVertex3DRef v0 = new RVertex3D(0, 0, 0);
		RVertex3DRef v1 = new RVertex3D(1, 0, 0);
		RVertex3DRef v2 = new RVertex3D(1, 1, 0);
		RVertex3DRef v3 = new RVertex3D(0, 1, 0);
		RVertex3DRef v4 = new RVertex3D(0, 0, 1);
		RVertex3DRef v5 = new RVertex3D(1, 0, 1);
		RVertex3DRef v6 = new RVertex3D(1, 1, 1);
		RVertex3DRef v7 = new RVertex3D(0, 1, 1);
		vertices.push_back(v0);
		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);
		vertices.push_back(v4);
		vertices.push_back(v5);
		vertices.push_back(v6);
		vertices.push_back(v7);
		osg::ref_ptr<RIntelliDrawableDef> def = new RIntelliDrawableDef(vertices, obj);

		REdgeRef e1 = new REdge(v0, v1);
		REdgeRef e2 = new REdge(v1, v2);
		REdgeRef e3 = new REdge(v2, v3);
		REdgeRef e4 = new REdge(v3, v0);

		REdgeRef e5 = new REdge(v1, v5);
		REdgeRef e6 = new REdge(v2, v6);
		REdgeRef e7 = new REdge(v3, v7);
		REdgeRef e8 = new REdge(v0, v4);

		REdgeRef e9 = new REdge(v4, v5);
		REdgeRef e10 = new REdge(v5, v6);
		REdgeRef e11 = new REdge(v6, v7);
		REdgeRef e12 = new REdge(v7, v4);

		def->AddEdge(e1);
		def->AddEdge(e2);
		def->AddEdge(e3);
		def->AddEdge(e4);
		def->AddEdge(e5);
		def->AddEdge(e6);
		def->AddEdge(e7);
		def->AddEdge(e8);
		def->AddEdge(e9);
		def->AddEdge(e10);
		def->AddEdge(e11);
		def->AddEdge(e12);






		RIOFaceRef f1 = new RIOFace();
		REdgeloopRef el1 = new REdgeloop(f1);
		el1->AddEdge(e1, 0);
		el1->AddEdge(e2, 0);
		el1->AddEdge(e3, 0);
		el1->AddEdge(e4, 0);
		f1->AddEdgeLoop(el1);

		RIOFaceRef f2 = new RIOFace();
		REdgeloopRef el2 = new REdgeloop(f2);
		el2->AddEdge(e5, 0);
		el2->AddEdge(e10, 0);
		el2->AddEdge(e6, 1);
		el2->AddEdge(e2, 1);
		f2->AddEdgeLoop(el2);


		RIOFaceRef f3 = new RIOFace();
		REdgeloopRef el3 = new REdgeloop(f3);
		el3->AddEdge(e10, 1);
		el3->AddEdge(e9, 1);
		el3->AddEdge(e12, 1);
		el3->AddEdge(e11, 1);
		f3->AddEdgeLoop(el3);


		RIOFaceRef f4 = new RIOFace();
		REdgeloopRef el4 = new REdgeloop(f4);
		el4->AddEdge(e12, 0);
		el4->AddEdge(e8, 1);
		el4->AddEdge(e4, 1);
		el4->AddEdge(e7, 0);
		f4->AddEdgeLoop(el4);

		RIOFaceRef f5 = new RIOFace();
		REdgeloopRef el5 = new REdgeloop(f5);
		el5->AddEdge(e7, 1);
		el5->AddEdge(e3, 1);
		el5->AddEdge(e6, 0);
		el5->AddEdge(e11, 0);
		f5->AddEdgeLoop(el5);


		RIOFaceRef f6 = new RIOFace();
		REdgeloopRef el6 = new REdgeloop(f6);
		el6->AddEdge(e8, 0);
		el6->AddEdge(e1, 1);
		el6->AddEdge(e5, 1);
		el6->AddEdge(e9, 0);
		f6->AddEdgeLoop(el6);


		def->AddFace(f1);
		def->AddFace(f2);
		def->AddFace(f3);
		def->AddFace(f4);
		def->AddFace(f5);
		def->AddFace(f6);

		def->AddFaceDragger(f1->GetIndex());
		def->AddFaceDragger(f2->GetIndex());
		def->AddFaceDragger(f3->GetIndex());
		def->AddFaceDragger(f4->GetIndex());
		def->AddFaceDragger(f5->GetIndex());
		def->AddFaceDragger(f6->GetIndex());


		osg::ref_ptr<RIntelliDrawableInstance> instance = new RIntelliDrawableInstance(def);

		def->AddInstance(instance);

		def->Redraw(true);
		def->SetDraggerVisibility(true);





		obj->addChild(instance.get());

	}


	Viewer::Interfaces::IRTransformNode ^ RIntelligentObjectManagerInterop::CreateEmpytyObject()
	{
		osg::ref_ptr<RIntelligentObject> obj = new RIntelligentObject();

		CreateObj(obj);

		osg::ref_ptr< osg::MatrixTransform> mat = new osg::MatrixTransform();

		CRTransformNode* node = new CRTransformNode(mat);

		CRGeometryNode* geomNode = new CRGeometryNode(obj);


		ViewerInterop::ImpRTransformNode^ transnode = (ViewerInterop::ImpRTransformNode^)ViewerInterop::ObjectFactory::CreateTransformNode(node);
		ViewerInterop::ImpRGeometryNode^ geomNodeInterop = (ViewerInterop::ImpRGeometryNode^)ViewerInterop::ObjectFactory::CreateGeometryNode(geomNode);

		transnode->AddChild(geomNodeInterop);

		return transnode;

	}
	RIntelligentObjectInterop ^ RIntelligentObjectManagerInterop::CreateIntelligentObject()
	{

		osg::ref_ptr<RIntelligentObject> obj = new RIntelligentObject();


		osg::ref_ptr< osg::MatrixTransform> mat = new osg::MatrixTransform();

		CRTransformNode* node = new CRTransformNode(mat);

		CRGeometryNode* geomNode = new CRGeometryNode(obj);


		ViewerInterop::ImpRTransformNode^ transnode = (ViewerInterop::ImpRTransformNode^)ViewerInterop::ObjectFactory::CreateTransformNode(node);
		ViewerInterop::ImpRGeometryNode^ geomNodeInterop = (ViewerInterop::ImpRGeometryNode^)ViewerInterop::ObjectFactory::CreateGeometryNode(geomNode);

		transnode->AddChild(geomNodeInterop);

		m_rootParentNode->AddChild(transnode);

		return gcnew RIntelligentObjectInterop(obj);

	}
	RIntelligentObjectManagerInterop::~RIntelligentObjectManagerInterop()
	{

	}
}
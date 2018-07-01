#include "stdafx.h"
#include "ObjectFactory.h"
#include "ImpRGeometryNode.h"
#include "ImpRTransformNode.h"

using namespace ViewerInterop;



namespace ViewerInterop
{
	ObjectFactory::ObjectFactory()
	{
	}

	IRGeometryNode ^ ObjectFactory::CreateGeometryNode(CRGeometryNode * node)
	{

		ImpRGeometryNode^ gnode = gcnew ImpRGeometryNode(node);

		return gnode;


	}

	IRTransformNode ^ ObjectFactory::CreateTransformNode(CRTransformNode * node)
	{

		ImpRTransformNode^ gnode = gcnew ImpRTransformNode(node);


		return gnode;

	}

	


	IRTransformNode ^ ObjectFactory::CreateTransformNode(CRGeometryNode * node, CRTransformNode* parentNode)
	{
		osg::ref_ptr<osg::MatrixTransform> mat = new osg::MatrixTransform();

		CRTransformNode* transNode = new CRTransformNode(mat);

		ImpRGeometryNode^ geomInt = gcnew ImpRGeometryNode(node);
		ImpRTransformNode^ transNodeImp = gcnew ImpRTransformNode(transNode);

		transNodeImp->AddChild(geomInt);

		if (parentNode != NULL)
		{
			parentNode->AddChild(transNode);
		}

		return transNodeImp;
	}

	IRTransformNode ^ ObjectFactory::CreateTransformNode(void * crgeomnode, void * crtransformParentnode)
	{
		
		CRGeometryNode* geomNode = static_cast<CRGeometryNode*>(crgeomnode);
		if (geomNode == NULL)
			return nullptr;
		CRTransformNode* parentNode = static_cast<CRTransformNode*>(crtransformParentnode);
		if (parentNode == NULL)
			return nullptr;

		return CreateTransformNode(geomNode, parentNode);
	}

	IRGeometryNode ^ ObjectFactory::CreateGeometryNode(void * node)
	{
		CRGeometryNode* geomnode = static_cast<CRGeometryNode*>(node);
		if (geomnode == NULL)
			return nullptr;

		return CreateGeometryNode(geomnode);
	}

	IRTransformNode ^ ObjectFactory::CreateTransformNode(void * node)
	{
		CRTransformNode* transnode = static_cast<CRTransformNode*>(node);
		if (transnode == NULL)
			return nullptr;

		return CreateTransformNode(transnode);
	}

}

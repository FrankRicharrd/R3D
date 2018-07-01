#include "stdafx.h"
#include "FileSystemsManager.h"
#include "RGeometryNode.h"
#include <ReadObj.h>
#include <osgUtil/Simplifier>

FileSystemsManager::FileSystemsManager(FGlobalData* data)
{
	m_data = data;
}


FileSystemsManager::~FileSystemsManager()
{
}

CRGeometryNode* FileSystemsManager::ImportGeometry(string filePath, bool* status)
{


	OsgNode node = osgDB::readNodeFile(filePath)->asNode();

	if (!node)
	{
		*status = false;
		return nullptr;
	}
	CRGeometryNode* rnode = new  CRGeometryNode(node);
	*status = true;
	return rnode;
}



CRTransformNode* FileSystemsManager::ImportMeshGeometry(string filePath, bool* status)
{

	string extension = filePath.substr(filePath.find_last_of(".") + 1);

	//REGISTER_OSGPLUGIN(obj, ReaderWriterOBJ);
	string ss;
	//RReader reader;

	OsgNode node;



	float sampleRatio = 0.5f;
	float multiplier = 0.8f;
	float minRatio = 0.001f;
	float ratio = sampleRatio;
	float maxError = 4.0f;


	//if (extension == "obj")
	//{
	//	//node = RReader::ReadObjFile(filePath);
	//}
	//else
	{
		node = osgDB::readNodeFile(filePath);
		/*osgUtil::Simplifier simplifier;

		node->accept(simplifier);*/

	}

	//	osg::ref_ptr<osg::Group> node = RReader::ReadObjFile(filePath);
		//OsgNode node = osgDB::readNodeFile(filePath);






	if (!node)
	{
		*status = false;
		return nullptr;
	}
	osg::ref_ptr<osg::MatrixTransform> matTrans = new osg::MatrixTransform();
	matTrans->addChild(node);
	CRTransformNode* rnode = new  CRTransformNode(matTrans);
	*status = true;
	return rnode;
}

RGeomInstanceRef FileSystemsManager::ImportObj(string filePath, string basePath)
{
	ReadObj* oo = new ReadObj(filePath, basePath);
	auto aa =  oo->ReadFile();

	if (aa.valid())
	{
		auto inst = aa->CreateInstance();
		m_data->GetGeomRootNode()->GetDefinition()->AddGeomInstanceChild(inst);

		delete oo;
		return inst;
	}
	else
	{
		delete oo;
		return nullptr;
	}
}


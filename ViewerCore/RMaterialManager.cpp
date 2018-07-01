#include "stdafx.h"
#include "RMaterialManager.h"


RMaterialManager::RMaterialManager(FGlobalData* data)
{
	
	m_data = data;
}

RMaterialManager::~RMaterialManager()
{
}

bool RMaterialManager::SetShaderToNode(string shaderprogram, osg::Shader::Type shaderType, RGeomInstanceRef node)
{
	RGeomInstanceRef geomNode = node;

	if (geomNode == NULL)
	{
		//mat = m_data->GetSelectedNode();

		//if (mat == NULL)
		geomNode = m_data->GetGeomRootNode();
	}

	if (!geomNode.valid())
	{
		return false;
	}


	osg::ref_ptr<osg::Shader> vertShader =
		new osg::Shader(shaderType, shaderprogram);
	/*
	osg::ref_ptr<osg::Program> program = new osg::Program;
	program->addShader(vertShader.get());
	
	geomNode->getOrCreateStateSet()->setAttributeAndModes(program.get());
*/
	geomNode->SetShader(vertShader);

	return true;

}


int RMaterialManager::CompileShader(string shaderprogram, osg::Shader::Type shaderType, string & output)
{
	// implement using native opengl

	return 0;
}

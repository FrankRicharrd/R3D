#pragma once

#include  <osg/Geometry>
#include <osg/Material>
using namespace osg;
#include <osg/Light>
#include <osg/LightSource>
#include <osg/PositionAttitudeTransform>
#include "RDrawingEntity.h"


//void SetDefaultMaterial(osg::Group* rootNode)
//{
//
//	osg::Material *material = new osg::Material();
//
//	material->setDiffuse(osg::Material::FRONT, osg::Vec4(1.0, 1.0, 1.0, 1.0));
//
//	material->setSpecular(osg::Material::FRONT, osg::Vec4(0.0, 0.0, 0.0, 1.0));
//
//	material->setAmbient(osg::Material::FRONT, osg::Vec4(0.1, 0.1, 0.1, 1.0));
//
//	material->setEmission(osg::Material::FRONT, osg::Vec4(0.0, 0.0, 0.0, 1.0));
//
//	material->setShininess(osg::Material::FRONT, 25.0);
//
//	material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
//
//	rootNode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
//
//	rootNode->getOrCreateStateSet()->setAttributeAndModes(material, osg::StateAttribute::ON);
//}


class VIEWER_API RMaterialManager
{
public:
	RMaterialManager(FGlobalData* data);
	~RMaterialManager();

	bool SetShaderToNode(string shaderprogram, osg::Shader::Type shaderType, RGeomInstanceRef node);

	int CompileShader(string shaderprogram, osg::Shader::Type shaderType, string& output);


	FGlobalData* GetData() { return m_data; }

private:

	FGlobalData* m_data;

};



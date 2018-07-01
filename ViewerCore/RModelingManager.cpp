#include "stdafx.h"
#include "RModelingManager.h"
#include "RGeometryNode.h"

RModelingManager::RModelingManager(FGlobalData* data)
{
	m_data = data;
}


RModelingManager::~RModelingManager()
{
}

RGeomInstanceRef RModelingManager::CreateSphere(double radius, const unsigned int rings, const unsigned int sectors)
{
	//osg::ref_ptr<osg::Geode>      sphereGeode = new osg::Geode;
	/*osg::ref_ptr<osg::Geometry>   sphereGeometry = new osg::Geometry;
	osg::ref_ptr<osg::Vec3Array>  sphereVertices = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array>  sphereNormals = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec2Array>  sphereTexCoords = new osg::Vec2Array;
*/
	/*float const R = 1. / static_cast<float>(rings - 1);
	float const S = 1. / static_cast<float>(sectors - 1);
*/

	//sphereGeode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0, 0, 0), radius)));


	//sphereGeode->addDrawable(sphereGeometry);

	//// Establish texture coordinates, vertex list, and normals
	//for (unsigned int r(0); r < rings; ++r) {
	//	for (unsigned int s(0); s < sectors; ++s) {
	//		float const y = sin(-M_PI_2 + M_PI * r * R);
	//		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
	//		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

	//		sphereTexCoords->push_back(osg::Vec2(s * R, r * R));

	//		sphereVertices->push_back(osg::Vec3(x * radius,
	//			y * radius,
	//			z * radius))
	//			;
	//		sphereNormals->push_back(osg::Vec3(x, y, z));

	//	}
	//}

	//sphereGeometry->setVertexArray(sphereVertices);
	//sphereGeometry->setTexCoordArray(0, sphereTexCoords);

	//// Generate quads for each face.
	//for (unsigned int r(0); r < rings - 1; ++r) {
	//	for (unsigned int s(0); s < sectors - 1; ++s) {

	//		osg::ref_ptr<osg::DrawElementsUInt> face =
	//			new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS,
	//				4)
	//			;
	//		// Corners of quads should be in CCW order.
	//		face->push_back((r + 0) * sectors + (s + 0));
	//		face->push_back((r + 0) * sectors + (s + 1));
	//		face->push_back((r + 1) * sectors + (s + 1));
	//		face->push_back((r + 1) * sectors + (s + 0));

	//		sphereGeometry->addPrimitiveSet(face);
	//	}
	//}
	auto m_material = new osg::Material;

	m_material->setAmbient(osg::Material::FRONT, osg::Vec4(0.19225, 0.19225, 0.19225, 1.0));
	m_material->setDiffuse(osg::Material::FRONT, osg::Vec4(0.50754, 0.50754, 0.50754, 1.0));
	m_material->setSpecular(osg::Material::FRONT, osg::Vec4(0.508273, 0.508273, 0.508273, 1.0));
	m_material->setEmission(osg::Material::FRONT, osg::Vec4(0.0, 0.0, 0.0, 0.0));
	m_material->setShininess(osg::Material::FRONT, 51.2);

	//sphereGeode->getOrCreateStateSet()->setAttribute(m_material);


	osg::Program *programObject = new osg::Program();
	osg::Shader *vertexObject = new osg::Shader(osg::Shader::VERTEX);
	osg::Shader *fragmentObject = new osg::Shader(osg::Shader::FRAGMENT);
	programObject->addShader(fragmentObject);
	programObject->addShader(vertexObject);

	vertexObject->loadShaderSourceFromFile("gouraud.vert");
	fragmentObject->loadShaderSourceFromFile("gouraud.frag");

	//sphereStateSet->setAttributeAndModes(programObject, osg::StateAttribute::ON);

	
	RGeomDefinitionRef d = new RGeomDefinition();
	d->getOrCreateStateSet()->setAttribute(m_material);
	//d->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0, 0, 0), radius)));
	//d->getOrCreateStateSet()->setAttributeAndModes(programObject, osg::StateAttribute::ON);
	

	DrawSphere(osg::Vec3(0, 0, 0), 1);

	return d->CreateInstance();
}

void RModelingManager::DrawSphere(osg::Vec3 pos, double radius)
{

	osg::Program *programObject = new osg::Program();
	osg::Shader *vertexObject = new osg::Shader(osg::Shader::VERTEX);
	osg::Shader *fragmentObject = new osg::Shader(osg::Shader::FRAGMENT);
	programObject->addShader(fragmentObject);
	programObject->addShader(vertexObject);

	bool status = vertexObject->loadShaderSourceFromFile("E:/programming/r3d-engine/R3D/ViewerCore/gouraud.vert");
	bool status1 = fragmentObject->loadShaderSourceFromFile("E:/programming/r3d-engine/R3D/ViewerCore/gouraud.frag");


	osg::Sphere* unitSphere = new osg::Sphere(pos, radius);
	osg::ShapeDrawable* unitSphereDrawable = new osg::ShapeDrawable(unitSphere);

	osg::Geode* basicShapesGeode = new osg::Geode();
	basicShapesGeode->addDrawable(unitSphereDrawable);

	basicShapesGeode->getOrCreateStateSet()->setAttributeAndModes(programObject, osg::StateAttribute::ON);

	m_data->GetRootNode()->addChild(basicShapesGeode);

}



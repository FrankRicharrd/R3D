#include "stdafx.h"
#if defined(_MSC_VER)
#pragma warning( disable : 4786 )
#endif

#include <RDrawingEntity.h>

#include "ReadObj.h"



#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/api/win32/GraphicsWindowWin32>
#include <osgViewer/Viewer>
#include <osg/ref_ptr>
#include <osg/MatrixTransform>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/GraphicsWindow>

#include <osgGA/StateSetManipulator>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osgViewer/CompositeViewer>
#include <osg\PositionAttitudeTransform>
#include <osg\PrimitiveSet>
#include <osg\ShapeDrawable>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/CoordinateSystemNode>
#include <osgText/Text>

#include <osgManipulator/TabBoxDragger>
#include <osgManipulator/TabBoxTrackballDragger>
#include <osgManipulator/TabPlaneDragger>
#include <osgManipulator/TabPlaneTrackballDragger>
#include <osgManipulator/Scale1DDragger>
#include <osgManipulator/Scale2DDragger>
#include <osgManipulator/TrackballDragger>
#include <osgManipulator/Translate1DDragger>
#include <osgManipulator/Translate2DDragger>
#include <osgManipulator/TranslateAxisDragger>
#include <osgManipulator/TranslatePlaneDragger>
#include <osgManipulator/RotateCylinderDragger>
#include <osgManipulator/AntiSquish>
#include <osgUtil/SmoothingVisitor>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/io_utils>
#include <osgUtil/Tessellator>
#include <osgShadow/ShadowedScene>#include <osgShadow/ShadowMap>
#include <osg/TexMat>
#define TINYOBJLOADER_IMPLEMENTATION

#include "tiny_obj_loader.h"
#include <cassert>
#include <timeapi.h>



using namespace osg;

typedef std::map< std::string, osg::ref_ptr<osg::StateSet> > MaterialToStateSetMap;


class timerutil {
public:
#ifdef _WIN32
	typedef DWORD time_t;

	timerutil() { ::timeBeginPeriod(1); }
	~timerutil() { ::timeEndPeriod(1); }

	void start() { t_[0] = ::timeGetTime(); }
	void end() { t_[1] = ::timeGetTime(); }

	time_t sec() { return (time_t)((t_[1] - t_[0]) / 1000); }
	time_t msec() { return (time_t)((t_[1] - t_[0])); }
	time_t usec() { return (time_t)((t_[1] - t_[0]) * 1000); }
	time_t current() { return ::timeGetTime(); }

#else
#if defined(__unix__) || defined(__APPLE__)
	typedef unsigned long int time_t;

	void start() { gettimeofday(tv + 0, &tz); }
	void end() { gettimeofday(tv + 1, &tz); }

	time_t sec() { return (time_t)(tv[1].tv_sec - tv[0].tv_sec); }
	time_t msec() {
		return this->sec() * 1000 +
			(time_t)((tv[1].tv_usec - tv[0].tv_usec) / 1000);
	}
	time_t usec() {
		return this->sec() * 1000000 + (time_t)(tv[1].tv_usec - tv[0].tv_usec);
	}
	time_t current() {
		struct timeval t;
		gettimeofday(&t, NULL);
		return (time_t)(t.tv_sec * 1000 + t.tv_usec);
	}

#else  // C timer
	// using namespace std;
	typedef clock_t time_t;

	void start() { t_[0] = clock(); }
	void end() { t_[1] = clock(); }

	time_t sec() { return (time_t)((t_[1] - t_[0]) / CLOCKS_PER_SEC); }
	time_t msec() { return (time_t)((t_[1] - t_[0]) * 1000 / CLOCKS_PER_SEC); }
	time_t usec() { return (time_t)((t_[1] - t_[0]) * 1000000 / CLOCKS_PER_SEC); }
	time_t current() { return (time_t)clock(); }

#endif
#endif

private:
#ifdef _WIN32
	DWORD t_[2];
#else
#if defined(__unix__) || defined(__APPLE__)
	struct timeval tv[2];
	struct timezone tz;
#else
	time_t t_[2];
#endif
#endif
};

static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]) {
	float v10[3];
	v10[0] = v1[0] - v0[0];
	v10[1] = v1[1] - v0[1];
	v10[2] = v1[2] - v0[2];

	float v20[3];
	v20[0] = v2[0] - v0[0];
	v20[1] = v2[1] - v0[1];
	v20[2] = v2[2] - v0[2];

	N[0] = v20[1] * v10[2] - v20[2] * v10[1];
	N[1] = v20[2] * v10[0] - v20[0] * v10[2];
	N[2] = v20[0] * v10[1] - v20[1] * v10[0];

	float len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
	if (len2 > 0.0f) {
		float len = sqrtf(len2);

		N[0] /= len;
		N[1] /= len;
		N[2] /= len;
	}
}


namespace  // Local utility functions
{
	struct vec3 {
		float v[3];
		vec3() {
			v[0] = 0.0f;
			v[1] = 0.0f;
			v[2] = 0.0f;
		}
	};

	void normalizeVector(vec3 &v) {
		float len2 = v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2];
		if (len2 > 0.0f) {
			float len = sqrtf(len2);

			v.v[0] /= len;
			v.v[1] /= len;
			v.v[2] /= len;
		}

	}

	// Check if `mesh_t` contains smoothing group id.
	bool hasSmoothingGroup(const tinyobj::shape_t& shape)
	{
		for (size_t i = 0; i < shape.mesh.smoothing_group_ids.size(); i++) {
			if (shape.mesh.smoothing_group_ids[i] > 0) {
				return true;
			}
		}
		return false;
	}

	void computeSmoothingNormals(const tinyobj::attrib_t& attrib, const tinyobj::shape_t& shape,
		std::map<int, vec3>& smoothVertexNormals) {
		smoothVertexNormals.clear();
		std::map<int, vec3>::iterator iter;

		for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++) {
			// Get the three indexes of the face (all faces are triangular)
			tinyobj::index_t idx0 = shape.mesh.indices[3 * f + 0];
			tinyobj::index_t idx1 = shape.mesh.indices[3 * f + 1];
			tinyobj::index_t idx2 = shape.mesh.indices[3 * f + 2];

			// Get the three vertex indexes and coordinates
			int vi[3];      // indexes
			float v[3][3];  // coordinates

			for (int k = 0; k < 3; k++) {
				vi[0] = idx0.vertex_index;
				vi[1] = idx1.vertex_index;
				vi[2] = idx2.vertex_index;
				assert(vi[0] >= 0);
				assert(vi[1] >= 0);
				assert(vi[2] >= 0);

				v[0][k] = attrib.vertices[3 * vi[0] + k];
				v[1][k] = attrib.vertices[3 * vi[1] + k];
				v[2][k] = attrib.vertices[3 * vi[2] + k];
			}

			// Compute the normal of the face
			float normal[3];
			CalcNormal(normal, v[0], v[1], v[2]);

			// Add the normal to the three vertexes
			for (size_t i = 0; i < 3; ++i) {
				iter = smoothVertexNormals.find(vi[i]);
				if (iter != smoothVertexNormals.end()) {
					// add
					iter->second.v[0] += normal[0];
					iter->second.v[1] += normal[1];
					iter->second.v[2] += normal[2];
				}
				else {
					smoothVertexNormals[vi[i]].v[0] = normal[0];
					smoothVertexNormals[vi[i]].v[1] = normal[1];
					smoothVertexNormals[vi[i]].v[2] = normal[2];
				}
			}

		}  // f

		   // Normalize the normals, that is, make them unit vectors
		for (iter = smoothVertexNormals.begin(); iter != smoothVertexNormals.end();
			iter++) {
			normalizeVector(iter->second);
		}

	}  // computeSmoothingNormals
}  // namespace

enum TextureMapType {
	DIFFUSE = 0,
	OPACITY,
	AMBIENT,
	SPECULAR,
	SPECULAR_EXPONENT,
	BUMP,
	DISPLACEMENT,
	REFLECTION,        // read of a reflection map will also apply spherical texgen coordinates
	UNKNOWN            // UNKNOWN has to be the last
};



static void load_material_texture(tinyobj::material_t &material,
	TextureMapType mapType,
	osg::StateSet *stateset,
	const unsigned int texture_unit, std::string texFileName, tinyobj::texture_option_t& textureOpt, std::string basePath)
{
	std::string filename = texFileName;

	OutputDebugStringA("Filenames");
	OutputDebugStringA(filename.c_str());
	std::string ss = basePath + '/' + filename;
	OutputDebugStringA(ss.c_str());



	if (!filename.empty())
	{
		osg::ref_ptr< osg::Image > image;
		if (!basePath.empty())
		{
			// first try with database path of parent.
			image = osgDB::readRefImageFile(basePath + '/' + filename);
		}

		if (!image.valid())
		{
			// if not already set then try the filename as is.
			image = osgDB::readRefImageFile(filename);
		}

		if (image.valid())
		{
			osg::Texture2D* texture = new osg::Texture2D(image.get());
			osg::Texture::WrapMode textureWrapMode;
			if (textureOpt.clamp == true)
			{
				textureWrapMode = osg::Texture::CLAMP_TO_BORDER;
				texture->setBorderColor(osg::Vec4(0.0, 0.0, 0.0, 0.0));    // transparent
																		   //stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
																		   //stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
			}
			else
			{
				textureWrapMode = osg::Texture::REPEAT;
			}

			texture->setWrap(osg::Texture2D::WRAP_R, textureWrapMode);
			texture->setWrap(osg::Texture2D::WRAP_S, textureWrapMode);
			texture->setWrap(osg::Texture2D::WRAP_T, textureWrapMode);
			stateset->setTextureAttributeAndModes(texture_unit, texture, osg::StateAttribute::ON);

			if (mapType == TextureMapType::REFLECTION)
			{
				osg::TexGen* texgen = new osg::TexGen;
				texgen->setMode(osg::TexGen::SPHERE_MAP);
				stateset->setTextureAttributeAndModes(texture_unit, texgen, osg::StateAttribute::ON);
			}

			if (image->isImageTranslucent())
			{
				OSG_INFO << "Found transparent image" << std::endl;
				stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
				stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
			}
		}
	}

	if (textureOpt.scale[0] != 1.0f || textureOpt.scale[1] != 1.0f ||
		textureOpt.origin_offset[0] != 0.0f || textureOpt.origin_offset[1] != 0.0f)
	{
		osg::Matrix mat;
		if (textureOpt.scale[0] != 1.0f || textureOpt.scale[1] != 1.0f)
		{
			OSG_DEBUG << "Obj TexMat scale=" << textureOpt.scale[0] << "," << textureOpt.scale[1] << std::endl;
			mat *= osg::Matrix::scale(textureOpt.scale[0], textureOpt.scale[1], textureOpt.scale[2]);
		}
		if (textureOpt.origin_offset[0] != 0.0f || textureOpt.origin_offset[1] != 0.0f)
		{
			OSG_DEBUG << "Obj TexMat offset=" << textureOpt.origin_offset[0] << "," << textureOpt.origin_offset[1] << std::endl;
			mat *= osg::Matrix::translate(textureOpt.origin_offset[0], textureOpt.origin_offset[1], textureOpt.origin_offset[2]);
		}

		osg::TexMat* texmat = new osg::TexMat;
		texmat->setMatrix(mat);
		stateset->setTextureAttributeAndModes(texture_unit, texmat, osg::StateAttribute::ON);
	}
}

void buildMaterialToStateSetMap(std::vector<tinyobj::material_t>& materials, MaterialToStateSetMap& materialToStateSetMap, std::string basePath)
{
	//if (localOptions.fixBlackMaterials)
	//{
	//	// hack to fix Maya exported models that contain all black materials.
	//	int numBlack = 0;
	//	int numNotBlack = 0;
	//	obj::Model::MaterialMap::iterator itr;
	//	for (itr = model.materialMap.begin();
	//		itr != model.materialMap.end();
	//		++itr)
	//	{
	//		obj::Material& material = itr->second;
	//		if (material.ambient == osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f) &&
	//			material.diffuse == osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f))
	//		{
	//			++numBlack;
	//		}
	//		else
	//		{
	//			++numNotBlack;
	//		}
	//	}

	//	if (numNotBlack == 0 && numBlack != 0)
	//	{
	//		for (itr = model.materialMap.begin();
	//			itr != model.materialMap.end();
	//			++itr)
	//		{
	//			obj::Material& material = itr->second;
	//			if (material.ambient == osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f) &&
	//				material.diffuse == osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f))
	//			{
	//				material.ambient.set(0.3f, 0.3f, 0.3f, 1.0f);
	//				material.diffuse.set(1.0f, 1.0f, 1.0f, 1.0f);
	//			}
	//		}
	//	}
	//}


	for (auto material : materials)
	{
		osg::ref_ptr< osg::StateSet > stateset = new osg::StateSet;

		bool isTransparent = false;

		// handle material colors
		// http://java3d.j3d.org/utilities/loaders/obj/sun.html
		if (material.illum != 0)
		{
			osg::ref_ptr<osg::Material> osg_material = new osg::Material;
			stateset->setAttribute(osg_material);
			osg_material->setName(material.name);
			osg_material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(material.ambient[0], material.ambient[1], material.ambient[2], 1));
			osg_material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(material.diffuse[0], material.diffuse[1], material.diffuse[2], 1));
			osg_material->setEmission(osg::Material::FRONT_AND_BACK, osg::Vec4(material.emission[0], material.emission[1], material.emission[2], 1));

			if (material.illum == 2) {
				osg_material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(material.specular[0], material.specular[1], material.specular[2], 1));
			}
			else {
				osg_material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0, 0, 0, 1));
			}
			osg_material->setShininess(osg::Material::FRONT_AND_BACK, (material.shininess / 1000.0f)*128.0f); // note OBJ shiniess is 0..1000.

			if (material.dissolve == 0)
			{
				OSG_INFO << "Found transparent material" << std::endl;
				isTransparent = true;
			}
		}

		// If the user has explicitly set the required texture type to unit map via the options
		// string, then we load ONLY those textures that are in the map.
		//if (localOptions.textureUnitAllocation.size()>0)
		//{
		//	for (unsigned int i = 0; i<localOptions.textureUnitAllocation.size(); i++)
		//	{
		//		// firstly, get the option set pair
		//		int unit = localOptions.textureUnitAllocation[i].first;
		//		obj::Material::Map::TextureMapType type = localOptions.textureUnitAllocation[i].second;
		//		// secondly, see if this texture type (e.g. DIFFUSE) is one of those in the material
		//		int index = -1;
		//		for (unsigned int j = 0; j<material.maps.size(); j++)
		//		{
		//			if (material.maps[j].type == type)
		//			{
		//				index = (int)j;
		//				break;
		//			}
		//		}
		//		if (index >= 0) load_material_texture(model, material.maps[index], stateset.get(), unit, options);
		//	}
		//}
		// If the user has set no options, then we load them up in the order contained in the enum. This
		// latter method is an attempt not to break user's existing code
		//else
		{
			int unit = 0;
			for (int i = 0; i < (int)TextureMapType::UNKNOWN; i++) // for each type
			{
				TextureMapType type = (TextureMapType)i;
				// see if this texture type (e.g. DIFFUSE) is one of those in the material
				int index = -1;

				if (type == TextureMapType::AMBIENT && !material.ambient_texname.empty())
				{
					load_material_texture(material, type, stateset.get(), unit, material.ambient_texname, material.ambient_texopt, basePath);
					unit++;
				}
				else if (type == TextureMapType::BUMP && !material.bump_texname.empty())
				{
					load_material_texture(material, type, stateset.get(), unit, material.bump_texname, material.bump_texopt, basePath);
					unit++;
				}
				else if (type == TextureMapType::DIFFUSE && !material.diffuse_texname.empty())
				{
					load_material_texture(material, type, stateset.get(), unit, material.diffuse_texname, material.diffuse_texopt, basePath);
					unit++;
				}
				else if (type == TextureMapType::DISPLACEMENT && !material.displacement_texname.empty())
				{
					load_material_texture(material, type, stateset.get(), unit, material.displacement_texname, material.displacement_texopt, basePath);
					unit++;
				}
				else if (type == TextureMapType::OPACITY && !material.alpha_texname.empty())
				{
					load_material_texture(material, type, stateset.get(), unit, material.alpha_texname, material.alpha_texopt, basePath);
					unit++;
				}
				else if (type == TextureMapType::REFLECTION && !material.reflection_texname.empty())
				{
					load_material_texture(material, type, stateset.get(), unit, material.reflection_texname, material.reflection_texopt, basePath);
					unit++;
				}
				else if (type == TextureMapType::SPECULAR && !material.specular_texname.empty())
				{
					load_material_texture(material, type, stateset.get(), unit, material.specular_texname, material.specular_texopt, basePath);
					unit++;
				}
				else if (type == TextureMapType::SPECULAR_EXPONENT && !material.specular_highlight_texname.empty())
				{
					load_material_texture(material, type, stateset.get(), unit, material.specular_highlight_texname, material.specular_highlight_texopt, basePath);
					unit++;
				}
			}
		}

		if (isTransparent)
		{
			stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
			stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		}

		materialToStateSetMap[material.name] = stateset.get();
	}
}



RGeomDefinitionRef  ReadObjFromFile(std::string filePath, std::string basePath) {
	/*Group *scene = new Group();
	scene->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);
*/
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	timerutil tm;

	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filePath.c_str(), basePath.c_str());
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	tm.end();

	if (!ret) {
		std::cerr << "Failed to load " << std::endl;
		return nullptr;
	}

	printf("Parsing time: %d [ms]\n", (int)tm.msec());

	printf("# of vertices  = %d\n", (int)(attrib.vertices.size()) / 3);
	printf("# of normals   = %d\n", (int)(attrib.normals.size()) / 3);
	printf("# of texcoords = %d\n", (int)(attrib.texcoords.size()) / 2);
	printf("# of materials = %d\n", (int)materials.size());
	printf("# of shapes    = %d\n", (int)shapes.size());

	// Append `default` material
	materials.push_back(tinyobj::material_t());

	for (size_t i = 0; i < materials.size(); i++) {
		printf("material[%d].diffuse_texname = %s\n", int(i),
			materials[i].diffuse_texname.c_str());
	}

	printf("Shapes Total %d", int(shapes.size()));


	MaterialToStateSetMap matStateSet;
	buildMaterialToStateSetMap(materials, matStateSet, basePath);

	
	RGeomDefinitionRef fileGeom = new RGeomDefinition();

	for (size_t s = 0; s < shapes.size(); s++) {

		RGeomEntityRef geom = new RGeomEntity();

		auto matId = shapes[s].mesh.material_ids.front();


		if ((matId < 0) ||
			(matId >= static_cast<int>(materials.size()))) {
			// Invaid material ID. Use default material.
			matId =
				materials.size() -
				1;  // Default material is added to the last item in `materials`.
		}

		std::string matName = materials[matId].name;



		std::cout << "Shape Name :" << shapes[s].name << "   <<  >>   " << "Material Name: " << matName << std::endl;


		MaterialToStateSetMap::const_iterator it = matStateSet.find(matName);
		if (it != matStateSet.end())
		{
			osg::ref_ptr<osg::StateSet> stateset = it->second.get();
			geom->setStateSet(stateset.get());
		}
		osg::ref_ptr<osg::Vec3Array> verArr = new osg::Vec3Array();
		osg::ref_ptr<osg::Vec3Array> norArr = new osg::Vec3Array();
		osg::ref_ptr<osg::Vec3Array> colArr = new osg::Vec3Array();
		osg::ref_ptr<osg::Vec2Array> texArr = new osg::Vec2Array();
		geom->setVertexArray(verArr);
		geom->setNormalArray(norArr);
		geom->setColorArray(colArr);
		geom->setTexCoordArray(0, texArr);
		geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
		geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);


		std::vector<float> buffer;
		// Check for smoothing group and compute smoothing normals
		std::map<int, vec3> smoothVertexNormals;
		if (hasSmoothingGroup(shapes[s]) > 0) {
			std::cout << "Compute smoothingNormal for shape [" << s << "]" << std::endl;
			computeSmoothingNormals(attrib, shapes[s], smoothVertexNormals);
		}

		for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++) {
			tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
			tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
			tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

			int current_material_id = shapes[s].mesh.material_ids[f];

			if ((current_material_id < 0) ||
				(current_material_id >= static_cast<int>(materials.size()))) {
				// Invaid material ID. Use default material.
				current_material_id =
					materials.size() -
					1;  // Default material is added to the last item in `materials`.
			}
			// if (current_material_id >= materials.size()) {
			//    std::cerr << "Invalid material index: " << current_material_id <<
			//    std::endl;
			//}
			//
			float diffuse[3];
			for (size_t i = 0; i < 3; i++) {
				diffuse[i] = materials[current_material_id].diffuse[i];
			}
			float tc[3][2];
			if (attrib.texcoords.size() > 0) {
				if ((idx0.texcoord_index < 0) || (idx1.texcoord_index < 0) ||
					(idx2.texcoord_index < 0)) {
					// face does not contain valid uv index.
					tc[0][0] = 0.0f;
					tc[0][1] = 0.0f;
					tc[1][0] = 0.0f;
					tc[1][1] = 0.0f;
					tc[2][0] = 0.0f;
					tc[2][1] = 0.0f;
				}
				else {
					assert(attrib.texcoords.size() >
						size_t(2 * idx0.texcoord_index + 1));
					assert(attrib.texcoords.size() >
						size_t(2 * idx1.texcoord_index + 1));
					assert(attrib.texcoords.size() >
						size_t(2 * idx2.texcoord_index + 1));

					// Flip Y coord.
					tc[0][0] = attrib.texcoords[2 * idx0.texcoord_index];
					tc[0][1] = 1.0f - attrib.texcoords[2 * idx0.texcoord_index + 1];
					tc[1][0] = attrib.texcoords[2 * idx1.texcoord_index];
					tc[1][1] = 1.0f - attrib.texcoords[2 * idx1.texcoord_index + 1];
					tc[2][0] = attrib.texcoords[2 * idx2.texcoord_index];
					tc[2][1] = 1.0f - attrib.texcoords[2 * idx2.texcoord_index + 1];
				}
			}
			else {
				tc[0][0] = 0.0f;
				tc[0][1] = 0.0f;
				tc[1][0] = 0.0f;
				tc[1][1] = 0.0f;
				tc[2][0] = 0.0f;
				tc[2][1] = 0.0f;
			}

			float v[3][3];
			for (int k = 0; k < 3; k++) {
				int f0 = idx0.vertex_index;
				int f1 = idx1.vertex_index;
				int f2 = idx2.vertex_index;
				assert(f0 >= 0);
				assert(f1 >= 0);
				assert(f2 >= 0);

				v[0][k] = attrib.vertices[3 * f0 + k];
				v[1][k] = attrib.vertices[3 * f1 + k];
				v[2][k] = attrib.vertices[3 * f2 + k];
				/*	bmin[k] = std::min(v[0][k], bmin[k]);
				bmin[k] = std::min(v[1][k], bmin[k]);
				bmin[k] = std::min(v[2][k], bmin[k]);
				bmax[k] = std::max(v[0][k], bmax[k]);
				bmax[k] = std::max(v[1][k], bmax[k]);
				bmax[k] = std::max(v[2][k], bmax[k]);*/
			}

			float n[3][3];
			{
				bool invalid_normal_index = false;
				if (attrib.normals.size() > 0) {
					int nf0 = idx0.normal_index;
					int nf1 = idx1.normal_index;
					int nf2 = idx2.normal_index;

					if ((nf0 < 0) || (nf1 < 0) || (nf2 < 0)) {
						// normal index is missing from this face.
						invalid_normal_index = true;
					}
					else {
						for (int k = 0; k < 3; k++) {
							assert(size_t(3 * nf0 + k) < attrib.normals.size());
							assert(size_t(3 * nf1 + k) < attrib.normals.size());
							assert(size_t(3 * nf2 + k) < attrib.normals.size());
							n[0][k] = attrib.normals[3 * nf0 + k];
							n[1][k] = attrib.normals[3 * nf1 + k];
							n[2][k] = attrib.normals[3 * nf2 + k];
						}
					}
				}
				else {
					invalid_normal_index = true;
				}

				if (invalid_normal_index && !smoothVertexNormals.empty()) {
					// Use smoothing normals
					int f0 = idx0.vertex_index;
					int f1 = idx1.vertex_index;
					int f2 = idx2.vertex_index;

					if (f0 >= 0 && f1 >= 0 && f2 >= 0) {
						n[0][0] = smoothVertexNormals[f0].v[0];
						n[0][1] = smoothVertexNormals[f0].v[1];
						n[0][2] = smoothVertexNormals[f0].v[2];

						n[1][0] = smoothVertexNormals[f1].v[0];
						n[1][1] = smoothVertexNormals[f1].v[1];
						n[1][2] = smoothVertexNormals[f1].v[2];

						n[2][0] = smoothVertexNormals[f2].v[0];
						n[2][1] = smoothVertexNormals[f2].v[1];
						n[2][2] = smoothVertexNormals[f2].v[2];

						invalid_normal_index = false;
					}
				}

				if (invalid_normal_index) {
					// compute geometric normal
					CalcNormal(n[0], v[0], v[1], v[2]);
					n[1][0] = n[0][0];
					n[1][1] = n[0][1];
					n[1][2] = n[0][2];
					n[2][0] = n[0][0];
					n[2][1] = n[0][1];
					n[2][2] = n[0][2];
				}
			}


			for (int k = 0; k < 3; k++) {


				// Combine normal and diffuse to get color.
				float normal_factor = 0.2;
				float diffuse_factor = 1 - normal_factor;
				float c[3] = { n[k][0] * normal_factor + diffuse[0] * diffuse_factor,
					n[k][1] * normal_factor + diffuse[1] * diffuse_factor,
					n[k][2] * normal_factor + diffuse[2] * diffuse_factor };
				float len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
				if (len2 > 0.0f) {
					float len = sqrtf(len2);

					c[0] /= len;
					c[1] /= len;
					c[2] /= len;
				}

				/*
				buffer.push_back(c[0] * 0.5 + 0.5);
				buffer.push_back(c[1] * 0.5 + 0.5);
				buffer.push_back(c[2] * 0.5 + 0.5);
				*/

				verArr->push_back(osg::Vec3(v[k][0], v[k][1], v[k][2]));
				norArr->push_back(osg::Vec3(n[k][0], n[k][1], n[k][2]));
				colArr->push_back(osg::Vec3(c[0] * 0.5 + 0.5, c[1] * 0.5 + 0.5, c[2] * 0.5 + 0.5));
				texArr->push_back(osg::Vec2(tc[k][0], tc[k][1]));

				/*buffer.push_back(tc[k][0]);
				buffer.push_back(tc[k][1]);*/
			}
		}

		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, verArr->size()));

		//geom->getOrCreateStateSet()->setter

		fileGeom->addDrawable(geom);

	}

	return fileGeom;
}


ReadObj::ReadObj(std::string filePath, std::string basePath): m_filePath(filePath), m_basePath(basePath)
{

}


ReadObj::~ReadObj()
{
}

RGeomDefinitionRef ReadObj::ReadFile()
{
	RGeomDefinitionRef nn =  ReadObjFromFile(m_filePath, m_basePath);
	return nn;
}


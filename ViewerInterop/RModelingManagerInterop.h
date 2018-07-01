#pragma once
namespace ViewerInterop {
	public ref class RModelingManagerInterop : public IRModelingManager
	{
	protected:
		RModelingManager*  m_modelingManager;


	public:
		RModelingManagerInterop(RModelingManager*  m_modelingManager);

		// Inherited via IModelingManager
		virtual Viewer::Interfaces::IRTransformNode ^ CreateSphere(double radius, int rings, int sector);



		// Inherited via IRModelingManager
		virtual void CreateCustomData(System::String ^fileName, System::Object ^v);

		// Inherited via IRModelingManager
		virtual System::String ^ GetNumberOfDrawables();
		int GetDrawables(osg::ref_ptr<osg::Group> group);

		void CreateCustomDataPerDef(RExternalData::RevitData ^ rdata);
		RMeshRef createmeshPerDef(RExternalData::RMesh ^ mesh, RMeshRef defMesh, String^ materialID, RExternalData::RevitData ^ rdata, osg::ref_ptr<osg::Vec3Array> m_drawVertexCol, osg::ref_ptr<osg::Vec3Array> m_drawNormalArray, osg::ref_ptr<osg::Vec4Array> colorArray);

	private :
		void createcustomData(System::Object ^v);
		RMeshRef createmesh(RExternalData::RMesh ^ mesh, RMeshRef defMesh, String^ materialID, RExternalData::RevitData ^ rdata, osg::ref_ptr<osg::Vec3Array> vbo, osg::ref_ptr<osg::Vec3Array> m_drawNormalArray);

		osg::ref_ptr<osg::Material> CreateOsgMaterial(RExternalData::RMaterial^ material);
		RExternalData::RMaterial^ GetMaterial(RExternalData::RevitData^ rdata, String^ id);

		bool CalculateLines = true;
		bool ShareVbos = true;
		bool CreateOneMeshPerDefinition = true;

	};

}


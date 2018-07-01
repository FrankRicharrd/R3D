#pragma once

namespace ViewerInterop {
	public ref class RMaterialManagerInterop :
		public IRMaterialManager
	{

	protected:
		RMaterialManager* m_materialManager;

	public:
		RMaterialManagerInterop(RMaterialManager* materialManager );

		// Inherited via IRMaterialManager
		virtual bool SetShaderToNode(System::String ^shaderprogram, Viewer::ShaderType shaderType, Viewer::Interfaces::IRNode ^node);
	};

}
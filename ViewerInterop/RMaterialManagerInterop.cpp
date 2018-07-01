#include "stdafx.h"
#include "RMaterialManager.h"
#include "RMaterialManagerInterop.h"
#include "InteropUtilities.h"
#include "WrapperObjects.h"
#include "RGeomInstanceInterop.h"
namespace ViewerInterop {
	RMaterialManagerInterop::RMaterialManagerInterop(RMaterialManager* materialManager)
	{
		m_materialManager = materialManager;
	}
	bool RMaterialManagerInterop::SetShaderToNode(System::String ^shaderprogram, Viewer::ShaderType shaderType, Viewer::Interfaces::IRNode ^node)
	{
		std::string shaderprm;

		InteropUtilities::MarshalString(shaderprogram, shaderprm);

		if (node == nullptr)
		{
			return m_materialManager->SetShaderToNode(shaderprm, (osg::Shader::Type)shaderType, NULL);
		}
		else
		{
			/*if (node->GetWrappedNativeObject() != nullptr) {
				CRNodeWrapper^ cnodewrapper = (CRNodeWrapper^)node->GetWrappedNativeObject();
				CRNode* cnode = cnodewrapper->GetNativeObject();
				return m_materialManager->SetShaderToNode(shaderprm, (osg::Shader::Type)shaderType, cnode->GetOsgNode());
			}
			else*/
			{
				auto n = (RGeomInstanceInterop^)node;
				if (n != nullptr)
					return m_materialManager->SetShaderToNode(shaderprm, (osg::Shader::Type)shaderType, n->GetnativeObj());
			}
		}

		return false;
	}
}
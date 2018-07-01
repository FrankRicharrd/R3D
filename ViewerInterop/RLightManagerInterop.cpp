#include "stdafx.h"
#include "RLightsManager.h"
#include "RLightManagerInterop.h"
#include "WrapperObjects.h"
#include "RLightInterop.h"
namespace ViewerInterop {
	RLightManagerInterop::RLightManagerInterop(RLightsManager* manager) : m_manager(manager)
	{
	}
	void RLightManagerInterop::SwitchMainLight(bool enable)
	{
		m_manager->SwitchMainLight(enable);
	}
	void RLightManagerInterop::HeadLightSwitch(bool v)
	{
		m_manager->HeadLightSwitch(v);
	}
	void RLightManagerInterop::SkyLightSwitch(bool v)
	{
		m_manager->SkyLightSwitch(v);
	}

	Viewer::Interfaces::IRLight ^ RLightManagerInterop::CreateLight(Viewer::Interfaces::IRTransformNode ^transNode, System::Windows::Media::Color color, double posX, double posY, double posZ)
	{
		RLight* light;

		if (transNode == nullptr)
		{
			light = m_manager->Createlight(nullptr, osg::Vec4(color.R, color.G, color.B, color.A), osg::Vec3(posX, posY, posZ));
		}
		else
		{
			CRNodeWrapper^ node = (CRNodeWrapper^)transNode->GetWrappedNativeObject();

			CRNode* tnode = node->GetNativeObject();

			light = m_manager->Createlight(tnode->GetOsgNode()->asGroup(), osg::Vec4(color.R, color.G, color.B, color.A), osg::Vec3(posX, posY, posZ));
		}

		if (!light)
			return nullptr;
		else
		{
			RLightInterop^ lightint = gcnew RLightInterop(light);
			return lightint;
		}

	}

	void RLightManagerInterop::ExtraLights(bool v)
	{
		m_manager->ExtraLightsSwitch(v);
	}

}
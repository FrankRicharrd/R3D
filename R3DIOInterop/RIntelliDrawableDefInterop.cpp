#include "stdafx.h"
#include "RIntelliDrawableDefInterop.h"

namespace R3DIOInterop {
	RIntelliDrawableDefInterop::RIntelliDrawableDefInterop(RIntelliDrawableDef* definition, RIntelligentObject* intelliObject)
	{
		m_definition = definition;
		m_intelliObject = intelliObject;
	}
	RIntelliDrawableInstanceInterop ^ RIntelliDrawableDefInterop::CreateInstance()
	{
		// create an instance and add it to intelligentobject
		osg::ref_ptr<RIntelliDrawableInstance> instance = new RIntelliDrawableInstance(m_definition);
		m_definition->AddInstance(instance);

		m_intelliObject->AddInstance(instance.get());

		return gcnew RIntelliDrawableInstanceInterop(instance);

	}

	RIntelliDrawableInstanceInterop ^ RIntelliDrawableDefInterop::CreateDynamicInstance()
	{
		// create an instance and add it to intelligentobject
		osg::ref_ptr<RIntelliDrawableInstance> instance = new RIntelliDrawableInstance(m_definition);
		m_definition->AddInstance(instance);

		m_intelliObject->AddTempInstance(instance.get());

		return gcnew RIntelliDrawableInstanceInterop(instance);
	}

	int RIntelliDrawableDefInterop::CreateFace(List<int>^ vertexIndex)
	{
		std::vector<int> lstIndex;


		for each (int var in vertexIndex)
		{
			lstIndex.push_back(var);
		}

		auto face  = m_definition->CreateFace(lstIndex);

		return face->GetIndex();
	}
	bool RIntelliDrawableDefInterop::Redraw(bool Initdraw)
	{
		return m_definition->Redraw(Initdraw);
	}
	void RIntelliDrawableDefInterop::AddFaceDragger(int Faceindex)
	{
		m_definition->AddFaceDragger(Faceindex);
	}
	void RIntelliDrawableDefInterop::SetDraggerVisibility(bool enable)
	{
		m_definition->SetDraggerVisibility(enable);
	}
	RVertex3DInterop ^ RIntelliDrawableDefInterop::GetFaceNormal(int faceindex)
	{
		auto face = m_definition->Getfaces()[faceindex];

		if (face)
		{
			osg::Vec3 n = face->GetFaceNormal();
			gcnew RVertex3DInterop(n.x(), n.y(), n.z());
		}
		return nullptr;

	}
}

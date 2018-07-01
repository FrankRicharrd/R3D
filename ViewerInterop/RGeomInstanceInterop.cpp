#include "stdafx.h"
#include "RGeomInstanceInterop.h"
#include "InteropUtilities.h"
#include <RDrawingEntity.h>

namespace ViewerInterop
{
	
	RGeomInstanceInterop::RGeomInstanceInterop(RGeomInstance* nativeObj) : m_object(nativeObj)
	{
		NodeType = Viewer::RNodeType::Transform;
		auto ss = m_object->GetDefinitionName().c_str();

		Name = gcnew System::String(ss);
		if (Name == "")
			Name = "tempName";

	}

	System::String ^ RGeomInstanceInterop::GetName()
	{
		auto ss = m_object->GetDefinitionName();

		auto str = ss.c_str();

		auto n =  gcnew System::String(str);
		if (n == "")
			n = "tempName";
		return n;
	}

	void RGeomInstanceInterop::SetName(System::String ^name)
	{
	}

	void RGeomInstanceInterop::SetPolygonMode(int mode)
	{
		throw gcnew System::NotImplementedException();
	}

	System::Object ^ RGeomInstanceInterop::GetWrappedNativeObject()
	{
		return  nullptr;
	}

	void RGeomInstanceInterop::AddChild(Viewer::Interfaces::IRNode ^childNode)
	{
		
	}

	Viewer::Interfaces::Geometry::IVec3 ^ RGeomInstanceInterop::GetTranslation()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}

	void RGeomInstanceInterop::SetTranslation(Viewer::Interfaces::Geometry::IVec3 ^vec)
	{
		throw gcnew System::NotImplementedException();
	}

	void RGeomInstanceInterop::RemoveChild(Viewer::Interfaces::IRNode ^node)
	{
		throw gcnew System::NotImplementedException();
	}

	System::Collections::Generic::List<Viewer::Interfaces::IRNode^>^ RGeomInstanceInterop::GetChildren()
	{
		System::Collections::Generic::List<Viewer::Interfaces::IRNode^>^ childrenCol = gcnew System::Collections::Generic::List<Viewer::Interfaces::IRNode^>  ();

		int num =  m_object->getNumChildren();

		for (size_t i = 0; i < num; i++)
		{
			auto c = m_object->getChild(i);

			RGeomInstance*  cc = dynamic_cast<RGeomInstance*>(c);

			if (cc == NULL)
				continue;

			RGeomInstanceInterop^ ci = gcnew  RGeomInstanceInterop(cc);

			childrenCol->Add(ci);
		}

		//todo get definiton and their child

		int numc = m_object->GetDefinition()->getNumChildren();


		for (size_t i = 0; i < numc; i++)
		{
			auto c = m_object->GetDefinition()->getChild(i);

			RGeomInstance*  cc = dynamic_cast<RGeomInstance*>(c);

			if (cc == NULL)
				continue;

			RGeomInstanceInterop^ ci = gcnew  RGeomInstanceInterop(cc);

			childrenCol->Add(ci);
		}
		return childrenCol;
	}

}
// This is the main DLL file.

#include "stdafx.h"

#include "InteropUtilities.h"
#include "ViewerInterop.h"
#include "ImpRNode.h"
#include "ImpRGeometryNode.h"
#include "ImpRTransformNode.h"
#include "ObjectFactory.h"
#include "RModelingManagerInterop.h"
#include "RMaterialManagerInterop.h"
#include "RLightManagerInterop.h"
#include "RGeomInstanceInterop.h"


Viewer::Interfaces::IRNode ^ ViewerInterop::ViewerEditor::ImportGeometry(System::String ^filePath)
{
	if (System::IO::Path::GetExtension(filePath) == ".obj")
	{
		auto basePath = System::IO::Path::GetDirectoryName(filePath);
		basePath = basePath + "\\";

		string basepathstr;
		InteropUtilities::MarshalString(basePath, basepathstr);

		string filenamestr;
		InteropUtilities::MarshalString(filePath, filenamestr);

		auto inst = manager->Getdata()->GetFileSystemManager()->ImportObj(filenamestr, basepathstr);

		if (inst.valid())
		{
			RGeomInstanceInterop^ instInt = gcnew RGeomInstanceInterop(inst);
			return instInt;
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		String^ Filename = System::IO::Path::GetFileNameWithoutExtension(filePath);

		string filenamestr;
		InteropUtilities::MarshalString(Filename, filenamestr);


		string path;
		InteropUtilities::MarshalString(filePath, path);

		bool status;
		CRTransformNode* node = manager->Getdata()->GetFileSystemManager()->ImportMeshGeometry(path, &status);

		node->SetName(filenamestr);

		if (!status)
			return nullptr;
		else
		{

			ImpRTransformNode^ transNode = gcnew ImpRTransformNode(node);

			return transNode;
		}
	}



}


Viewer::Interfaces::IRTransformNode ^ ViewerInterop::ViewerEditor::CreateTransformNode(System::String ^name)
{
	string nameStr;
	InteropUtilities::MarshalString(name, nameStr);

	CRTransformNode* transNode = manager->CreateTransformNode(nameStr);

	ImpRTransformNode^ transnode = gcnew ImpRTransformNode(transNode);

	return transnode;
}

void ViewerInterop::ViewerEditor::SetFocus(Viewer::Interfaces::IRNode ^node)
{
	if (node == nullptr)
	{
		manager->GetViewController()->SetHomeFocus();
	}
	else
	{
		CRNodeWrapper^ wrapper = (CRNodeWrapper^)node->GetWrappedNativeObject();

		manager->GetViewController()->SetFocus(wrapper->GetNativeObject());
	}
}

void ViewerInterop::ViewerEditor::DeleteNode(Viewer::Interfaces::IRNode ^node)
{
	if (node == nullptr)
	{
		//manager->Getdata()->GetSelectedNode();
	}
	else
	{

		if (node->Parent == nullptr)
			return;

		IRTransformNode^ parent = (IRTransformNode^)node->Parent;
		parent->RemoveChild(node);
	}
}

Viewer::Interfaces::IRModelingManager ^ ViewerInterop::ViewerEditor::GetModelingManager()
{
	RModelingManagerInterop^ modeler = gcnew RModelingManagerInterop(manager->Getdata()->GetModelingManager());

	return modeler;
}

Viewer::Interfaces::IRMaterialManager ^ ViewerInterop::ViewerEditor::GetMaterialManager()
{
	RMaterialManagerInterop^ modeler = gcnew RMaterialManagerInterop(manager->Getdata()->GetMaterialManager());

	return modeler;
}

void ViewerInterop::ViewerEditor::SetSelectionType(Viewer::SelectionType type)
{
	manager->Getdata()->GetSelectionManager()->SetSelectionType((SelectionManager::SelectionType)(int)type);
}

Viewer::Interfaces::IRLightManager ^ ViewerInterop::ViewerEditor::GetLightsManager()
{
	RLightManagerInterop^ lights = gcnew RLightManagerInterop(manager->Getdata()->GetLightsManager());

	return lights;
}

void ViewerInterop::ViewerEditor::SubscribeEventForTransformNodeSelected(OnTransformNodeSelected ^callback)
{
	if (m_selectionManager == nullptr)
	{
		return;
	}

	m_selectionManager->SubscribeTransformNodeSelected(callback);
}

void ViewerInterop::ViewerEditor::KeyPress(int key)
{
	manager->getEventQueue()->keyPress(key);
}

void ViewerInterop::ViewerEditor::KeyRelease(int key)
{
	manager->getEventQueue()->keyRelease(key);
}


void ViewerInterop::ViewerEditor::SetDraggerState(Viewer::DraggerType dragger, bool isChecked)
{
	manager->SetDraggerState((ViewerCore::DraggerType)(int)dragger, isChecked);
}

void ViewerInterop::ViewerEditor::SetGridVisibility(bool ischecked)
{
	manager->Getdata()->GetGrid()->setNodeMask(ischecked);
}


Viewer::Interfaces::IRTransformNode ^ ViewerInterop::ViewerEditor::CreateCanvasObject(IntPtr hwndHost, int width, int height)
{
	HWND* hwndptr = (HWND*)(void*)hwndHost;

	void* voidhwnd = hwndHost.ToPointer();

	manager->CreateCanvas(voidhwnd, width, height);

	m_selectionManager = gcnew RSelectionManagerInterop(manager->Getdata()->GetSelectionManager());


	/*CRTransformNode* rootnode = manager->_data->GetRootParentNode();

	rootnode->SetName("ParentNode");

	IRTransformNode^ rootTransNode = gcnew ImpRTransformNode(rootnode);
*/

	IRTransformNode^ rootTransNode = gcnew RGeomInstanceInterop(manager->Getdata()->GetGeomRootNode());

	return rootTransNode;

	//data->Tree->MainParentNode = rootTransNode;
}
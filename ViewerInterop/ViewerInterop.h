// ViewerInterop.h

#pragma once



using namespace System;

using namespace ViewerGraphicsEditor;

using namespace Viewer::Interfaces;
//template <bool> class static_assert;
//
//template <> class static_assert<true> {};
//
//template <typename T>
//
//T intptr_cast(System::IntPtr intPtr) {
//
//	static_assert< sizeof(T) == sizeof(void *) >();
//
//	return reinterpret_cast<T>(intPtr.ToPointer());
//
//}

#include "RSelectionManagerInterop.h"

namespace ViewerInterop {

	public ref class ViewerEditor :
		public IEditorInterop
	{

		ViewerManager* manager;

		RSelectionManagerInterop^ m_selectionManager;



	public:
		ViewerEditor()
		{
			manager = new ViewerManager();

			//data = gcnew ViewerCommon::AppData(this);
		}

	

		virtual Viewer::Interfaces::IRTransformNode ^ CreateCanvasObject(IntPtr hwndHost, int width, int height);
	

		virtual IntPtr __clrcall  GetCanvasObject()
		{
			return *(IntPtr*)(void*)manager;
		}

		virtual void __clrcall ResizeCanvasObject(int newWidth, int newHeight)
		{
			
		}

		virtual String^ __clrcall TestMessage()
		{
			return "Test Success";

		}

		virtual void __clrcall DestroyCanvas()
		{
			
			manager->DestroyCanvas();
		}

		virtual void __clrcall MouseMoveEvent(Viewer::MouseButton btnType, double x, double y)
		{
			manager->mouseMoveEvent((ViewerCore::MouseButton)(int)btnType, x, y);
		}

		virtual void __clrcall MousePressEvent(Viewer::MouseButton btnType, double x, double y)
		{
			manager->mousePressEvent((ViewerCore::MouseButton)(int)btnType, x, y);
		}

		virtual void __clrcall MouseReleaseEvent(Viewer::MouseButton btnType, double x, double y)
		{
			manager->mouseReleaseEvent((ViewerCore::MouseButton)(int)btnType, x, y);
		}

		virtual void __clrcall MouseWheelEvent(int delta, double x, double y)
		{
			manager->mouseWheelEvent(delta, x, y);
		}

		virtual void __clrcall SetView(Viewer::FViewType view)
		{
			return manager->GetViewController()->ToggleView((ViewerCore::FViewType)(int)view);
		}


		// Inherited via IEditorInterop
		//virtual property ViewerCommon::AppData ^ data;


	

		// Inherited via IEditorInterop
		virtual void SetDraggerState(Viewer::DraggerType dragger, bool isChecked);


		// Inherited via IEditorInterop
		virtual void SetGridVisibility(bool ischecked);


		// Inherited via IEditorInterop
		virtual property Viewer::Interfaces::IApplicationData ^ data;


		// Inherited via IEditorInterop
		virtual Viewer::Interfaces::IRNode ^ ImportGeometry(System::String ^filePath);

		virtual Viewer::Interfaces::IRTransformNode ^ CreateTransformNode(System::String ^name);


		


		// Inherited via IEditorInterop
		virtual void SetFocus(Viewer::Interfaces::IRNode ^node);

		virtual void DeleteNode(Viewer::Interfaces::IRNode ^node);


		// Inherited via IEditorInterop
		virtual Viewer::Interfaces::IRModelingManager ^ GetModelingManager();


		// Inherited via IEditorInterop
		virtual Viewer::Interfaces::IRMaterialManager ^ GetMaterialManager();


		// Inherited via IEditorInterop
		virtual void SetSelectionType(Viewer::SelectionType type);


		// Inherited via IEditorInterop
		virtual Viewer::Interfaces::IRLightManager ^ GetLightsManager();


	


		// Inherited via IEditorInterop
		 void SubscribeEventForTransformNodeSelected(OnTransformNodeSelected ^callback);


		 // Inherited via IEditorInterop
		 virtual void KeyPress(int key);

		 virtual void KeyRelease(int key);

};
}

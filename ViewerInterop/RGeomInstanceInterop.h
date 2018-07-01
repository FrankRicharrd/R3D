#pragma once
#include <RDrawingEntity.h>

namespace ViewerInterop {
	public ref class RGeomInstanceInterop : Viewer::Interfaces::IRTransformNode
	{
	public:
		RGeomInstanceInterop(RGeomInstance* nativeObj);

		// Inherited via IRTransformNode
		virtual property Viewer::RNodeType NodeType;
		virtual property System::String ^ Name {
			System::String ^ get() { return GetName(); }
			void set(System::String ^ oo) { SetName(oo); }
		}
		virtual property Viewer::Interfaces::IRNode ^ Parent;
		virtual System::String ^ GetName();
		virtual void SetName(System::String ^name);
		virtual void SetPolygonMode(int mode);
		virtual System::Object ^ GetWrappedNativeObject();
		virtual property System::Collections::Generic::List<Viewer::Interfaces::IRNode ^> ^ Children {
			System::Collections::Generic::List<Viewer::Interfaces::IRNode ^> ^ get() { return GetChildren(); }
			void set(System::Collections::Generic::List<Viewer::Interfaces::IRNode ^> ^ obj) { Children = obj; }
		}
		
		virtual void AddChild(Viewer::Interfaces::IRNode ^childNode);
		
		virtual Viewer::Interfaces::Geometry::IVec3 ^ GetTranslation();
		
		virtual void SetTranslation(Viewer::Interfaces::Geometry::IVec3 ^vec);
		
		virtual void RemoveChild(Viewer::Interfaces::IRNode ^node);

		RGeomInstance* GetnativeObj() { return m_object; }

	private :
		RGeomInstance* m_object;

		System::Collections::Generic::List<Viewer::Interfaces::IRNode ^>^ GetChildren();


	};
}


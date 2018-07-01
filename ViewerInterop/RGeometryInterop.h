#pragma once
namespace ViewerInterop {
	public	ref class RGeometryInterop :Viewer::Interfaces::IRGeometryNode
	{
	public:
		RGeometryInterop();

		// Inherited via IRGeometryNode
		virtual property Viewer::RNodeType NodeType;

		virtual property System::String ^ Name;

		virtual property Viewer::Interfaces::IRNode ^ Parent;

		virtual System::String ^ GetName();

		virtual void SetName(System::String ^name);

		virtual void SetPolygonMode(int mode);

		virtual System::Object ^ GetWrappedNativeObject();




	};
}

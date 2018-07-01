#pragma once
namespace ViewerInterop {
	public ref class CRNodeWrapper
	{
	public:
		CRNodeWrapper(CRNode* Node);

		CRNode* GetNativeObject();

	private:
		CRNode* m_nativeObj;

	};

	public ref class CRTransformNodeWrapper
	{
	public:
		CRTransformNodeWrapper(CRTransformNode* Node);

		CRTransformNode* GetNativeObject();

	private:
		CRTransformNode* m_nativeObj;

	};

	public ref class CRGeometryNodeWrapper
	{
	public:
		CRGeometryNodeWrapper(CRGeometryNode* Node);

		CRGeometryNode* GetNativeObject();

	private:
		CRGeometryNode* m_nativeObj;

	};
}

#pragma once

namespace samplenamespace
{
	public ref class SampleClass
	{
	public:
		SampleClass(void* data)
		{
			m_data = data;
		}


		void* m_data;
	};

}


namespace ViewerInterop
{

	public ref class ObjectFactory
	{
	public:
		ObjectFactory();


		static IRGeometryNode^ CreateGeometryNode(CRGeometryNode* node);
		static IRTransformNode^ CreateTransformNode(CRTransformNode* node);
		static IRTransformNode^ CreateTransformNode(CRGeometryNode* node, CRTransformNode* parentNode);
		static IRTransformNode^ CreateTransformNode(void* crgeomnode, void* crtransformParentnode);
		static IRGeometryNode^ CreateGeometryNode(void* node);
		static IRTransformNode^ CreateTransformNode(void* node);

	};

}

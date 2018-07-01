#include "stdafx.h"
#include "WrapperObjects.h"

namespace ViewerInterop {
	CRNodeWrapper::CRNodeWrapper(CRNode* Node)
	{
		m_nativeObj = Node;
	}
	CRNode * CRNodeWrapper::GetNativeObject()
	{
		return m_nativeObj;
	}
	CRTransformNodeWrapper::CRTransformNodeWrapper(CRTransformNode * Node)
	{
		m_nativeObj = Node;
	}
	CRTransformNode * CRTransformNodeWrapper::GetNativeObject()
	{
		return m_nativeObj;
	}
	CRGeometryNodeWrapper::CRGeometryNodeWrapper(CRGeometryNode * Node)
	{
		m_nativeObj = Node;
	}
	CRGeometryNode * CRGeometryNodeWrapper::GetNativeObject()
	{
		return m_nativeObj;
	}
}
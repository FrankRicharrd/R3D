#pragma once


#include "RNode.h"
#include <RDrawingEntity.h>
class FGlobalData;
class VIEWER_API FileSystemsManager
{
public:
	FileSystemsManager(FGlobalData* data);
	~FileSystemsManager();

	CRGeometryNode* ImportGeometry(string filePath, bool* status);
	CRTransformNode* ImportMeshGeometry(string filePath, bool* status);
	RGeomInstanceRef ImportObj(string filePath, string basePath);

private:
	FGlobalData* m_data;
};


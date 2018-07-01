#pragma once
class VIEWER_API RModelingManager
{
public:
	RModelingManager(FGlobalData* data);
	~RModelingManager();

	RGeomInstanceRef CreateSphere(double radius, const unsigned int rings,const unsigned int sectors);

	FGlobalData* GetData() { return m_data; }

	void DrawSphere(osg::Vec3 pos, double radius);

private:
	FGlobalData* m_data;
};


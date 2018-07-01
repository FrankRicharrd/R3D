#include "stdafx.h"
#include "EnumManager.h"
#include "RDrawingEntity.h"
#include "RIODrawingEntities.h"


RIODrawingEntities::RIODrawingEntities()
{
}


RIODrawingEntities::~RIODrawingEntities()
{
}


RIOFace::RIOFace()
{
}

inline RIOFace::RIOFace(const RIOFace & p, const osg::CopyOp & copyop) : RFace(p, copyop)
{
}

inline bool RIOFace::TranslateFace(osg::Vec3 trans)
{
	double xm = trans.x();
	double ym = trans.y();
	double zm = trans.z();

	std::string str = "Matrix value: " + std::to_string(xm) + std::string(",") + std::to_string(ym) + std::string(",") + std::to_string(zm) + std::string(",") + std::string("\n");
	OutputDebugStringA(str.c_str());



	for (auto v : m_vertices)
	{
		int i = v->GetIndex();

		osg::Vec3 ov = v->GetOsgVec();


		double xa = v->x();
		double ya = v->y();
		double za = v->z();


		std::string str1 = "original value: " + std::to_string(xa) + std::string(",") + std::to_string(ya) + std::string(",") + std::to_string(za)  + std::string("\n");
		OutputDebugStringA(str1.c_str());

		osg::Vec3 outvec = ov + trans;
	

		v->setx(outvec.x());
		v->sety(outvec.y());
		v->setz(outvec.z());

		double x = v->x();
		double y = v->y();
		double z = v->z();

		std::string str2 = "calculated value: " + std::to_string(x) + std::string(",") + std::to_string(y) + std::string(",") + std::to_string(z)+ std::string("\n");
		OutputDebugStringA(str2.c_str());

	}
	 m_definition->Redraw();

	 return true;
}

RIOFace::~RIOFace()
{
}
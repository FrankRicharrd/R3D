#pragma once

class RIOFace;

typedef osg::ref_ptr<RIOFace> RIOFaceRef;



class R3DEXPORT RIODrawingEntities
{
public:
	RIODrawingEntities();
	~RIODrawingEntities();
};




class R3DEXPORT RIOFace : public RFace
{
public:
	RIOFace();
	RIOFace(const RIOFace& p, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);

	META_Object(R3DViewer, RIOFace);

	virtual bool TranslateFace(osg::Vec3 trans);

private:

protected:
	~RIOFace();


};


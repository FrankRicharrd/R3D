#include "stdafx.h"
#include "Utilities.h"
#include "ExtraGeometries.h"
//#include <RIntelligentObject.h>
//
//class RIntelligentObject;
#include "ViewerValidator.h"

Utilities::Utilities()
{
}

//handle instantiation in the below method
osg::MatrixTransform * Utilities::GetNearTransformParent(osg::Node * node)
{

	osg::Group* parent = node->getParent(0);
	if (parent)
	{
		osg::Transform* mat = parent->asTransform();
		if (mat)
		{
			osg::MatrixTransform* matTra = mat->asMatrixTransform();

			if (matTra)
				return matTra;
		}
		return GetNearTransformParent(parent);
	}
	return NULL;
}

osg::Group * Utilities::GetNodeParentIfIObject(osg::Node * node)
{
	if (node->getNumParents() > 0)
	{
		osg::Group* parent = node->getParent(0);
		if (parent)
		{
			if (ViewerValidator::IsIoObject(parent))
			{
				return parent;
			}
			else
			{
				return GetNodeParentIfIObject(parent);
			}

		}
	}
	return NULL;
}


Utilities::~Utilities()
{
}

osg::ref_ptr<osg::MatrixTransform> Utilities::CreateGrid()
{
	
	osg::ref_ptr<osg::MatrixTransform> mat = new osg::MatrixTransform();
	osg::ref_ptr<osg::Geode> geo = new osg::Geode();


	osg::ref_ptr<GridGeom> gr = new GridGeom();
	gr->CreateGrid();

	osg::ref_ptr<CenterCoordinateAxis> ax = new CenterCoordinateAxis();
	ax->createAxis();
	/*osg::StateSet* ss = ax->getOrCreateStateSet();
	ss->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);*/

	geo->addDrawable(ax.get());

	geo->addDrawable(gr.get());
	mat->addChild(geo.get());


	return mat.get();
}



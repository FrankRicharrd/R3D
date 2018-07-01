#pragma once
#include "stdafx.h"
#include <osg/NodeVisitor>
#include <osg/BoundingBox>
#include <osg/BoundingSphere>
#include <osg/MatrixTransform>
#include <osg/Billboard>


class  CalculateBoundingBox : public osg::NodeVisitor {

public:

	CalculateBoundingBox() : NodeVisitor(NodeVisitor::TRAVERSE_ALL_CHILDREN) {
		// ----------------------------------------------------------------------
		//
		//     Default Public Class Constructor
		//
		// ----------------------------------------------------------------------

		m_transformMatrix.makeIdentity();

	}



	virtual ~CalculateBoundingBox() {}


	virtual
		void apply(osg::Geode &geode) {
		// -------------------------------------------
		//
		//   Handle nodes of the type osg::Geode
		//
		// -------------------------------------------

		osg::BoundingBox bbox;

		//
		// update bounding box for each drawable
		//
		for (unsigned int i = 0; i < geode.getNumDrawables(); ++i) {

			//
			// expand the overall bounding box
			//

			bbox.expandBy(geode.getDrawable(i)->getBound());
		}

		//
		// transform corners by current matrix
		//
		osg::BoundingBox bboxTrans;

		for (unsigned int i = 0; i < 8; ++i) {

			osg::Vec3 xvec = bbox.corner(i) * m_transformMatrix;

			bboxTrans.expandBy(xvec);

		}

		//
		// update the overall bounding box size
		//
		m_boundingBox.expandBy(bboxTrans);

		//
		// continue traversing through the graph
		//
		traverse(geode);

	} // ::apply(osg::Geode &geode)


	virtual
		void apply(osg::MatrixTransform &node) {
		// ---------------------------------------------------------
		//
		//   Handle nodes of the type osg::MatrixTransform
		//
		// ---------------------------------------------------------

		m_transformMatrix *= node.getMatrix();

		//
		// continue traversing through the graph
		//
		traverse(node);

	} // ::apply(osg::MatrixTransform &node)


	virtual
		void apply(osg::Billboard &node) {
		// -----------------------------------------------
		//
		//    Handle nodes of the type osg::Billboard
		//
		// -----------------------------------------------

		//
		// important to handle billboard so that its size will
		// not affect the geode size continue traversing the graph
		//
		traverse(node);

	} // ::apply(osg::MatrixTransform &node)


	  //
	  // return teh bounding box     
	  //
	osg::BoundingBox &getBoundBox() { return m_boundingBox; }

	osg::Matrix &getMat() { return m_transformMatrix; }

protected:

	osg::BoundingBox m_boundingBox;          // the overall resultant bounding box
	osg::Matrix      m_transformMatrix;      // the current transform matrix


};
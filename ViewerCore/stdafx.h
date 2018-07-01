// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#define _USE_MATH_DEFINES // for C++  
#include <cmath> 
using namespace std;

//#define _SCL_SECURE_NO_WARNINGS

#define VIEWER_API __declspec(dllexport)
#define R3DAPI __declspec(dllexport)
#define VIEWERDB_EXPORT __declspec(dllimport)

//#define OSGSHADOW_EXPORT __declspec(dllexport)

#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/api/win32/GraphicsWindowWin32>
#include <osgViewer/Viewer>
#include <osg/ref_ptr>
#include <osg/MatrixTransform>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/GraphicsWindow>

#include <osgGA/StateSetManipulator>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osgViewer/CompositeViewer>
#include <osg\PositionAttitudeTransform>
#include <osg\PrimitiveSet>
#include <osg\ShapeDrawable>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/CoordinateSystemNode>
#include <osgText/Text>

#include <osgManipulator/TabBoxDragger>
#include <osgManipulator/TabBoxTrackballDragger>
#include <osgManipulator/TabPlaneDragger>
#include <osgManipulator/TabPlaneTrackballDragger>
#include <osgManipulator/Scale1DDragger>
#include <osgManipulator/Scale2DDragger>
#include <osgManipulator/TrackballDragger>
#include <osgManipulator/Translate1DDragger>
#include <osgManipulator/Translate2DDragger>
#include <osgManipulator/TranslateAxisDragger>
#include <osgManipulator/TranslatePlaneDragger>
#include <osgManipulator/RotateCylinderDragger>
#include <osgManipulator/AntiSquish>
#include <osgUtil/SmoothingVisitor>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/io_utils>
#include <osgUtil/Tessellator>
#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowVolume>
#include <osgShadow/ShadowTexture>
#include <osgShadow/ShadowMap>
#include <osgShadow/SoftShadowMap>
#include <osgShadow/ParallelSplitShadowMap>
#include <osgShadow/LightSpacePerspectiveShadowMap>
#include <osgShadow/StandardShadowMap>
#include <osgShadow/ViewDependentShadowMap>

#include <iostream>



#include <ViewerDB.h>


typedef std::vector< osg::ref_ptr<osgViewer::View> > RefViews;
typedef osg::ref_ptr< osg::Node> OsgNode;
typedef osg::ref_ptr< osg::MatrixTransform> OsgMatrixTransform;


#include "EnumManager.h"
#include "Utilities.h"
#include "FGlobalData.h"
using namespace ViewerCore;

#pragma comment(lib, "ViewerDB.lib")










// TODO: reference additional headers your program requires here

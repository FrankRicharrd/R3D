// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#define R3DEXPORT __declspec(dllimport)
#define VIEWER_API __declspec(dllimport)


#include <EnumManager.h>
#include <FGlobalData.h>
#include <DraggerManager.h>
#include <RNode.h>
#include <RGeometryNode.h>
#include <RTransformNode.h>
#include <ViewerManager.h>


#pragma comment(lib, "ViewerDB.lib")
#pragma comment(lib, "ViewerCore.lib")
#pragma comment(lib, "R3DIntelligentObjects.lib")
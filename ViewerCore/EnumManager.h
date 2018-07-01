#pragma once

namespace ViewerCore
{


	enum MouseButton
	{
		Left = 1,
		Middle = 2,
		Right = 3,
	};

	enum class FViewType
	{
		Top = 0,
		Bottom = 1,
		Front = 2,
		Back = 3,
		Left = 4,
		Right = 5,
		Perspective = 6
	};

	enum class RPolygonMode
	{
		Wireframe = 0,
		Shaded = 1,
	};

	enum class DraggerType
	{
		TabBoxDragger = 0,
		TabPlaneDragger = 1,
		TabBoxTrackballDragger = 2,
		TrackballDragger = 3,
		Translate1DDragger = 4,
		Translate2DDragger = 5,
		TranslateAxisDragger = 6,
		TabPlaneTrackballDragger = 7,
		TranslatePlaneDragger = 8,
		Scale1DDragger = 9,
		Scale2DDragger = 10,
		RotateCylinderDragger = 11,
		RotateSphereDragger = 12,

		FaceDragger = 13,
		EdgeDragger = 14,
		VertexDragger =15,

	};


}
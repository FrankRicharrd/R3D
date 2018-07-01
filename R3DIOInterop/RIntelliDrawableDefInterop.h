#pragma once

#include "RDrawingEntity.h"
#include "RDrawingEntitiesInterop.h"
#include "RIntelligentObject.h"

#include "RIntelliDrawableInstanceInterop.h"

using namespace System::Collections::Generic;

namespace R3DIOInterop {
	public	ref class RIntelliDrawableDefInterop
	{
	public:
		RIntelliDrawableDefInterop(RIntelliDrawableDef* definition, RIntelligentObject* intelliObject);

		
		RIntelliDrawableInstanceInterop^ CreateInstance();

		//use this to create instance when draggers are active on this current IO object
		RIntelliDrawableInstanceInterop^ CreateDynamicInstance();


		// createface();

		//send vertex index for the outerloop of face in ccw direction, returns created face index. returns -1 if face not created
		int CreateFace(List<int>^ vertexIndex);

		// send true for initial draw and then false for updating 
		bool Redraw(bool Initdraw);

		//give the face index to create a dragger in the middle of the face
		void AddFaceDragger(int Faceindex);

		void SetDraggerVisibility(bool enable);

		RVertex3DInterop^ GetFaceNormal(int faceindex);



	private:
		RIntelliDrawableDef* m_definition;
		RIntelligentObject* m_intelliObject;
	};
}


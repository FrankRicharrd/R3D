#pragma once
#include "RIOConstrain.h"

namespace R3DIOInterop {

	public enum class RIOConstrainEntityType
	{
		Face,
	};

	//only negativedistance implemented
	public enum class RIOConstrainType
	{
		None,
		Coincident,
		Parallel,
		Perpendicular,
		Tangent,
		Concentric,
		Lock,
		//distance towards the positive normal of primary entity
		PositiveDistance,
		//distance towards the negative normal of primary entity
		NegativeDistance,
		Angle
	};

	public	ref class RIOConstrainManagerInterop
	{
	public:
		RIOConstrainManagerInterop(RIOConstrain* constrain, RIOConstrainEntityType m_type);
	
		void SetConstrainType(RIOConstrainType constType) { m_constrain->SetConstrainType((RIOConstrain::RIOConstrainType)constType); }

		void SetFaceConstrainDistance(double distance);

		RIOConstrainEntityType m_type;

	private:
		RIOConstrain* m_constrain;
	};
}

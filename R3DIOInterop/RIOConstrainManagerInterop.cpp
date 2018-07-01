#include "stdafx.h"
#include "RIOConstrainManagerInterop.h"

namespace R3DIOInterop {
	RIOConstrainManagerInterop::RIOConstrainManagerInterop(RIOConstrain* constrain, RIOConstrainEntityType type)
	{
		m_constrain = constrain;
		m_type = type;
	}

	void RIOConstrainManagerInterop::SetFaceConstrainDistance(double distance)
	{
		if (m_type != RIOConstrainEntityType::Face)
			return;

		RIOFaceConstrainRef faceconst = dynamic_cast<RIOFaceConstrain*>(m_constrain);
		faceconst->SetConstrainDistance(distance);
	}

	

}
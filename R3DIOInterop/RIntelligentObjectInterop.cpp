#include "stdafx.h"
#include "RIntelligentObject.h"
#include "RIntelliDrawableDefInterop.h"
#include "RIntelliDrawableInstanceInterop.h"
#include "RDrawingEntity.h"
#include "RIntelligentObjectInterop.h"
#include "RIOConstrain.h"

using namespace System::Collections::Generic;

namespace R3DIOInterop {
	RIntelligentObjectInterop::RIntelligentObjectInterop(RIntelligentObject* io)
	{
		m_intelligentobjectNative = io;
	}
	RIntelliDrawableDefInterop ^ RIntelligentObjectInterop::CreateIntelliDrawableDefinition(List<RVertex3DInterop^> ^ vertices)
	{
		RVertex3DCollection nvertices;
		nvertices.reserve(vertices->Count);

		for each (RVertex3DInterop^ var in vertices)
		{
			RVertex3DRef v = new RVertex3D(var->x, var->y, var->z);

			nvertices.push_back(v);
		}

		osg::ref_ptr<RIntelliDrawableDef> def = new RIntelliDrawableDef(nvertices, m_intelligentobjectNative);

		m_intelligentobjectNative->AddDefinition(def);

		return gcnew RIntelliDrawableDefInterop(def, m_intelligentobjectNative);

	}

	RIOTranslateDraggerInterop^ RIntelligentObjectInterop::CreateTranslateDragger(RVertex3DInterop ^ normal, RVertex3DInterop ^ position, List<RIntelliDrawableInstanceInterop^>^ attachedInstances, List<RIntelliDrawableInstanceInterop^>^ fixedInstances)
	{
		osg::Vec3 n(normal->x, normal->y, normal->z);
		osg::Vec3 p(position->x, position->y, position->z);

		RIntelliDrawableInstanceCollection instances;

		for each (RIntelliDrawableInstanceInterop^ item in attachedInstances)
		{
			instances.push_back(item->GetNativeObject());
		}

		RIntelliDrawableInstanceCollection fixedinstances;

		for each (RIntelliDrawableInstanceInterop^ item in fixedInstances)
		{
			fixedinstances.push_back(item->GetNativeObject());
		}

		auto dragger =  m_intelligentobjectNative->AddTranslateDragger(n, p, instances, fixedinstances);

		return gcnew RIOTranslateDraggerInterop(dragger.get());
	}

	void RIntelligentObjectInterop::Redraw(bool initdraw)
	{
		m_intelligentobjectNative->Redraw(initdraw);
	}
	void RIntelligentObjectInterop::SetIODraggerVisiblility(bool visible)
	{
		m_intelligentobjectNative->SetDraggerVisibility(visible);
	}
	RIOConstrainManagerInterop ^ RIntelligentObjectInterop::CreateFaceConstrain(KeyValuePair<RIntelliDrawableInstanceInterop^, int>^ primaryEntity, KeyValuePair<RIntelliDrawableInstanceInterop^, int>^ secondaryEntity)
	{
		RIntelliDrawableInstanceRef primaryInstance = primaryEntity->Key->GetNativeObject();

		RFaceRef face = (primaryInstance->GetIODefinition()->Getfaces()[primaryEntity->Value]);

		RIOFaceRef ioface = dynamic_cast<RIOFace*>(face.get());

		RIntelliDrawableInstanceRef secondaryInstance = secondaryEntity->Key->GetNativeObject();

		RFaceRef face2 = secondaryInstance->GetIODefinition()->Getfaces()[secondaryEntity->Value];

		RIOFaceRef ioface2 = dynamic_cast<RIOFace*>(face2.get());

		RIOFaceAndInstancePair primary = RIOFaceAndInstancePair(primaryInstance, ioface);

		RIOFaceAndInstancePair secondary = RIOFaceAndInstancePair(secondaryInstance, ioface2);

		RIOFaceConstrainRef constrain = new RIOFaceConstrain(primary, secondary);

		return gcnew RIOConstrainManagerInterop(constrain, RIOConstrainEntityType::Face);
	}
	
}
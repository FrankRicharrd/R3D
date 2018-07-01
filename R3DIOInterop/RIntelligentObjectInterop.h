#pragma once
#include "RDrawingEntitiesInterop.h"
#include "RIntelliDrawableDefInterop.h"
#include "RDrawingEntitiesInterop.h"
#include "RIOTranslateDraggerInterop.h"
#include "RIOConstrainManagerInterop.h"
namespace R3DIOInterop {
	public	ref class RIntelligentObjectInterop
	{
	public:
		RIntelligentObjectInterop(RIntelligentObject* io);

		RIntelliDrawableDefInterop^ CreateIntelliDrawableDefinition(System::Collections::Generic::List<RVertex3DInterop^> ^ vertices);

		RIOTranslateDraggerInterop^ CreateTranslateDragger(RVertex3DInterop^ normal, RVertex3DInterop^ position, List<RIntelliDrawableInstanceInterop^>^ attachedInstances, List<RIntelliDrawableInstanceInterop^>^ fixedInstances);

		void Redraw(bool initdraw);

		void SetIODraggerVisiblility(bool visible);

		//arguments are primary entity and secondary entity. keyvalue pair entity should hold the instance as key and face id as value,
		//Normal of Face on primary entity is taken as start point of distance or angle given for RIOConstraintype
		RIOConstrainManagerInterop^ CreateFaceConstrain(KeyValuePair<RIntelliDrawableInstanceInterop^, int>^ primaryEntity , KeyValuePair<RIntelliDrawableInstanceInterop^, int>^ secondaryEntity);

	private:
		RIntelligentObject* m_intelligentobjectNative;
	};
}

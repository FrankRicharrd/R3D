#pragma once
#include "RIntelligentObjectInterop.h"
#include <RIntelligentObjectManager.h>
	namespace R3DIOInterop
	{
		public ref class RIntelligentObjectManagerInterop : Viewer::Interfaces::IntelligentObject::IRIntelligentObjectManager
		{
		public:
			RIntelligentObjectManagerInterop(Viewer::Interfaces::IRTransformNode^ rootParentNode);

			void CreateObj(osg::ref_ptr<RIntelligentObject> obj);

			// Inherited via IRIntelligentObjectManager
			virtual Viewer::Interfaces::IRTransformNode ^ CreateEmpytyObject();

			RIntelligentObjectInterop^ CreateIntelligentObject();


			~RIntelligentObjectManagerInterop();

		private:
			RIntelligentObjectManager* m_intelligentObjectManager;
			Viewer::Interfaces::IRTransformNode^ m_rootParentNode;
		};

	}


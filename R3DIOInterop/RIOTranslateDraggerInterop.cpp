#include "stdafx.h"
#include "RIOTranslateDraggerInterop.h"
using namespace System;
using namespace System::Runtime::InteropServices;

namespace R3DIOInterop {
	RIOTranslateDraggerInterop::RIOTranslateDraggerInterop(RIOTranslateDragger* dragger) : m_dragger(dragger)
	{
		
	}
	void RIOTranslateDraggerInterop::SubscribeBeforeUpdatingEvent(BeforeUpdatingDelegate ^ callback)
	{
		BeforeUpdatingVar = callback;

		osgManipulator::DraggerCallback* dcallback = m_dragger->getDraggerCallbacks()[0];

		RIOTranslateDraggerCallback* riocallback = dynamic_cast<RIOTranslateDraggerCallback*>(dcallback);


		pin_ptr<BeforeUpdatingDelegate^> tmp = &BeforeUpdatingVar;

		riocallback->BeforeUpdatingConstraints = (bool(__cdecl *)(double, double, double))(Marshal::GetFunctionPointerForDelegate(BeforeUpdatingVar).ToPointer());
	}
	void RIOTranslateDraggerInterop::SubscribeOnReleaseEvent(OnReleaseDelegate ^ callback)
	{
		OnReleaseVar = callback;

		osgManipulator::DraggerCallback* dcallback = m_dragger->getDraggerCallbacks()[0];

		RIOTranslateDraggerCallback* riocallback = dynamic_cast<RIOTranslateDraggerCallback*>(dcallback);


		pin_ptr<OnReleaseDelegate^> tmp = &OnReleaseVar;

		riocallback->OnRelease = (bool(__cdecl *)(void))(Marshal::GetFunctionPointerForDelegate(OnReleaseVar).ToPointer());
	}
}

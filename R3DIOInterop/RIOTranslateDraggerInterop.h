#pragma once
#include "RIOTranslateDragger.h"
namespace R3DIOInterop {

	public delegate bool  BeforeUpdatingDelegate(double, double, double);

	public delegate bool  OnReleaseDelegate();

	public ref class RIOTranslateDraggerInterop
	{
	public:
		RIOTranslateDraggerInterop(RIOTranslateDragger* dragger);

		
		void SubscribeBeforeUpdatingEvent(BeforeUpdatingDelegate^ callback);

		void SubscribeOnReleaseEvent(OnReleaseDelegate^ callback);


	private:
		RIOTranslateDragger* m_dragger;
		BeforeUpdatingDelegate^ BeforeUpdatingVar;
		OnReleaseDelegate^ OnReleaseVar;
	};
}


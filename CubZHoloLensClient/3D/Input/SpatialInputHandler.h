#pragma once

#include <3D\Input\InteractionListener.h>

namespace HoloLensClient
{
    // Sample gesture handler.
    // Hooks up events to recognize a tap gesture, and keeps track of input using a boolean value.
    class SpatialInputHandler
    {
	private:
		InteractionListener			*_interactionListener;

    public:
        SpatialInputHandler(InteractionListener *listener);
        ~SpatialInputHandler();

    private:
		void OnInteractionDetected(
			Windows::UI::Input::Spatial::SpatialInteractionManager^ sender,
			Windows::UI::Input::Spatial::SpatialInteractionDetectedEventArgs^ args);

        // API objects used to process gesture input, and generate gesture events.
        Windows::UI::Input::Spatial::SpatialInteractionManager^     _interactionManager;

		Windows::Foundation::EventRegistrationToken                 _interactionDetectedEventToken;
    };
}

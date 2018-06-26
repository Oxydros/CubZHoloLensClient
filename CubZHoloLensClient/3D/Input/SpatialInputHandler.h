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

        Windows::UI::Input::Spatial::SpatialInteractionSourceState^ CheckForInput();

    private:
        // Interaction event handler.
        void OnSourcePressed(
            Windows::UI::Input::Spatial::SpatialInteractionManager^ sender,
            Windows::UI::Input::Spatial::SpatialInteractionSourceEventArgs^ args);

		void OnInteractionDetected(
			Windows::UI::Input::Spatial::SpatialInteractionManager^ sender,
			Windows::UI::Input::Spatial::SpatialInteractionDetectedEventArgs^ args);

        // API objects used to process gesture input, and generate gesture events.
        Windows::UI::Input::Spatial::SpatialInteractionManager^     _interactionManager;

        // Event registration tokens.
        Windows::Foundation::EventRegistrationToken                 _sourcePressedEventToken;
		Windows::Foundation::EventRegistrationToken                 _interactionDetectedEventToken;

        // Used to indicate that a Pressed input event was received this frame.
        Windows::UI::Input::Spatial::SpatialInteractionSourceState^ _sourceState = nullptr;
    };
}

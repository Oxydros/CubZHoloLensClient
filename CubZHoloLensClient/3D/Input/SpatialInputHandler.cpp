#include "pch.h"
#include "3D\Input\SpatialInputHandler.h"
#include <functional>
#include "3D\Input\InputHandler.h"

using namespace HoloLensClient;

using namespace Windows::Foundation;
using namespace Windows::UI::Input::Spatial;
using namespace std::placeholders;

// Creates and initializes a GestureRecognizer that listens to a Person.
SpatialInputHandler::SpatialInputHandler(InteractionListener *listener)
	: _interactionListener(listener)
{
    // The interaction manager provides an event that informs the app when
    // spatial interactions are detected.
    _interactionManager = SpatialInteractionManager::GetForCurrentView();

    // Bind a handler to the SourcePressed event.
    _sourcePressedEventToken =
        _interactionManager->SourcePressed +=
            ref new TypedEventHandler<SpatialInteractionManager^, SpatialInteractionSourceEventArgs^>(
                bind(&SpatialInputHandler::OnSourcePressed, this, std::placeholders::_1, std::placeholders::_2)
                );

	// Bind a handler to the InteractionDetectedEvent
	// Used for the SpatialGestureRecognizer
    _interactionDetectedEventToken =
		_interactionManager->InteractionDetected +=
		ref new TypedEventHandler<SpatialInteractionManager^, SpatialInteractionDetectedEventArgs^>(
				bind(&SpatialInputHandler::OnInteractionDetected, this, std::placeholders::_1, std::placeholders::_2)
				);
}

SpatialInputHandler::~SpatialInputHandler()
{
    // Unregister our handler for the OnSourcePressed event.
    _interactionManager->SourcePressed -= _sourcePressedEventToken;
	_interactionManager->InteractionDetected -= _interactionDetectedEventToken;
}

// Checks if the user performed an input gesture since the last call to this method.
// Allows the main update loop to check for asynchronous changes to the user
// input state.
SpatialInteractionSourceState^ SpatialInputHandler::CheckForInput()
{
    SpatialInteractionSourceState^ sourceState = _sourceState;
    _sourceState = nullptr;
    return sourceState;
}

void SpatialInputHandler::OnSourcePressed(SpatialInteractionManager^ sender, SpatialInteractionSourceEventArgs^ args)
{
    _sourceState = args->State;

    //
    // TODO: In your app or game engine, rewrite this method to queue
    //       input events in your input class or event handler.
    //
}

void SpatialInputHandler::OnInteractionDetected(SpatialInteractionManager^ sender, SpatialInteractionDetectedEventArgs^ args)
{
	_interactionListener->InteractionDetectedEvent(sender, args);
}

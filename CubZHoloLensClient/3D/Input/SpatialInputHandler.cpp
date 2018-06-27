#include "pch.h"
#include "3D\Input\SpatialInputHandler.h"
#include <functional>

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
	_interactionManager->InteractionDetected -= _interactionDetectedEventToken;
}

void SpatialInputHandler::OnInteractionDetected(SpatialInteractionManager^ sender, SpatialInteractionDetectedEventArgs^ args)
{
	_interactionListener->InteractionDetectedEvent(sender, args);
}

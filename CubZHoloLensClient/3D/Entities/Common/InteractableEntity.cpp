#include <pch.h>
#include "InteractableEntity.h"

HoloLensClient::InteractableEntity::InteractableEntity(std::shared_ptr<HolographicScene> scene,
	Spatial::SpatialGestureSettings settings, CubZHoloLensClient::WinNetwork::EntityDescription const & entityDesc)
	: Entity(scene, entityDesc)
{
	Spatial::SpatialGestureRecognizer ^spatial = ref new Spatial::SpatialGestureRecognizer(settings);

	_tappedToken = spatial->Tapped +=
		ref new Windows::Foundation::TypedEventHandler<Spatial::SpatialGestureRecognizer^,
		Spatial::SpatialTappedEventArgs ^>(
			std::bind(&InteractableEntity::OnAirTap, this, std::placeholders::_1, std::placeholders::_2)
			);

	_manipStartedToken = spatial->ManipulationStarted +=
		ref new Windows::Foundation::TypedEventHandler<Spatial::SpatialGestureRecognizer^,
		Spatial::SpatialManipulationStartedEventArgs ^>(
			std::bind(&InteractableEntity::OnManipulationStarted, this, std::placeholders::_1, std::placeholders::_2)
			);

	_manipUpdatedToken = spatial->ManipulationUpdated +=
		ref new Windows::Foundation::TypedEventHandler<Spatial::SpatialGestureRecognizer^,
		Spatial::SpatialManipulationUpdatedEventArgs ^>(
			std::bind(&InteractableEntity::OnManipulationUpdated, this, std::placeholders::_1, std::placeholders::_2)
			);

	_manipCompletedToken = spatial->ManipulationCompleted +=
		ref new Windows::Foundation::TypedEventHandler<Spatial::SpatialGestureRecognizer^,
		Spatial::SpatialManipulationCompletedEventArgs ^>(
			std::bind(&InteractableEntity::OnManipulationCompleted, this, std::placeholders::_1, std::placeholders::_2)
			);

	_manipCanceledToken = spatial->ManipulationCanceled +=
		ref new Windows::Foundation::TypedEventHandler<Spatial::SpatialGestureRecognizer^,
		Spatial::SpatialManipulationCanceledEventArgs ^>(
			std::bind(&InteractableEntity::OnManipulationCanceled, this, std::placeholders::_1, std::placeholders::_2)
			);

	SetSpatialGestureRecognizer(spatial);
}

HoloLensClient::InteractableEntity::~InteractableEntity()
{
	auto spatial = GetSpatialGestureRecognizer();
	if (spatial)
	{
		spatial->Tapped -= _tappedToken;
		spatial->ManipulationStarted -= _manipStartedToken;
		spatial->ManipulationUpdated -= _manipUpdatedToken;
		spatial->ManipulationCompleted -= _manipCompletedToken;
		spatial->ManipulationCanceled -= _manipCanceledToken;
	}	
}

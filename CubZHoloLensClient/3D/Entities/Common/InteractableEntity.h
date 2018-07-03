#pragma once

#include <3D\Entities\Common\Entity.h>

using namespace Windows::UI::Input;

namespace HoloLensClient
{
	class InteractableEntity : public Entity
	{
	public:
		typedef std::function<void(Spatial::SpatialGestureRecognizer ^sender, Spatial::SpatialTappedEventArgs ^args)> AirTapCallback;

		typedef std::function<void(Spatial::SpatialGestureRecognizer^,	Spatial::SpatialManipulationStartedEventArgs ^)> ManipulationStartedCallback;
		typedef std::function<void(Spatial::SpatialGestureRecognizer^, Spatial::SpatialManipulationUpdatedEventArgs ^)> ManipulationUpdatedCallback;
		typedef std::function<void(Spatial::SpatialGestureRecognizer^, Spatial::SpatialManipulationCompletedEventArgs ^)> ManipulationCompletedCallback;
		typedef std::function<void(Spatial::SpatialGestureRecognizer^, Spatial::SpatialManipulationCanceledEventArgs ^)> ManipulationCanceledCallback;

	private:
		Windows::Foundation::EventRegistrationToken _tappedToken;
		Windows::Foundation::EventRegistrationToken _manipStartedToken;
		Windows::Foundation::EventRegistrationToken _manipUpdatedToken;
		Windows::Foundation::EventRegistrationToken _manipCompletedToken;
		Windows::Foundation::EventRegistrationToken _manipCanceledToken;

		AirTapCallback								_airTapCallback;
		ManipulationStartedCallback					_manipStart;
		ManipulationUpdatedCallback					_manipUpdate;
		ManipulationCompletedCallback				_manipComplete;
		ManipulationCanceledCallback				_manipCanceled;

	public:
		InteractableEntity(std::shared_ptr<HolographicScene> scene,
							Spatial::SpatialGestureSettings = Spatial::SpatialGestureSettings::None,
							CubZHoloLensClient::WinNetwork::EntityDescription const &entityDesc = {});
		~InteractableEntity();

	public:
		void SetAirTapCallback(AirTapCallback callback) { _airTapCallback = callback; }
		void SetManipulationStartedCallback(ManipulationStartedCallback callback) { _manipStart = callback; }
		void SetManipulationUpdatedCallback(ManipulationUpdatedCallback callback) { _manipUpdate = callback; }
		void SetManipulationCompletedCallback(ManipulationCompletedCallback callback) { _manipComplete = callback; }
		void SetManipulationCanceledCallback(ManipulationCanceledCallback callback) { _manipCanceled = callback; }

	private:
		virtual void OnAirTap(Spatial::SpatialGestureRecognizer ^sender,
			Spatial::SpatialTappedEventArgs ^args) {
			if (_airTapCallback) _airTapCallback(sender, args);
		}

		virtual void OnManipulationStarted(Spatial::SpatialGestureRecognizer^ sender,
			Spatial::SpatialManipulationStartedEventArgs ^args) {
			if (_manipStart) _manipStart(sender, args);
		}
		virtual void OnManipulationUpdated(Spatial::SpatialGestureRecognizer^ sender,
			Spatial::SpatialManipulationUpdatedEventArgs ^args) {
			if (_manipUpdate) _manipUpdate(sender, args);
		}
		virtual void OnManipulationCompleted(Spatial::SpatialGestureRecognizer^ sender,
			Spatial::SpatialManipulationCompletedEventArgs ^args) {
			if (_manipComplete) _manipComplete(sender, args);
		}
		virtual void OnManipulationCanceled(Spatial::SpatialGestureRecognizer^ sender,
			Spatial::SpatialManipulationCanceledEventArgs ^args) {
			if (_manipCanceled) _manipCanceled(sender, args);
		}
	};
}
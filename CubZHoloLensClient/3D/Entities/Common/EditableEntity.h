#pragma once

#include <3D\Entities\Common\Entity.h>

using namespace Windows::UI::Input;

namespace HoloLensClient
{
	class EditableEntity : public Entity
	{
	public:
		enum State
		{
			IDLE = 0,
			ADJUST,
			ROTATE,
		};

	private:
		Windows::Foundation::EventRegistrationToken _tappedToken;
		Windows::Foundation::EventRegistrationToken _manipStartedToken;
		Windows::Foundation::EventRegistrationToken _manipUpdatedToken;
		Windows::Foundation::EventRegistrationToken _manipCompletedToken;
		Windows::Foundation::EventRegistrationToken _manipCanceledToken;

	protected:
		State				_state{ IDLE };
		bool				_moving{ false };

	public:
		EditableEntity(std::shared_ptr<DX::DeviceResources> devicesResources,
			std::shared_ptr<HolographicScene> scene);
		virtual ~EditableEntity();

	public:
		virtual bool OnGetFocus() override;
		virtual bool OnLostFocus() override;

		virtual void OnKillClick();

	private:
		void OnAirTap(Spatial::SpatialGestureRecognizer ^sender,
			Spatial::SpatialTappedEventArgs ^args);

		void OnManipulationStarted(Spatial::SpatialGestureRecognizer^,
			Spatial::SpatialManipulationStartedEventArgs ^);
		void OnManipulationUpdated(Spatial::SpatialGestureRecognizer^,
			Spatial::SpatialManipulationUpdatedEventArgs ^);
		void OnManipulationCompleted(Spatial::SpatialGestureRecognizer^,
			Spatial::SpatialManipulationCompletedEventArgs ^);
		void OnManipulationCanceled(Spatial::SpatialGestureRecognizer^,
			Spatial::SpatialManipulationCanceledEventArgs ^);

	public:
		bool IsMoving() const { return _moving; }
		void StopMoving();
		void StartMoving();
		void setEditableState(State state) { _state = state; }
	};
}
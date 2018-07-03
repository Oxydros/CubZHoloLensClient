#pragma once

#include <3D\Entities\Common\InteractableEntity.h>

using namespace Windows::UI::Input;

namespace HoloLensClient
{
	class EditableEntity : public InteractableEntity
	{
	public:
		enum State
		{
			IDLE = 0,
			ADJUST,
			ROTATE,
		};

	protected:
		State				_state{ IDLE };
		bool				_moving{ false };

	public:
		EditableEntity(std::shared_ptr<DX::DeviceResources> devicesResources,
			std::shared_ptr<HolographicScene> scene,
			CubZHoloLensClient::WinNetwork::EntityDescription const &entityDesc = {});
		virtual ~EditableEntity();

	public:
		virtual bool OnGetFocus() override;
		virtual bool OnLostFocus() override;
		void kill() override;

		virtual void OnKillClick();

	private:
		void OnAirTap(Spatial::SpatialGestureRecognizer ^sender,
			Spatial::SpatialTappedEventArgs ^args);

		void OnManipulationStarted(Spatial::SpatialGestureRecognizer^,
			Spatial::SpatialManipulationStartedEventArgs ^) override;
		void OnManipulationUpdated(Spatial::SpatialGestureRecognizer^,
			Spatial::SpatialManipulationUpdatedEventArgs ^) override;
		void OnManipulationCompleted(Spatial::SpatialGestureRecognizer^,
			Spatial::SpatialManipulationCompletedEventArgs ^) override;
		void OnManipulationCanceled(Spatial::SpatialGestureRecognizer^,
			Spatial::SpatialManipulationCanceledEventArgs ^) override;

	public:
		bool IsMoving() const { return _moving; }
		void StopMoving();
		void StartMoving();
		void setEditableState(State state) { _state = state; }
	};
}
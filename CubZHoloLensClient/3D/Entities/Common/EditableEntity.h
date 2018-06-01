#pragma once

#include <3D\Entities\Common\InteractableEntity.h>

namespace HoloLensClient
{
	class EditableEntity : public InteractableEntity
	{
	public:
		enum State
		{
			IDLE = 0,
			MOVE,
			ROTATE,
		};

	protected:
		State				_state{ IDLE };
		bool				_moving{ false };

	public:
		EditableEntity(std::shared_ptr<DX::DeviceResources> devicesResources,
						std::shared_ptr<HolographicScene> scene);
		virtual ~EditableEntity() = default;

	public:
		virtual bool OnGetFocus() override;
		virtual bool OnLostFocus() override;
		virtual bool OnAirTap() override;

		virtual void OnRotateLeftClick();
		virtual void OnRotateRightClick();
		virtual void OnMoveClick();
		virtual void OnKillClick();

	public:
		bool IsMoving() const { return _moving; }
		void StopMoving();
		void StartMoving();
		void setEditableState(State state) { _state = state; }
	};
}
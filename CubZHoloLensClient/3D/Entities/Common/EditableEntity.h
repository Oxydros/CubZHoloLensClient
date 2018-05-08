#pragma once

#include <3D\Entities\Common\InteractableEntity.h>

namespace HoloLensClient
{
	class EditableEntity : public InteractableEntity
	{
	public:
		EditableEntity(std::shared_ptr<DX::DeviceResources> devicesResources,
						std::shared_ptr<HolographicScene> scene);
		virtual ~EditableEntity() = default;

	public:
		virtual void OnRotateLeftClick() = 0;
		virtual void OnRotateRightClick() = 0;
		virtual void OnMoveClick() = 0;
		virtual void OnKillClick() = 0;
	};
}
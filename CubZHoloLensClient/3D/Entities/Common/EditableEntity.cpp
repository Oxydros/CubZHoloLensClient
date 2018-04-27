#include <pch.h>
#include <3D\Entities\GUI\ModificationMenu.h>
#include "EditableEntity.h"

HoloLensClient::EditableEntity::EditableEntity(std::shared_ptr<DX::DeviceResources> devicesResources,
											   std::shared_ptr<HolographicScene> scene)
	: InteractableEntity(scene)
{
}

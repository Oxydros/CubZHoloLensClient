#include <pch.h>
#include <3D\Scene\HolographicScene.h>
#include <3D\Entities\Common\InteractableEntity.h>

using namespace HoloLensClient;
using namespace DirectX;
using namespace Windows::Foundation::Numerics;

InteractableEntity::InteractableEntity(std::shared_ptr<HolographicScene> scene, bool disableMultipleClick)
	: Entity(scene), _disableMultipleClick(disableMultipleClick)
{
}

void HoloLensClient::InteractableEntity::OnInputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState ^pointerState)
{
	if (pointerState != nullptr && pointerState->IsPressed && _focused)
	{
		TRACE("Process onInputs on " << GetLabel() << std::endl);
		// Prevent multiple click
		// Check if it was already clicked and cancel if it was the case
		if (_disableMultipleClick && _clicked)
			return;
		_clicked = true;
		Concurrency::task<void> callbackTask = Concurrency::create_task([this]()
		{
			OnAirTap();
		});
	}
	else
		_clicked = false;
}

#include "pch.h"
#include "Button.h"

HoloLensClient::Button::Button(std::shared_ptr<HolographicScene>& scene,
							   std::function<void()> &callback,
							   Windows::Foundation::Numerics::float2 size)
	: InteractableEntity(scene), _callback(callback)
{
}

void HoloLensClient::Button::OnAirTap()
{
	_callback();
}

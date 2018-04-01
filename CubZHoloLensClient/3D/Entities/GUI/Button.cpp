#include "pch.h"
#include "Button.h"
#include <3D\Objects\GUI\ButtonObject.h>

HoloLensClient::Button::Button(std::shared_ptr<DX::DeviceResources> &devicesResources, 
							   std::shared_ptr<HolographicScene>& scene,
							   std::function<void()> const &callback,
							   Windows::Foundation::Numerics::float2 size)
	: InteractableEntity(scene), _callback(callback)
{
	auto buttonMesh = std::make_unique<ButtonObject>(devicesResources, size);
	addMesh(std::move(buttonMesh));
}

void HoloLensClient::Button::OnAirTap()
{
	if (_callback) _callback();
}

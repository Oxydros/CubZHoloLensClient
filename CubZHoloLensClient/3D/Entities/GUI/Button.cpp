#include "pch.h"
#include "Button.h"
#include <3D\Objects\GUI\ButtonObject.h>

Windows::Foundation::Numerics::float4 HoloLensClient::Button::DEFAULT_COLOR = { 0.243f, 0.815f, 0.854f, 1.0f };
Windows::Foundation::Numerics::float4 HoloLensClient::Button::FOCUS_COLOR = { 0.443f, 0.515f, 0.454f, 1.0f };

HoloLensClient::Button::Button(std::shared_ptr<DX::DeviceResources> devicesResources, 
							   std::shared_ptr<HolographicScene> scene,
							   std::function<void()> const &callback,
							   Windows::Foundation::Numerics::float2 size)
	: GUIEntity(scene), _callback(callback)
{
	_color = DEFAULT_COLOR;
	auto buttonMesh = std::make_unique<ButtonObject>(devicesResources, size, _color);
	addMesh(std::move(buttonMesh));
}

void HoloLensClient::Button::OnLabelChanged()
{
	auto button = dynamic_cast<ButtonObject*>(_mesh.get());
	button->setText(getLabel());
}

bool HoloLensClient::Button::OnGetFocus()
{
	auto button = dynamic_cast<ButtonObject*>(_mesh.get());
	button->setColor(FOCUS_COLOR);
	return (true);
}

bool HoloLensClient::Button::OnLostFocus()
{
	auto button = dynamic_cast<ButtonObject*>(_mesh.get());
	button->setColor(DEFAULT_COLOR);
	return (true);
}

bool HoloLensClient::Button::OnAirTap()
{
	if (_callback) _callback();
	return (true);
}

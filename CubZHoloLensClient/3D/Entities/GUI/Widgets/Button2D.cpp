#include "pch.h"
#include <3D\Entities\GUI\Widgets\Button2D.h>
#include <3D\Objects\GUI\ButtonObject.h>

Windows::Foundation::Numerics::float4 HoloLensClient::Button2D::DEFAULT_COLOR = { 0.243f, 0.815f, 0.854f, 1.0f };
Windows::Foundation::Numerics::float4 HoloLensClient::Button2D::FOCUS_COLOR = { 0.443f, 0.515f, 0.454f, 1.0f };

HoloLensClient::Button2D::Button2D(std::shared_ptr<DX::DeviceResources> devicesResources,
							   std::shared_ptr<HolographicScene> scene,
							   Windows::Foundation::Numerics::float2 size)
	: GUIEntity(scene, Spatial::SpatialGestureSettings::Tap)
{
	_color = DEFAULT_COLOR;
	SetSize({ size.x, size.y, 0.0f });
	auto buttonMesh = std::make_unique<ButtonObject>(devicesResources, size, _color);
	addMesh(std::move(buttonMesh));
}

HoloLensClient::Button2D::~Button2D()
{}

void HoloLensClient::Button2D::OnLabelChanged()
{
	auto button = dynamic_cast<ButtonObject*>(_mesh.get());
	button->setText(getLabel());
}

bool HoloLensClient::Button2D::OnGetFocus()
{
	auto button = dynamic_cast<ButtonObject*>(_mesh.get());
	button->setColor(FOCUS_COLOR);
	return (true);
}

bool HoloLensClient::Button2D::OnLostFocus()
{
	auto button = dynamic_cast<ButtonObject*>(_mesh.get());
	button->setColor(DEFAULT_COLOR);
	return (true);
}

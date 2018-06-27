#include "pch.h"
#include <3D\Entities\GUI\Button3D.h>
#include <3D\Objects\Mesh\3DFormes\ColoredCube.h>

Windows::Foundation::Numerics::float4 HoloLensClient::Button3D::DEFAULT_COLOR = { 0.243f, 0.815f, 0.854f, 1.0f };
Windows::Foundation::Numerics::float4 HoloLensClient::Button3D::FOCUS_COLOR = { 0.443f, 0.515f, 0.454f, 1.0f };

HoloLensClient::Button3D::Button3D(std::shared_ptr<DX::DeviceResources> devicesResources,
	std::shared_ptr<HolographicScene> scene,
	std::function<void()> const &callback,
	Windows::Foundation::Numerics::float3 size)
	: GUIEntity(scene), _callback(callback)
{
	_color = DEFAULT_COLOR;
	SetSize(size);
	auto buttonMesh = std::make_unique<ColoredCube>(devicesResources, size, _color);
	addMesh(std::move(buttonMesh));
}

bool HoloLensClient::Button3D::OnGetFocus()
{
	auto button = dynamic_cast<ColoredCube*>(_mesh.get());
	button->SetColor(FOCUS_COLOR);
	return (true);
}

bool HoloLensClient::Button3D::OnLostFocus()
{
	auto button = dynamic_cast<ColoredCube*>(_mesh.get());
	button->SetColor(DEFAULT_COLOR);
	return (true);
}

void HoloLensClient::Button3D::OnAirTap(Spatial::SpatialGestureRecognizer ^sender,
										Spatial::SpatialTappedEventArgs ^args)
{
	if (_callback) _callback();
}

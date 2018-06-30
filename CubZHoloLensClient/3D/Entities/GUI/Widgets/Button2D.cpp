#include "pch.h"
#include <3D\Entities\GUI\Widgets\Button2D.h>
#include <3D\Objects\GUI\ButtonObject.h>

Windows::Foundation::Numerics::float4 HoloLensClient::Button2D::DEFAULT_COLOR = { 0.243f, 0.815f, 0.854f, 1.0f };
Windows::Foundation::Numerics::float4 HoloLensClient::Button2D::FOCUS_COLOR = { 0.443f, 0.515f, 0.454f, 1.0f };

HoloLensClient::Button2D::Button2D(std::shared_ptr<DX::DeviceResources> devicesResources,
							   std::shared_ptr<HolographicScene> scene,
							   std::function<void()> const &callback,
							   Windows::Foundation::Numerics::float2 size)
	: GUIEntity(scene), _callback(callback)
{
	_color = DEFAULT_COLOR;
	SetSize({ size.x, size.y, 0.0f });
	auto buttonMesh = std::make_unique<ButtonObject>(devicesResources, size, _color);
	addMesh(std::move(buttonMesh));

	Spatial::SpatialGestureRecognizer ^spatial = ref new Spatial::SpatialGestureRecognizer(Spatial::SpatialGestureSettings::Tap);

	_tappedToken = spatial->Tapped +=
		ref new Windows::Foundation::TypedEventHandler<Spatial::SpatialGestureRecognizer^,
		Spatial::SpatialTappedEventArgs ^>(
			std::bind(&Button2D::OnAirTap, this, std::placeholders::_1, std::placeholders::_2)
			);

	SetSpatialGestureRecognizer(spatial);
}

HoloLensClient::Button2D::~Button2D()
{
	auto spatial = GetSpatialGestureRecognizer();
	if (spatial)
		spatial->Tapped -= _tappedToken;
}

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

void HoloLensClient::Button2D::OnAirTap(Spatial::SpatialGestureRecognizer ^sender,
										Spatial::SpatialTappedEventArgs ^args)
{
	if (_callback) _callback();
}

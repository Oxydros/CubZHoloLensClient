#include <pch.h>
#include <3D\Objects\GUI\ButtonObject.h>

HoloLensClient::ButtonObject::ButtonObject(std::shared_ptr<DX::DeviceResources>& devicesResources,
											Windows::Foundation::Numerics::float2 size,
											Windows::Foundation::Numerics::float4 backgroundColor)
	: _deviceResources(devicesResources), _size(size), _color(backgroundColor)
{
	_textMesh = std::make_unique<TextObject>(devicesResources, size, backgroundColor);
	/*_backgroundMesh = std::make_unique<ColoredRectangle>(devicesResources, size, backgroundColor);*/
}

void HoloLensClient::ButtonObject::CreateDeviceDependentResources()
{
	_textMesh->CreateDeviceDependentResources();
	//_backgroundMesh->CreateDeviceDependentResources();
}

void HoloLensClient::ButtonObject::ReleaseDeviceDependentResources()
{
	_textMesh->ReleaseDeviceDependentResources();
	//_backgroundMesh->ReleaseDeviceDependentResources();
}

void HoloLensClient::ButtonObject::Render()
{
	_textMesh->Render();
	//_backgroundMesh->Render();
}

void HoloLensClient::ButtonObject::SetModelTransform(DirectX::XMMATRIX const &modelTransform)
{
	//_backgroundMesh->ApplyMatrix(modelTransform);
	_textMesh->SetModelTransform(modelTransform);
}

std::shared_ptr<DX::DeviceResources> HoloLensClient::ButtonObject::getDeviceResources() const
{
	return (_deviceResources);
}

void HoloLensClient::ButtonObject::GetBoundingBox(DirectX::BoundingOrientedBox &boundingBox)
{
	return (_textMesh->GetBoundingBox(boundingBox));
}

std::wstring const & HoloLensClient::ButtonObject::getText() const
{
	return (_textMesh->getText());
}

void HoloLensClient::ButtonObject::setText(std::wstring const & text)
{
	_textMesh->setText(text);
}

float const HoloLensClient::ButtonObject::getFontSize() const
{
	return _textMesh->getFontSize();
}

void HoloLensClient::ButtonObject::setFontSize(float font)
{
	_textMesh->setFontSize(font);
}

void HoloLensClient::ButtonObject::setColor(Windows::Foundation::Numerics::float4 color)
{
	_color = color;
	_textMesh->setColor(color);
}

Windows::Foundation::Numerics::float4 HoloLensClient::ButtonObject::getColor() const
{
	return _color;
}

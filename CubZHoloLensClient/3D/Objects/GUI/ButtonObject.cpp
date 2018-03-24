#include <pch.h>
#include <3D\Objects\GUI\ButtonObject.h>

HoloLensClient::ButtonObject::ButtonObject(std::shared_ptr<DX::DeviceResources>& devicesResources,
											Windows::Foundation::Numerics::float2 size,
											Windows::Foundation::Numerics::float4 backgroundColor)
	: _deviceResources(devicesResources), _size(size), _color(backgroundColor)
{
	_textMesh = std::make_unique<TextObject>(devicesResources, size);
	_backgroundMesh = std::make_unique<ColoredRectangle>(devicesResources, size, backgroundColor);
}

void HoloLensClient::ButtonObject::CreateDeviceDependentResources()
{
	_textMesh->CreateDeviceDependentResources();
	_backgroundMesh->CreateDeviceDependentResources();
}

void HoloLensClient::ButtonObject::ReleaseDeviceDependentResources()
{
	_textMesh->ReleaseDeviceDependentResources();
	_backgroundMesh->ReleaseDeviceDependentResources();
}

void HoloLensClient::ButtonObject::Update()
{
	_textMesh->Update();
	_backgroundMesh->Update();
}

void HoloLensClient::ButtonObject::Render()
{
	_textMesh->Render();
	//_backgroundMesh->Render();
}

void HoloLensClient::ButtonObject::ApplyMatrix(DirectX::XMMATRIX const &modelTransform)
{
	_backgroundMesh->ApplyMatrix(modelTransform);
	_textMesh->ApplyMatrix(modelTransform);
}

void HoloLensClient::ButtonObject::SetPosition(Windows::Foundation::Numerics::float3 pos)
{
	_backgroundMesh->SetPosition(pos);
	_textMesh->SetPosition(pos);
}

void HoloLensClient::ButtonObject::SetRotation(Windows::Foundation::Numerics::float3 rot)
{
	_backgroundMesh->SetRotation(rot);
	_textMesh->SetRotation(rot);
}

void HoloLensClient::ButtonObject::SetScale(Windows::Foundation::Numerics::float3 scale)
{
	_backgroundMesh->SetScale(scale);
	_textMesh->SetScale(scale);
}

Windows::Foundation::Numerics::float3 HoloLensClient::ButtonObject::GetPosition() const
{
	return (_backgroundMesh->GetPosition());
}

Windows::Foundation::Numerics::float3 HoloLensClient::ButtonObject::GetRotation() const
{
	return (_backgroundMesh->GetRotation());
}

Windows::Foundation::Numerics::float3 HoloLensClient::ButtonObject::GetScale() const
{
	return (_backgroundMesh->GetScale());
}

void HoloLensClient::ButtonObject::SetPosition(DirectX::XMMATRIX const & posMatrix)
{
	_backgroundMesh->SetPosition(posMatrix);
	_textMesh->SetPosition(posMatrix);
}

void HoloLensClient::ButtonObject::SetRotation(DirectX::XMMATRIX const & rotMatrix)
{
	_backgroundMesh->SetRotation(rotMatrix);
	_textMesh->SetRotation(rotMatrix);
}

DirectX::XMMATRIX const & HoloLensClient::ButtonObject::getPositionMatrix() const
{
	return (_backgroundMesh->getPositionMatrix());
}

DirectX::XMMATRIX const & HoloLensClient::ButtonObject::getRotationMatrix() const
{
	return (_backgroundMesh->getRotationMatrix());
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

#include "pch.h"
#include "3D\Entities\Entity.h"

using namespace HoloLensClient;

Entity::Entity(std::shared_ptr<HolographicScene> &scene) : _alive(true), _scene(scene)
{
}

void HoloLensClient::Entity::Update(DX::StepTimer const & timer)
{
	DoUpdate(timer);
	_mesh->Update();
}

void Entity::InitializeMesh()
{
	if (!_mesh)
		throw std::runtime_error("The entity doesn't have any mesh attached to be initialized.");
	_mesh->CreateDeviceDependentResources();
}

void Entity::ReleaseMesh()
{
	if (!_mesh)
		throw std::runtime_error("The entity doesn't have any mesh attached to be released.");
	_mesh->ReleaseDeviceDependentResources();
}

void Entity::Render()
{
	if (!_mesh)
		throw std::runtime_error("The entity doesn't have any mesh attached to be render.");
	_mesh->Render();
}

void HoloLensClient::Entity::kill()
{
	_alive = false;
}

Windows::Foundation::Numerics::float3 HoloLensClient::Entity::getPosition() const
{
	if (!_mesh)
		throw std::runtime_error("Mesh is not defined");
	return (_mesh->GetPosition());
}

bool HoloLensClient::Entity::isDead() const
{
	return (!_alive);
}

void HoloLensClient::Entity::SetPosition(Windows::Foundation::Numerics::float3 position)
{
	if (!_mesh)
		throw std::runtime_error("Mesh is not defined");
	_mesh->SetPosition(position);
}

void HoloLensClient::Entity::SetRotation(Windows::Foundation::Numerics::float3 rotation)
{
	if (!_mesh)
		throw std::runtime_error("Mesh is not defined");
	_mesh->SetRotation(rotation);
}

void HoloLensClient::Entity::SetPosition(DirectX::XMMATRIX positionMatrix)
{
	if (!_mesh)
		throw std::runtime_error("Mesh is not defined");
	_mesh->SetPosition(positionMatrix);
}

void HoloLensClient::Entity::SetRotation(DirectX::XMMATRIX rotationMatrix)
{
	if (!_mesh)
		throw std::runtime_error("Mesh is not defined");
	_mesh->SetRotation(rotationMatrix);
}

std::unique_ptr<IObject> const &Entity::getMesh() const
{
	return (_mesh);
}

void Entity::setMesh(std::unique_ptr<IObject> mesh)
{
	_mesh = std::move(mesh);
}

std::unique_ptr<IObject>& Entity::getMesh()
{
	return (_mesh);
}

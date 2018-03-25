#include "pch.h"
#include "3D\Entities\Common\Entity.h"

using namespace HoloLensClient;

Entity::Entity(std::shared_ptr<HolographicScene> &scene) 
	: _parent(nullptr), _alive(true), _scene(scene)
{
}

HoloLensClient::Entity::~Entity()
{
}

void HoloLensClient::Entity::Update(DX::StepTimer const & timer)
{
	DoUpdate(timer);
	std::for_each(_meshs.begin(), _meshs.end(),
		[](auto &mesh)
	{
		mesh->Update();
	});
}

void Entity::InitializeMesh()
{
	std::for_each(_meshs.begin(), _meshs.end(),
		[](auto &mesh)
	{
		mesh->CreateDeviceDependentResources();
	});
}

void Entity::ReleaseMesh()
{
	std::for_each(_meshs.begin(), _meshs.end(),
		[](auto &mesh)
	{
		mesh->ReleaseDeviceDependentResources();
	});
}

void Entity::Render()
{
	std::for_each(_meshs.begin(), _meshs.end(),
		[](auto &mesh)
	{
		mesh->Render();
	});
}

void HoloLensClient::Entity::kill()
{
	std::for_each(_childs.begin(), _childs.end(),
		[](auto &child)
	{
		//Kill all the childs
		//No need to remove the parent from them since all entities are deleted
		//After the call to the update function
		//So there should be no memory race
		child->kill();
	});
	_alive = false;
}

bool HoloLensClient::Entity::isDead() const
{
	return (!_alive);
}

void HoloLensClient::Entity::Move(Windows::Foundation::Numerics::float3 offset)
{
	_position += offset;
}

void HoloLensClient::Entity::SetPosition(Windows::Foundation::Numerics::float3 position)
{
	_position = position;
}

void HoloLensClient::Entity::SetRotation(Windows::Foundation::Numerics::float3 rotation)
{
	_rotation = rotation;
}

void HoloLensClient::Entity::SetPosition(DirectX::XMMATRIX positionMatrix)
{
}

void HoloLensClient::Entity::SetRotation(DirectX::XMMATRIX rotationMatrix)
{
}

void HoloLensClient::Entity::GetPosition(Windows::Foundation::Numerics::float3 & position)
{
	position = _position;
}

void HoloLensClient::Entity::GetRotation(Windows::Foundation::Numerics::float3 & rotation)
{
	rotation = _rotation;
}

void HoloLensClient::Entity::GetPosition(DirectX::XMMATRIX & positionMatrix)
{
}

void HoloLensClient::Entity::GetRotation(DirectX::XMMATRIX & rotationMatrix)
{
}

void HoloLensClient::Entity::SetParent(IEntity *parent)
{
	//Remove this entity from the childs entity of the previous parent
	_parent->RemoveChild(this);
	//Set the new parent
	_parent = parent;
}

void HoloLensClient::Entity::AddChild(IEntity *child)
{
	auto found = std::find(_childs.begin(), _childs.end(), child);

	if (found != _childs.end())
		std::runtime_error("This entity is already a child of this entity");
	_childs.push_back(child);
	child->SetParent(this);
}

void HoloLensClient::Entity::RemoveChild(IEntity *child)
{
	_childs.erase(
		std::remove_if(_childs.begin(), _childs.end(),
			[&child](auto &c) {
				return c == child;
			}),
		_childs.end());
}

IEntity *HoloLensClient::Entity::getParent() const
{
	return _parent;
}

void Entity::addMesh(IObject::IObjectPtr mesh)
{
	_meshs.push_back(std::move(mesh));
}

//std::unique_ptr<IObject> const &Entity::getMesh() const
//{
//	return (_mesh);
//}

//std::unique_ptr<IObject>& Entity::getMesh()
//{
//	return (_mesh);
//}

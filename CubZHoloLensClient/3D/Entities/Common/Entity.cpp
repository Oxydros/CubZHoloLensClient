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
	for (auto it = _childs.begin(); it != _childs.end(); ++it)
	{
		//(*it)->DoUpdate(timer);
	}
	std::for_each(_childs.begin(), _childs.end(),
		[&timer](auto &child)
	{
		child->DoUpdate(timer);
	});
	DoUpdate(timer);
	//std::for_each(_meshs.begin(), _meshs.end(),
	//	[](auto &mesh)
	//{
	//	mesh->Update();
	//});
	if (!_useTranslationMatrix)
		_mesh->SetPosition(_realPosition);
	if (!_useRotationMatrix)
		_mesh->SetRotation(_realRotation);
	_useTranslationMatrix = _useRotationMatrix = false;
	_mesh->Update();
}

void HoloLensClient::Entity::Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState ^ pointerState)
{
	std::for_each(_childs.begin(), _childs.end(),
		[&pointerState](auto &child)
	{
		child->OnInputs(pointerState);
	});
	OnInputs(pointerState);
}

void Entity::InitializeMesh()
{
	//std::for_each(_meshs.begin(), _meshs.end(),
	//	[](auto &mesh)
	//{
	//	mesh->CreateDeviceDependentResources();
	//});
	_mesh->CreateDeviceDependentResources();
}

void Entity::ReleaseMesh()
{
	/*std::for_each(_meshs.begin(), _meshs.end(),
		[](auto &mesh)
	{
		mesh->ReleaseDeviceDependentResources();
	});*/
	_mesh->ReleaseDeviceDependentResources();
}

void Entity::Render()
{
	/*std::for_each(_meshs.begin(), _meshs.end(),
		[](auto &mesh)
	{
		mesh->Render();
	});*/
	_mesh->Render();
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
	_relativePosition += offset;
	UpdateReal();
}

void HoloLensClient::Entity::SetRelativePosition(Windows::Foundation::Numerics::float3 position)
{
	_relativePosition = position;
	UpdateReal();
}

void HoloLensClient::Entity::SetRelativeRotation(Windows::Foundation::Numerics::float3 rotation)
{
	_relativeRotation = rotation;
	UpdateReal();
}

void HoloLensClient::Entity::SetRealPosition(Windows::Foundation::Numerics::float3 position)
{
	_realPosition = position;
	UpdateRelative();
}

void HoloLensClient::Entity::SetRealRotation(Windows::Foundation::Numerics::float3 rotation)
{
	_realRotation = rotation;
	UpdateRelative();
}

void HoloLensClient::Entity::SetPosition(DirectX::XMMATRIX &positionMatrix)
{
	_useTranslationMatrix = true;
	_mesh->SetPosition(positionMatrix);
}

void HoloLensClient::Entity::SetRotation(DirectX::XMMATRIX &rotationMatrix)
{
	_useRotationMatrix = true;
	_mesh->SetRotation(rotationMatrix);
}

inline void HoloLensClient::Entity::UpdateReal()
{
	if (_parent != nullptr)
	{
		_realPosition = _parent->GetPosition() + _relativePosition;
		_realRotation = _parent->GetRotation() + _relativeRotation;
	}
	else
	{
		_realPosition = _relativePosition;
		_realRotation = _relativeRotation;
	}
}

inline void HoloLensClient::Entity::UpdateRelative()
{
	if (_parent != nullptr)
	{
		_relativePosition = _realPosition - _parent->GetPosition();
		_relativeRotation = _realRotation - _parent->GetRotation();
	}
	else
	{
		_relativePosition = _realPosition;
		_relativeRotation = _realRotation;
	}
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
	_mesh = std::move(mesh);
	_mesh->CreateDeviceDependentResources();
	/*_meshs.push_back(std::move(mesh));*/
}

//std::unique_ptr<IObject> const &Entity::getMesh() const
//{
//	return (_mesh);
//}

//std::unique_ptr<IObject>& Entity::getMesh()
//{
//	return (_mesh);
//}

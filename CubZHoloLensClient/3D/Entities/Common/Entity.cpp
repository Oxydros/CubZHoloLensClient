#include "pch.h"
#include "3D\Entities\Common\Entity.h"
#include <3D\Scene\HolographicScene.h>

using namespace HoloLensClient;
using namespace DirectX;
using namespace Windows::Foundation::Numerics;

Entity::Entity(std::shared_ptr<HolographicScene> &scene) 
	: _parent(nullptr), _alive(true), _scene(scene)
{
	SetRealRotation({ 0, 0, 0 });
	SetRealPosition({ 0, 0, 0 });
}

HoloLensClient::Entity::~Entity()
{
}

void HoloLensClient::Entity::Update(DX::StepTimer const & timer)
{
	std::for_each(_childs.begin(), _childs.end(),
		[&timer](auto &child)
	{
		child->DoUpdate(timer);
	});

	//Update position and orient if needed
	if (_followGazePosition)
		positionInFrontOfGaze(_positionOffsetFromGaze);
	if (_followGazeRotation)
		rotateTowardGaze(_rotationOffsetFromGaze);

	DoUpdate(timer);

	if (_parent != nullptr) _mesh->SetModelTransform(_parent->GetTransformMatrix() * GetTransformMatrix());
	else _mesh->SetModelTransform(GetTransformMatrix());

	/*TRACE("For " << this << " Real position is (" << _realPosition.x << ", " << _realPosition.y << ", " << _realPosition.z 
		  << ") Relative is (" << _relativePosition.x << ", " << _relativePosition.y << ", " << _relativePosition.z << ")"
			<< " Bools matrix: " << _useTranslationMatrix << " " << _useRotationMatrix << std::endl);*/
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
	_mesh->CreateDeviceDependentResources();
}

void Entity::ReleaseMesh()
{
	_mesh->ReleaseDeviceDependentResources();
}

void Entity::Render()
{
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
	_modelTranslation = XMMatrixTranslationFromVector(XMLoadFloat3(&_relativePosition));
}

void HoloLensClient::Entity::SetRelativePosition(Windows::Foundation::Numerics::float3 position)
{
	_relativePosition = position;
	_modelTranslation = XMMatrixTranslationFromVector(XMLoadFloat3(&_relativePosition));
}

void HoloLensClient::Entity::SetRelativeRotation(Windows::Foundation::Numerics::float3 rotation)
{
	_relativeRotation = rotation;
	_modelRotation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&_relativeRotation));
}

void HoloLensClient::Entity::SetRealPosition(Windows::Foundation::Numerics::float3 position)
{
	if (_parent != nullptr) throw std::runtime_error("Can't update real position because it is a Child entity");
	_realPosition = position;
	_relativePosition = _realPosition;
	_modelTranslation = XMMatrixTranslationFromVector(XMLoadFloat3(&_relativePosition));
	/*UpdateRelative();*/
}

void HoloLensClient::Entity::SetRealRotation(Windows::Foundation::Numerics::float3 rotation)
{
	if (_parent != nullptr) throw std::runtime_error("Can't update real position because it is a Child entity");
	_realRotation = rotation;
	_relativeRotation = _realRotation;
	_modelRotation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&_relativeRotation));
	/*UpdateRelative();*/
}

void HoloLensClient::Entity::SetRealPosition(DirectX::XMMATRIX &positionMatrix)
{
	if (_parent != nullptr) throw std::runtime_error("Can't update real position because it is a Child entity");
	//_useTranslationMatrix = true;
	_modelTranslation = positionMatrix;
}

void HoloLensClient::Entity::SetRealRotation(DirectX::XMMATRIX &rotationMatrix)
{
	if (_parent != nullptr) throw std::runtime_error("Can't update real position because it is a Child entity");
	//_useRotationMatrix = true;
	_modelRotation = rotationMatrix;
}

//inline void HoloLensClient::Entity::UpdateReal()
//{
//	if (_parent != nullptr)
//	{
//		_realPosition = _parent->GetPosition() + _relativePosition;
//		_realRotation = _parent->GetRotation() + _relativeRotation;
//	}
//	else
//	{
//		_realPosition = _relativePosition;
//		_realRotation = _relativeRotation;
//	}
//	_modelTranslation = XMMatrixTranslationFromVector(XMLoadFloat3(&_realPosition));
//	_modelRotation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&_realRotation));
//}

//inline void HoloLensClient::Entity::UpdateRelative()
//{
//	//if (_parent != nullptr)
//	//{
//	//	_relativePosition = _realPosition - _parent->GetPosition();
//	//	_relativeRotation = _realRotation - _parent->GetRotation();
//	//}
//	//else
//	//{
//	_relativePosition = _realPosition;
//	_relativeRotation = _realRotation;
//	//}
//	_modelTranslation = XMMatrixTranslationFromVector(XMLoadFloat3(&_relativePosition));
//	_modelRotation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&_relativeRotation));
//}

void HoloLensClient::Entity::SetParent(IEntity *parent)
{
	//Remove this entity from the childs entity of the previous parent
	if (_parent != nullptr) _parent->RemoveChild(this);
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
}

void HoloLensClient::Entity::positionInFrontOfGaze(Windows::Foundation::Numerics::float3 offsets)
{
	auto pointerPose = _scene->getPointerPose();

	if (pointerPose != nullptr)
	{
		// Get the gaze direction relative to the given coordinate system.
		const Windows::Foundation::Numerics::float3 headPosition = pointerPose->Head->Position;
		const Windows::Foundation::Numerics::float3 headDirection = pointerPose->Head->ForwardDirection;

		// The tag-along hologram follows a point 2.0m in front of the user's gaze direction.
		const Windows::Foundation::Numerics::float3 gazeAtTwoMeters = headPosition + (offsets.z * headDirection);

		SetRealPosition(gazeAtTwoMeters);
	}
}

void HoloLensClient::Entity::rotateTowardGaze(Windows::Foundation::Numerics::float3 offsets)
{
	auto pointerPose = _scene->getPointerPose();

	if (pointerPose != nullptr)
	{
		// Get the gaze direction relative to the given coordinate system.
		const float3 headPosition = pointerPose->Head->Position;
		const float3 headDirection = pointerPose->Head->ForwardDirection;

		const float3 gazeAtTwoMeters = headPosition + (1.0f * headDirection);

		// Lerp the position, to keep the hologram comfortably stable.
		//auto lerpedPosition = lerp(getPosition(), gazeAtTwoMeters, dtime * c_lerpRate);

		// Create a direction normal from the hologram's position to the origin of person space.
		// This is the z-axis rotation.
		XMVECTOR facingNormal = XMVector3Normalize(-XMLoadFloat3(&gazeAtTwoMeters));

		// Rotate the x-axis around the y-axis.
		// This is a 90-degree angle from the normal, in the xz-plane.
		// This is the x-axis rotation.
		XMVECTOR xAxisRotation = XMVector3Normalize(XMVectorSet(XMVectorGetZ(facingNormal), 0.f, -XMVectorGetX(facingNormal), 0.f));

		// Create a third normal to satisfy the conditions of a rotation matrix.
		// The cross product  of the other two normals is at a 90-degree angle to
		// both normals. (Normalize the cross product to avoid floating-point math
		// errors.)
		// Note how the cross product will never be a zero-matrix because the two normals
		// are always at a 90-degree angle from one another.
		XMVECTOR yAxisRotation = XMVector3Normalize(XMVector3Cross(facingNormal, xAxisRotation));

		// Construct the 4x4 rotation matrix.

		// Rotate the quad to face the user.
		auto rotation = XMMATRIX(
			xAxisRotation,
			yAxisRotation,
			facingNormal,
			XMVectorSet(0.f, 0.f, 0.f, 1.f)
		);
		SetRealRotation(rotation);
	}
}
#include "pch.h"
#include "3D\Utility\Collision.h"
#include "3D\Entities\Common\Entity.h"
#include <3D\Scene\HolographicScene.h>

using namespace HoloLensClient;
using namespace DirectX;
using namespace Windows::Foundation::Numerics;

Entity::Entity(std::shared_ptr<HolographicScene> scene) 
	: _parent(nullptr), _alive(true), _scene(scene)
{
	SetRealRotation({ 0, 0, 0 });
	SetRealPosition({ 0, 0, 0 });
	_modelScaling = XMMatrixIdentity();
}

HoloLensClient::Entity::~Entity()
{
	// Delete all childs
	_childs.clear();

	if (_mesh)
		_mesh->ReleaseDeviceDependentResources();
}

DirectX::XMMATRIX const Entity::GetTransformMatrix() const
{
	DirectX::XMMATRIX localTransform = _modelScaling * _modelRotation * _modelTranslation;
	DirectX::XMMATRIX finalTransform = _parent != nullptr ? localTransform * _parent->GetTransformMatrix() : localTransform;

	//TRACE("For " << GetLabel() << " Rotation " << _realRotation << " " <<  _modelRotation << std::endl);
	//TRACE("For " << GetLabel() << " Translation " << _realPosition << " " << _modelTranslation << std::endl);
	//TRACE("For " << GetLabel() << " Local " << localTransform << std::endl);
	//TRACE("For " << GetLabel() << " Final " << finalTransform << std::endl);
	return (finalTransform);

}

void HoloLensClient::Entity::Update(DX::StepTimer const & timer)
{
	//Add new entities created at the previous call to update
	std::for_each(_newChilds.begin(), _newChilds.end(),
		[this](auto &child)
	{
		TRACE("Adding new child from pending list " << child->GetLabel() << " " << child.get() << std::endl);
		_childs.emplace_back(std::move(child));
	});
	_newChilds.clear();

	if (_scene->getPointerPose() != nullptr) {
		float3 positionMotion = _scene->getPointerPose()->Head->Position - _previousGazePosition;
		float3 directionMotion = _scene->getPointerPose()->Head->ForwardDirection - _previousGazeDirection;
		GazeMotion(positionMotion, directionMotion);
		_previousGazePosition = _scene->getPointerPose()->Head->Position;
		_previousGazeDirection = _scene->getPointerPose()->Head->ForwardDirection;
	}

	//Update position and orient if needed
	if (_followGazeRotation)
		rotateTowardGaze(_rotationOffsetFromGaze);
	if (_followGazePosition)
		positionInFrontOfGaze(_positionOffsetFromGaze);

	//Update childs
	std::for_each(_childs.begin(), _childs.end(),
		[&timer](auto &child)
	{
		/*TRACE("Updating child " << child->GetLabel() << std::endl);*/
		child->Update(timer);
	});

	updateInGaze();
	DoUpdate(timer);

	if(_mesh)
		_mesh->SetModelTransform(GetTransformMatrix());

	// If child is delete, remove the unique_ptr from the child list
	// This will delete its instance
	_childs.erase(
		std::remove_if(_childs.begin(), _childs.end(),
			[](auto &child) {
				if (child->isDead())
					TRACE("Killing child " << child->GetLabel() << std::endl);
				return child->isDead();
			}),
		_childs.end()
	);

	/*TRACE("For " << this << " Real position is (" << _realPosition.x << ", " << _realPosition.y << ", " << _realPosition.z 
		  << ") Relative is (" << _relativePosition.x << ", " << _relativePosition.y << ", " << _relativePosition.z << ")"
			<< " Bools matrix: " << _useTranslationMatrix << " " << _useRotationMatrix << std::endl);*/
}

void Entity::InitializeMesh()
{
	std::for_each(_childs.begin(), _childs.end(),
		[](auto &child)
	{
		TRACE("Init mesh of " << child->GetLabel() << std::endl);
		child->InitializeMesh();
	});

	if (_mesh)
		_mesh->CreateDeviceDependentResources();
}

void Entity::ReleaseMesh()
{
	std::for_each(_childs.begin(), _childs.end(),
		[](auto &child)
	{
		TRACE("Release mesh of " << child->GetLabel() << std::endl);
		child->ReleaseMesh();
	});

	if (_mesh)
		_mesh->ReleaseDeviceDependentResources();
}

void Entity::Render()
{
	std::for_each(_childs.begin(), _childs.end(),
		[](auto &child)
	{
		/*TRACE("Render mesh of " << child->GetLabel() << std::endl);*/
		child->Render();
	});

	// Don't render if entity is not supposed to be visible
	if (_visible && _mesh)
		_mesh->Render();
}

void HoloLensClient::Entity::kill()
{
	_alive = false;
	//std::for_each(_childs.begin(), _childs.end(),
	//	[](auto &child)
	//{
	//	//Kill all the childs
	//	//No need to remove the parent from them since all entities are deleted
	//	//After the call to the update function
	//	//So there should be no memory race
	//	child->kill();
	//});
	//if (_parent != nullptr) _parent->RemoveChild(this);
}

bool HoloLensClient::Entity::isDead() const
{
	return (!_alive);
}

void HoloLensClient::Entity::setVisible(bool visibility)
{
	_visible = visibility;
	std::for_each(_childs.begin(), _childs.end(),
		[&visibility](auto &child)
	{
		child->setVisible(visibility);
	});
}

void HoloLensClient::Entity::Move(Windows::Foundation::Numerics::float3 offset)
{
	_relativePosition += offset;
	_modelTranslation = XMMatrixTranslationFromVector(XMLoadFloat3(&_relativePosition));
}

void HoloLensClient::Entity::Rotate(Windows::Foundation::Numerics::float3 offset)
{
	_relativeRotation += offset;
	_modelRotation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&_relativeRotation));
}

void HoloLensClient::Entity::Scale(Windows::Foundation::Numerics::float3 offset)
{
	_scaling += offset;
	_modelScaling = XMMatrixScalingFromVector(XMLoadFloat3(&_scaling));
}

void HoloLensClient::Entity::SetScale(Windows::Foundation::Numerics::float3 scale)
{
	_scaling = scale;
	_modelScaling = XMMatrixScalingFromVector(XMLoadFloat3(&_scaling));
}

Windows::Foundation::Numerics::float3 HoloLensClient::Entity::GetSize() const
{
	return (Windows::Foundation::Numerics::float3(
		_originalSize.x * _scaling.x,
		_originalSize.y * _scaling.y,
		_originalSize.z * _scaling.z));
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
	if (_parent == nullptr)
		return SetRelativePosition(position);
	_relativePosition = position - _parent->GetRealPosition();
	_modelTranslation = XMMatrixTranslationFromVector(XMLoadFloat3(&_relativePosition));
}

void HoloLensClient::Entity::SetRealRotation(Windows::Foundation::Numerics::float3 rotation)
{
	if (_parent == nullptr)
		return SetRelativeRotation(rotation);
	_relativeRotation = rotation - _parent->GetRealRotation();
	_modelRotation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&_relativeRotation));
}

void HoloLensClient::Entity::SetModelPosition(DirectX::XMMATRIX &positionMatrix)
{
	/*TRACE("WARNING: Use of setRealPosition Matrix for "  << std::endl);*/
	/*if (_parent != nullptr && !_parent->isRoot()) throw std::runtime_error("Can't update real position because it is a Child entity");*/
	_modelTranslation = positionMatrix;
}

void HoloLensClient::Entity::SetModelRotation(DirectX::XMMATRIX &rotationMatrix)
{
	/*TRACE("WARNING: Use of setRealRotation Matrix for "  << std::endl);*/
	/*if (_parent != nullptr && !_parent->isRoot()) throw std::runtime_error("Can't update real position because it is a Child entity");*/
	_modelRotation = rotationMatrix;
}

Windows::Foundation::Numerics::float3 const HoloLensClient::Entity::GetRealPosition() const
{
	float3 parentReal = { 0, 0, 0 };

	//Real position is the sum of all relative positions starting from the root node
	if (_parent)
		parentReal = _parent->GetRealPosition();
	return (parentReal + _relativePosition);
}

Windows::Foundation::Numerics::float3 const HoloLensClient::Entity::GetRealRotation() const
{
	float3 parentReal = { 0, 0, 0 };

	//Real rotation is the sum of all relative rotations starting from the root node
	if (_parent)
		parentReal = _parent->GetRealRotation();
	return (parentReal + _relativeRotation);
}

void HoloLensClient::Entity::SetParent(IEntity *parent)
{
	//Remove this entity from the childs entity of the previous parent
	if (_parent != nullptr) _parent->RemoveChild(this);
	//Set the new parent
	_parent = parent;
}

void HoloLensClient::Entity::AddChild(IEntity::IEntityPtr child)
{
	//auto found = std::find(_childs.begin(), _childs.end(), child);

	//if (found != _childs.end())
	//	std::runtime_error("This entity is already a child of this entity");
	child->SetParent(this);
	//Child has same visibility as parent
	child->setVisible(_visible);
	_newChilds.push_back(std::move(child));
}

void HoloLensClient::Entity::RemoveChild(IEntity *child)
{
	_childs.erase(
		std::remove_if(_childs.begin(), _childs.end(),
			[&child](auto &c) {
				return c.get() == child;
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

void HoloLensClient::Entity::updateInGaze()
{
	//Don't calcul gaze if no mesh in entity, or not visible,
	// or if entity is the cursor
	if (_mesh == nullptr || !_visible || this == _scene->getCursor() || IgnoreInGaze())
	{
		_inGaze = false;
		_distance = -1;
		return;
	}

	auto pointerPose = _scene->getPointerPose();

	if (pointerPose != nullptr)
	{
		// Get the gaze direction relative to the given coordinate system.
		const float3 position = pointerPose->Head->Position;
		const float3 direction = pointerPose->Head->ForwardDirection;

		XMFLOAT3 headPosition = XMFLOAT3(position.x, position.y, position.z);
		XMFLOAT3 headDirection = XMFLOAT3(direction.x, direction.y, direction.z);
		float distance = 0.0f;

		DirectX::BoundingOrientedBox currentBoundingBox;
		_mesh->GetBoundingBox(currentBoundingBox);

		float3 extents{ currentBoundingBox.Extents.x, currentBoundingBox.Extents.y, currentBoundingBox.Extents.z };
		float3 B1 = (GetRealPosition() - extents);
		float3 B2 = (GetRealPosition() + extents);

		float3 Hit;
		float3 L1{ headPosition.x, headPosition.y, headPosition.z };
		float3 L2{ headDirection.x, headDirection.y, headDirection.z };

		bool check = CheckLineBox(B1, B2, L1, (L1 + (L2 * 6.0f)), Hit, 0.1f);
		
		XMVECTOR originVec = DirectX::XMLoadFloat3(&(headPosition));
		XMVECTOR hitVec = DirectX::XMLoadFloat3(&XMFLOAT3(Hit.x, Hit.y, Hit.z));
		XMVECTOR distanceV = XMVector3Length(XMVectorSubtract(originVec, hitVec));
		DirectX::XMStoreFloat(&_distance, distanceV);
		
		/*TRACE("In Gaze " << GetLabel() << " " << _inGaze << " " << _distance << std::endl);*/
		_inGaze = currentBoundingBox.Intersects(DirectX::XMLoadFloat3(&headPosition), DirectX::XMLoadFloat3(&headDirection), distance);

		//if (_inGaze)
		//{
		//	TRACE(this->GetLabel() << " in gaze " << _inGaze << " dist " << _distance << " hit " << Hit << " " << check << std::endl);
		//	TRACE("User position " << L1 << " direction " << L2 << std::endl);
		//	TRACE("Box extend is " << extents << std::endl);
		//	TRACE("Real position is " << GetRealPosition() << std::endl);
		//}
		
		//check is false when its colliding sometime because the extents dont reflex the orientation of the box
		/*if (check != _inGaze)
			_inGaze = false;*/
	}
	else
	{
		_inGaze = false;
		_distance = -1;
	}
}

std::pair<IEntity*, float> HoloLensClient::Entity::getNearestInGazeEntity()
{
	auto pair = std::make_pair(static_cast<IEntity*>(this), _distance);

	std::for_each(_childs.begin(), _childs.end(),
		[&pair](auto &child)
	{
		auto newDistance = child->getNearestInGazeEntity();
		if (newDistance.first->isInGaze() && (pair.second < 0 || newDistance.second < pair.second))
		{
			pair = newDistance;
		}
	});

	return (pair);
}

void HoloLensClient::Entity::CaptureInteraction(Windows::UI::Input::Spatial::SpatialInteraction ^interaction)
{
	if (_spatialGestureRecognizer)
		_spatialGestureRecognizer->CaptureInteraction(interaction);
}

void HoloLensClient::Entity::SetSpatialGestureRecognizer(Windows::UI::Input::Spatial::SpatialGestureRecognizer ^ recognizer)
{
	_spatialGestureRecognizer = recognizer;
}

void HoloLensClient::Entity::positionInFrontOfGaze(Windows::Foundation::Numerics::float3 offsets)
{
	auto pointerPose = _scene->getPointerPose();

	if (pointerPose != nullptr)
	{
		// Get the gaze direction relative to the given coordinate system.
		Windows::Foundation::Numerics::float3 headPosition = pointerPose->Head->Position;

		// Add offset x, y
		headPosition.x += offsets.x;
		headPosition.y += offsets.y;

		const Windows::Foundation::Numerics::float3 headDirection = pointerPose->Head->ForwardDirection;

		// The tag-along hologram follows a point 2.0m in front of the user's gaze direction.
		const Windows::Foundation::Numerics::float3 gazeAtTwoMeters = headPosition + (offsets.z * headDirection);

		//TRACE("For " << this << " translation of (" << gazeAtTwoMeters.x << ", " << gazeAtTwoMeters.y << ", " << gazeAtTwoMeters.z
		//		<< ") "<< std::endl);

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
		const float3 realPos = GetRealPosition();
		const float3 headDirection = realPos - headPosition;
		/*const float3 headDirection = pointerPose->Head->ForwardDirection;*/

		/*TRACE(GetLabel() << " GAZE ROT POS " << headPosition << " DIR " << headDirection << " " << pointerPose->Head->ForwardDirection << std::endl);

		const float3 gazeAtTwoMeters = headPosition + (1.0f * headDirection);*/

		// Lerp the position, to keep the hologram comfortably stable.
		//auto lerpedPosition = lerp(getPosition(), gazeAtTwoMeters, dtime * c_lerpRate);

		// Create a direction normal from the hologram's position to the origin of person space.
		// This is the z-axis rotation.
		XMVECTOR facingNormal = XMVector3Normalize(-XMLoadFloat3(&headDirection));

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

		float3 x, y, z;
		XMStoreFloat3(&x, xAxisRotation);
		XMStoreFloat3(&y, yAxisRotation);
		XMStoreFloat3(&z, facingNormal);

		// Construct the 4x4 rotation matrix.
		//TRACE("For " << this << " rotation of "
		//	<< "("<< x.x << ", " << x.y << ", " << x.z << ") "
		//	<< "(" << y.x << ", " << y.y << ", " << y.z << ") "
		//	<< "(" << z.x << ", " << z.y << ", " << z.z << ") "
		//	<< std::endl);


		// Rotate the quad to face the user.
		auto rotation = XMMATRIX(
			xAxisRotation,
			yAxisRotation,
			facingNormal,
			XMVectorSet(0.f, 0.f, 0.f, 1.f)
		);
		SetModelRotation(rotation);
	}
}

void HoloLensClient::Entity::setFocus(bool newFocus)
{
	auto oldFocus = _focused;

	_focused = newFocus;
	if (_focused != oldFocus)
	{
		Concurrency::task<void> callbackTask = Concurrency::create_task([this]()
		{
			if (_focused) {
				TRACE("Got Focus on " << GetLabel() << std::endl);
				OnGetFocus();
			}
			else {
				TRACE("Lost Focus on " << GetLabel() << std::endl);
				OnLostFocus();
			}
		});
	}
}

void HoloLensClient::Entity::getInGazeEntities(std::vector<IEntity*>& entities)
{
	std::for_each(_childs.begin(), _childs.end(),
		[&entities](auto &child)
	{
		child->getInGazeEntities(entities);
	});
	if (isInGaze())
		entities.push_back(this);
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


// UTILITIES DISPLAY FUNCTIONS

std::ostream& operator<<(std::ostream& stream, const DirectX::XMMATRIX& matrix) {
	DirectX::XMFLOAT4X4 fView;
	DirectX::XMStoreFloat4x4(&fView, matrix);
	for (int i = 0; i < 4; i++)
	{
		stream << "[";
		for (int j = 0; j < 4; j++)
		{
			stream << " " << fView.m[i][j];
		}
		stream << "]";
	}
	return (stream);
}

std::ostream& operator<<(std::ostream& stream, const Windows::Foundation::Numerics::float3 a)
{
	stream << "(" << a.x << ", " << a.y << ", " << a.z << ")";
	return (stream);
}

// -- END OF UTILITIES FUNCTIONS
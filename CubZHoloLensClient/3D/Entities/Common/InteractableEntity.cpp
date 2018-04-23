#include <pch.h>
#include <3D\Scene\HolographicScene.h>
#include <3D\Entities\Common\InteractableEntity.h>

using namespace HoloLensClient;
using namespace DirectX;
using namespace Windows::Foundation::Numerics;

InteractableEntity::InteractableEntity(std::shared_ptr<HolographicScene> scene, bool disableMultipleClick)
	: Entity(scene), _disableMultipleClick(disableMultipleClick)
{
}

void InteractableEntity::DoUpdate(DX::StepTimer const &timer)
{
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
		GetBiggestBoundingBox(currentBoundingBox);

		setFocus(currentBoundingBox.Intersects(XMLoadFloat3(&headPosition), XMLoadFloat3(&headDirection), distance));
	}
	else
		setFocus(false);
}

void HoloLensClient::InteractableEntity::OnInputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState ^pointerState)
{
	/*TRACE("Process onInputs on " << GetLabel() << std::endl);*/
	if (pointerState != nullptr && pointerState->IsPressed && _focused)
	{
		// Prevent multiple click
		// Check if it was already clicked and cancel if it was the case
		if (_disableMultipleClick && _clicked)
			return;
		_clicked = true;
		Concurrency::task<void> callbackTask = Concurrency::create_task([this]()
		{
			OnAirTap();
		});
	}
	else
		_clicked = false;
}

void HoloLensClient::InteractableEntity::GetBiggestBoundingBox(DirectX::BoundingOrientedBox &boundingBox)
{
	//TODO
	//std::for_each(_meshs.begin(), _meshs.end(),
	//	[&boundingBox](auto &mesh)
	//{
	//	mesh->GetBoundingBox(boundingBox);
	//});
	if (_mesh)
		_mesh->GetBoundingBox(boundingBox);
}

void HoloLensClient::InteractableEntity::setFocus(bool newFocus)
{
	auto oldFocus = _focused;

	_focused = newFocus;
	if (_focused != oldFocus)
	{
		Concurrency::task<void> callbackTask = Concurrency::create_task([this]()
		{
			if (_focused) OnGetFocus();
			else OnLostFocus();
		});
	}
}

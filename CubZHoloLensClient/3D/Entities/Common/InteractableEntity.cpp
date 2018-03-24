#include <pch.h>
#include <3D\Scene\HolographicScene.h>
#include <3D\Entities\Common\InteractableEntity.h>

using namespace HoloLensClient;
using namespace DirectX;
using namespace Windows::Foundation::Numerics;

InteractableEntity::InteractableEntity(std::shared_ptr<HolographicScene>& scene) 
	: Entity(scene)
{
}

void InteractableEntity::Update(DX::StepTimer const &timer)
{
	Entity::Update(timer);
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

		_focused = currentBoundingBox.Intersects(XMLoadFloat3(&headPosition), XMLoadFloat3(&headDirection), distance);
	}
	else
		_focused = false;
}

void HoloLensClient::InteractableEntity::Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState ^pointerState)
{
	if (pointerState->IsPressed && _focused)
	{
		Concurrency::task<void> callbackTask = Concurrency::create_task([this]()
		{
			OnAirTap();
		});
	}
}

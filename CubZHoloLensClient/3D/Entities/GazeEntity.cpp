#include "pch.h"

#include "3D\Entities\GazeEntity.h"
#include "3D\Entities\CubeEntity.h"
#include "3D\Utility\DirectXHelper.h"
#include "3D\Scene\HolographicScene.h"

using namespace HoloLensClient;
using namespace DirectX;
using namespace Windows::Foundation::Numerics;

GazeEntity::GazeEntity(std::shared_ptr<DX::DeviceResources> &devicesResources, std::shared_ptr<HolographicScene> &scene)
	: Entity(scene)
{
	setMesh(std::make_unique<GazeRenderer>(devicesResources));
	InitializeMesh();
}

GazeEntity::~GazeEntity()
{
}

void GazeEntity::DoUpdate(DX::StepTimer const & timer)
{
	DirectX::XMMATRIX translation;
	DirectX::XMMATRIX rotation;

	getFaceRotationTranslationModel(translation, rotation, 2.0f, _scene->getPointerPose());

	getMesh()->SetPosition(translation);
	getMesh()->SetRotation(rotation);
}

void GazeEntity::Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState ^pointerState)
{
	auto pointerPose = pointerState->TryGetPointerPose(_scene->getCoordinateSystem());

	if (pointerPose != nullptr)
	{
		auto cube = std::make_unique<CubeEntity>(getMesh()->getDeviceResources(), _scene);

		// Get the gaze direction relative to the given coordinate system.
		const float3 headPosition = pointerPose->Head->Position;
		const float3 headDirection = pointerPose->Head->ForwardDirection;

		// The tag-along hologram follows a point 2.0m in front of the user's gaze direction.
		const float3 gazeAtTwoMeters = headPosition + (2.0f * headDirection);
		
		cube->SetPosition(gazeAtTwoMeters);
		cube->SetRotation({ 0, 0, 0 });
		_scene->addEntity(std::move(cube));
	}
}

void HoloLensClient::GazeEntity::getFaceRotationTranslationModel(DirectX::XMMATRIX &translation, DirectX::XMMATRIX &rotation, float distance,
									Windows::UI::Input::Spatial::SpatialPointerPose ^pointerPose)
{
	if (pointerPose != nullptr)
	{
		// Get the gaze direction relative to the given coordinate system.
		const float3 headPosition = pointerPose->Head->Position;
		const float3 headDirection = pointerPose->Head->ForwardDirection;

		// The tag-along hologram follows a point 2.0m in front of the user's gaze direction.
		const float3 gazeAtTwoMeters = headPosition + (distance * headDirection);

		translation = XMMatrixTranslationFromVector(XMLoadFloat3(&gazeAtTwoMeters));

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
		rotation = XMMATRIX(
			xAxisRotation,
			yAxisRotation,
			facingNormal,
			XMVectorSet(0.f, 0.f, 0.f, 1.f)
		);
	}
}

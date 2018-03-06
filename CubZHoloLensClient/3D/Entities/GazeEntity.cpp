#include "pch.h"

#include "3D\Entities\GazeEntity.h"
#include "3D\Entities\CubeEntity.h"
#include "3D\Utility\DirectXHelper.h"
#include "3D\Scene\HolographicScene.h"

using namespace HoloLensClient;
using namespace DirectX;
using namespace Windows::Foundation::Numerics;

GazeEntity::GazeEntity(std::shared_ptr<DX::DeviceResources> devicesResources, std::shared_ptr<HolographicScene> scene)
	: _mesh(devicesResources), _scene(std::move(scene))
{
	InitializeMesh();
}

GazeEntity::~GazeEntity()
{
}

void GazeEntity::Update(DX::StepTimer const & timer)
{
	Windows::UI::Input::Spatial::SpatialPointerPose ^pointerPose = _scene->getPointerPose();
	if (pointerPose != nullptr)
	{
		// Get the gaze direction relative to the given coordinate system.
		const float3 headPosition = pointerPose->Head->Position;
		const float3 headDirection = pointerPose->Head->ForwardDirection;

		// The tag-along hologram follows a point 2.0m in front of the user's gaze direction.
		static const float distanceFromUser = 2.0f; // meters
		const float3 gazeAtTwoMeters = headPosition + (distanceFromUser * headDirection);

		// Lerp the position, to keep the hologram comfortably stable.
		//auto lerpedPosition = lerp(getPosition(), gazeAtTwoMeters, dtime * c_lerpRate);

		// This will be used as the translation component of the hologram's
		// model transform.
		_mesh.setPosition(gazeAtTwoMeters);
	}
	//Position of gaze
	const XMMATRIX modelTranslation = XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&_mesh.getPosition()));

	// Create a direction normal from the hologram's position to the origin of person space.
	// This is the z-axis rotation.
	XMVECTOR facingNormal = XMVector3Normalize(-XMLoadFloat3(&_mesh.getPosition()));

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
	XMMATRIX rotationMatrix = XMMATRIX(
		xAxisRotation,
		yAxisRotation,
		facingNormal,
		XMVectorSet(0.f, 0.f, 0.f, 1.f)
	);

	// The view and projection matrices are provided by the system; they are associated
	// with holographic cameras, and updated on a per-camera basis.
	// Here, we provide the model transform for the sample hologram. The model transform
	// matrix is transposed to prepare it for the shader.
	_mesh.ApplyMatrix(XMMatrixTranspose(rotationMatrix * modelTranslation));

	// Loading is asynchronous. Resources must be created before they can be updated.
	_mesh.Update();
}

void GazeEntity::Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState ^pointerState)
{
	auto pointerPose = pointerState->TryGetPointerPose(_scene->getCoordinateSystem());

	if (pointerPose != nullptr)
	{
		auto Cube = std::make_unique<CubeEntity>(_mesh.getDeviceResources(), _scene);

		const float3 headPosition = pointerPose->Head->Position;
		const float3 headDirection = pointerPose->Head->ForwardDirection;

		// The hologram is positioned two meters along the user's gaze direction.
		constexpr float distanceFromUser = 2.0f; // meters
		const float3 gazeAtTwoMeters = headPosition + (distanceFromUser * headDirection);

		Cube->setPosition(gazeAtTwoMeters);
		_scene->addEntity(std::move(Cube));
	}
}

void GazeEntity::InitializeMesh()
{
	_mesh.Initialize();
}

void GazeEntity::ReleaseMesh()
{
	_mesh.Release();
}

void GazeEntity::Render()
{
	_mesh.Render();
}

ObjectRenderer const &GazeEntity::getMesh() const
{
	return (_mesh);
}

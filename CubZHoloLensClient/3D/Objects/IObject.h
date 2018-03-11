#pragma once

#include <DirectXCollision.h>
#include <3D\Resources\DeviceResources.h>
#include <3D\Utility\StepTimer.h>

namespace HoloLensClient
{
	// Define the properties of a simple object in the 3D world
	class IObject
	{
	public:
		// Initializes the resources used by this object
		virtual void CreateDeviceDependentResources() = 0;
		// Releases the resources used by this object
		virtual void ReleaseDeviceDependentResources() = 0;

		// Update this object
		virtual void Update() = 0;

		// Render this object
		virtual void Render() = 0;

		// Force apply the modelTransform to the model
		// You shouldn't call the Update function when using this one
		virtual void ApplyMatrix(DirectX::XMMATRIX const &modelTransform) = 0;

		// Change the 3D position
		virtual void SetPosition(Windows::Foundation::Numerics::float3 pos) = 0;
		// Change the 3D rotation
		virtual void SetRotation(Windows::Foundation::Numerics::float3 rot) = 0;
		// Change the scale
		virtual void SetScale(Windows::Foundation::Numerics::float3 scale) = 0;

		// Return the 3D position
		virtual Windows::Foundation::Numerics::float3 GetPosition() const = 0;
		// Return the 3D rotation
		virtual Windows::Foundation::Numerics::float3 GetRotation() const = 0;
		// Return the 3D scale
		virtual Windows::Foundation::Numerics::float3 GetScale() const = 0;

		// Set 3D Position matrix
		virtual void SetPosition(DirectX::XMMATRIX const &posMatrix) = 0;

		// Set 3D Rotation matrix
		virtual void SetRotation(DirectX::XMMATRIX const &rotMatrix) = 0;

		// Return the 3D position matrix
		virtual DirectX::XMMATRIX const &getPositionMatrix() const = 0;

		// Return the 3D rotation matrix
		virtual DirectX::XMMATRIX const &getRotationMatrix() const = 0;

		// Get the DX::DeviceResources
		virtual std::shared_ptr<DX::DeviceResources>	getDeviceResources() const = 0;

		// Get the BoundingBox of the mesh
		virtual void GetBoundingBox(DirectX::BoundingOrientedBox &boundingBox) = 0;
	};
}
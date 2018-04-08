#pragma once

#include <memory>
#include <DirectXCollision.h>
#include <3D\Resources\DeviceResources.h>
#include <3D\Utility\StepTimer.h>

namespace HoloLensClient
{
	// Define the properties of a simple object in the 3D world
	class IObject
	{
	public:
		typedef std::unique_ptr<IObject> IObjectPtr;
	public:
		// Default destructor of the interface
		virtual ~IObject() = default;

		// Initializes the resources used by this object
		virtual void CreateDeviceDependentResources() = 0;
		// Releases the resources used by this object
		virtual void ReleaseDeviceDependentResources() = 0;

		// Render this object
		virtual void Render() = 0;

		// Force the model transform for this update loop
		virtual void SetModelTransform(DirectX::XMMATRIX const &modelTransform) = 0;

		// Get the DX::DeviceResources
		virtual std::shared_ptr<DX::DeviceResources>	getDeviceResources() const = 0;

		// Get the BoundingBox of the mesh
		virtual void GetBoundingBox(DirectX::BoundingOrientedBox &boundingBox) = 0;
	};
}
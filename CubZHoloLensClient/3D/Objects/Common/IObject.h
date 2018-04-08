///-------------------------------------------------------------------------------------------------
// file:	3D\Objects\Common\IObject.h
//
// summary:	Declares the IObject interface
///-------------------------------------------------------------------------------------------------
#pragma once

#include <memory>
#include <DirectXCollision.h>
#include <3D\Resources\DeviceResources.h>
#include <3D\Utility\StepTimer.h>


///-------------------------------------------------------------------------------------------------
// namespace: HoloLensClient
//
// summary:	.
///-------------------------------------------------------------------------------------------------
namespace HoloLensClient
{
	/// <summary>	Define the properties of a simple object in the 3D world. </summary>
	class IObject
	{
	public:
		/// <summary>	Defines an alias representing zero-based index of the object pointer. </summary>
		typedef std::unique_ptr<IObject> IObjectPtr;
	public:
		/// <summary>	Default destructor of the interface. </summary>
		virtual ~IObject() = default;

		/// <summary>	Initializes the resources used by this object. </summary>
		virtual void CreateDeviceDependentResources() = 0;
		/// <summary>	Releases the resources used by this object. </summary>
		virtual void ReleaseDeviceDependentResources() = 0;

		/// <summary>	Render this object. </summary>
		virtual void Render() = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Force the model transform for this update loop. </summary>
		///
		/// <param name="modelTransform">	The model transform. </param>
		///-------------------------------------------------------------------------------------------------
		virtual void SetModelTransform(DirectX::XMMATRIX const &modelTransform) = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Get the DX::DeviceResources. </summary>
		///
		/// <returns>	The device resources. </returns>
		///-------------------------------------------------------------------------------------------------
		virtual std::shared_ptr<DX::DeviceResources>	getDeviceResources() const = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Get the BoundingBox of the mesh. </summary>
		///
		/// <param name="boundingBox">	[in,out] The bounding box. </param>
		///-------------------------------------------------------------------------------------------------
		virtual void GetBoundingBox(DirectX::BoundingOrientedBox &boundingBox) = 0;
	};
}
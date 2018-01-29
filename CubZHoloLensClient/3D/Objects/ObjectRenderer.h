#pragma once

#include "3D\Resouces\DeviceResources.h"
#include "3D\Utility\StepTimer.h"

namespace HoloLensClient
{
	//Abstract class representing an object to be draw in
	//the holographic space
	class ObjectRenderer
	{
	protected:
		std::shared_ptr<DX::DeviceResources>	_deviceResources;
		Windows::Foundation::Numerics::float3	_pos;
	public:
		ObjectRenderer(std::shared_ptr<DX::DeviceResources> devicesResources);
		virtual ~ObjectRenderer();

	public:
		virtual void Render() = 0;
		virtual void Update(DX::StepTimer const &timer) = 0;
		virtual void CreateDeviceDependentResources() = 0;
		virtual void ReleaseDeviceDependentResources() = 0;

	public:
		virtual void setPosition(Windows::Foundation::Numerics::float3 pos);
		virtual Windows::Foundation::Numerics::float3 getPosition() const;
	};
}
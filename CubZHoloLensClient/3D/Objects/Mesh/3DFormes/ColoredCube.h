#pragma once

#include "3D\Shaders\ShaderStructures.h"
#include "3D\Objects\Mesh\Common\ColoredObject.h"

namespace HoloLensClient
{
	class ColoredCube : public ColoredObject
	{
	private:
		Windows::Foundation::Numerics::float3           _size;

	public:
		ColoredCube(std::shared_ptr<DX::DeviceResources> &devicesResources,
					Windows::Foundation::Numerics::float4 color = { 1.0f, 1.0f, 1.0f, 1.0f },
					Windows::Foundation::Numerics::float3 size = { 1.0f, 1.0f, 1.0f });
		~ColoredCube();
		
	private:
		void CreateMesh() override;
	};
}


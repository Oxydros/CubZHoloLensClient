#pragma once

#include <DirectXMath.h>
#include "3D\Shaders\ShaderStructures.h"
#include "3D\Objects\Mesh\Common\ColoredObject.h"

namespace HoloLensClient
{
	class ColoredRectangle : public ColoredObject
	{
	private:
		Windows::Foundation::Numerics::float2	_size;

	public:
		ColoredRectangle(std::shared_ptr<DX::DeviceResources> &devicesResources,
			Windows::Foundation::Numerics::float2 size,
			Windows::Foundation::Numerics::float4 color = { 1.0f, 1.0f, 1.0f, 1.0f });
		~ColoredRectangle();

	private:
		void CreateMesh() override;
	};
}


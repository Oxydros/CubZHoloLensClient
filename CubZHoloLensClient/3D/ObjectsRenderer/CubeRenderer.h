#pragma once

#include "3D\Shaders\ShaderStructures.h"
#include "3D\ObjectsRenderer\ObjectRenderer.h"

namespace HoloLensClient
{
	class CubeRenderer : public ObjectRenderer
	{
	private:
		DirectX::XMFLOAT3		const &_color;

	public:
		CubeRenderer(std::shared_ptr<DX::DeviceResources> devicesResources,
					 DirectX::XMFLOAT3 color = { 1.0f, 1.0f, 1.0f });
		~CubeRenderer();
		
	private:
		void CreateMesh() override;
	};
}


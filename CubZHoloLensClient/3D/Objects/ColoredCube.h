#pragma once

#include "3D\Shaders\ShaderStructures.h"
#include "3D\Objects\SceneObject.h"

namespace HoloLensClient
{
	class ColoredCube : public SceneObject
	{
	private:
		DirectX::XMFLOAT3			_color;
		DirectX::XMFLOAT3			_size;

	public:
		ColoredCube(std::shared_ptr<DX::DeviceResources> &devicesResources,
					DirectX::XMFLOAT3 color = { 1.0f, 1.0f, 1.0f },
					DirectX::XMFLOAT3 size = { 0.1f, 0.1f, 0.1f });
		~ColoredCube();
		
	private:
		void CreateMesh() override;
	};
}


#pragma once

#include "3D\Shaders\ShaderStructures.h"
#include "3D\Objects\ObjectRenderer.h"

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

	public:
		void Update(DX::StepTimer const & timer) override;

		void Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState) override;

	private:
		void CreateMesh() override;
	};
}


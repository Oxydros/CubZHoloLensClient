#pragma once

#include "3D\Shaders\ShaderStructures.h"
#include "3D\Objects\ObjectRenderer.h"

namespace HoloLensClient
{
	class GazeRenderer : public ObjectRenderer
	{
	public:
		GazeRenderer(std::shared_ptr<DX::DeviceResources> devicesResources, std::shared_ptr<HolographicScene> sharedScene);
		~GazeRenderer();

	public:
		void Update(DX::StepTimer const & timer) override;
		void Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState) override;

	private:
		void CreateMesh() override;
	};
}


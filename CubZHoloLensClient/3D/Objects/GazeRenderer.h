#pragma once
#include "3D\Objects\ObjectRenderer.h"

namespace HoloLensClient
{
	class GazeRenderer : public ObjectRenderer
	{
	public:
		GazeRenderer(std::shared_ptr<DX::DeviceResources> devicesResources, HolographicScene const *scene);
		~GazeRenderer();

	public:
		void Render() override;

		void Update(DX::StepTimer const & timer) override;

		void CreateDeviceDependentResources() override;

		void ReleaseDeviceDependentResources() override;

		void Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState) override;
	};
}


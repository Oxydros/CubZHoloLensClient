#pragma once
#include "3D\Objects\ObjectRenderer.h"

namespace HoloLensClient
{
	class GazeRenderer : public ObjectRenderer
	{
	public:
		GazeRenderer(std::shared_ptr<DX::DeviceResources> deviceResources);
		~GazeRenderer();

	public:
		virtual void Render() override;

		virtual void Update(DX::StepTimer const & timer) override;

		virtual void CreateDeviceDependentResources() override;

		virtual void ReleaseDeviceDependentResources() override;

	};
}


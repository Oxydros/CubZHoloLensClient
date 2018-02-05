#include "pch.h"
#include "GazeRenderer.h"

using namespace HoloLensClient;

GazeRenderer::GazeRenderer(std::shared_ptr<DX::DeviceResources> devicesResources, HolographicScene const *scene)
	: ObjectRenderer(devicesResources, scene)
{
}

GazeRenderer::~GazeRenderer()
{
}

void GazeRenderer::Render()
{
}

void GazeRenderer::Update(DX::StepTimer const & timer)
{
}

void GazeRenderer::CreateDeviceDependentResources()
{
}

void GazeRenderer::ReleaseDeviceDependentResources()
{
}

void HoloLensClient::GazeRenderer::Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState ^pointerState)
{
}

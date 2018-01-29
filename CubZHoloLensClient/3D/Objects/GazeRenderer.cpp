#include "pch.h"
#include "GazeRenderer.h"

using namespace HoloLensClient;

GazeRenderer::GazeRenderer(std::shared_ptr<DX::DeviceResources> deviceResources)
	: ObjectRenderer(deviceResources)
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

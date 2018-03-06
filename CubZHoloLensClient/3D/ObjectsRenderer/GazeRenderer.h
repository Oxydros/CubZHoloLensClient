#pragma once

#include "3D\Shaders\ShaderStructures.h"
#include "3D\ObjectsRenderer\ObjectRenderer.h"

namespace HoloLensClient
{
	class GazeRenderer : public ObjectRenderer
	{
	public:
		GazeRenderer(std::shared_ptr<DX::DeviceResources> devicesResources);
		~GazeRenderer();

	private:
		void CreateMesh() override;
	};
}


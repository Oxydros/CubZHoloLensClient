#pragma once

#include "3D\Shaders\ShaderStructures.h"
#include "3D\Objects\SceneObject.h"

namespace HoloLensClient
{
	class GazeRenderer : public SceneObject
	{
	public:
		GazeRenderer(std::shared_ptr<DX::DeviceResources> &devicesResources);
		~GazeRenderer();

	private:
		void CreateMesh() override;
	};
}


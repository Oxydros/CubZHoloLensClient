#pragma once

#include "3D\Resources\DeviceResources.h"
#include "3D\Utility\StepTimer.h"
#include "3D\Shaders\ShaderStructures.h"
#include "3D\Objects\SceneObject.h"

namespace HoloLensClient
{
    // This sample renderer instantiates a basic rendering pipeline.
    class SpinningCubeRenderer : public SceneObject
    {
    public:
        SpinningCubeRenderer(std::shared_ptr<DX::DeviceResources> &deviceResources);

	public:
        // Repositions the sample hologram.
        void PositionHologram(Windows::UI::Input::Spatial::SpatialPointerPose^ pointerPose);

	private:
		void CreateMesh() override;
    };
}

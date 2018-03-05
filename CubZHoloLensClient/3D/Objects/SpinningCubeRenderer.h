#pragma once

#include "3D\Resources\DeviceResources.h"
#include "3D\Utility\StepTimer.h"
#include "3D\Shaders\ShaderStructures.h"
#include "3D\Objects\ObjectRenderer.h"

namespace HoloLensClient
{
    // This sample renderer instantiates a basic rendering pipeline.
    class SpinningCubeRenderer : public ObjectRenderer
    {
    public:
        SpinningCubeRenderer(std::shared_ptr<DX::DeviceResources> deviceResources, std::shared_ptr<HolographicScene> sharedScene);

	public:
        void Update(const DX::StepTimer& timer) override;
		void Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState) override;

        // Repositions the sample hologram.
        void PositionHologram(Windows::UI::Input::Spatial::SpatialPointerPose^ pointerPose);

	private:
		void CreateMesh() override;
    };
}

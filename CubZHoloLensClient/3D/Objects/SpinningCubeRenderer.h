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
        SpinningCubeRenderer(std::shared_ptr<DX::DeviceResources> deviceResources, HolographicScene const *scene);
        void CreateDeviceDependentResources() override;
        void ReleaseDeviceDependentResources() override;
        void Update(const DX::StepTimer& timer) override;
        void Render() override;
		void Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState) override;

        // Repositions the sample hologram.
        void PositionHologram(Windows::UI::Input::Spatial::SpatialPointerPose^ pointerPose);

    private:
        // Direct3D resources for cube geometry.
        Microsoft::WRL::ComPtr<ID3D11InputLayout>       m_inputLayout;
        Microsoft::WRL::ComPtr<ID3D11Buffer>            m_vertexBuffer;
        Microsoft::WRL::ComPtr<ID3D11Buffer>            m_indexBuffer;
        Microsoft::WRL::ComPtr<ID3D11VertexShader>      m_vertexShader;
        Microsoft::WRL::ComPtr<ID3D11GeometryShader>    m_geometryShader;
        Microsoft::WRL::ComPtr<ID3D11PixelShader>       m_pixelShader;
        Microsoft::WRL::ComPtr<ID3D11Buffer>            m_modelConstantBuffer;

        // System resources for cube geometry.
        ModelConstantBuffer                             m_modelConstantBufferData;
        uint32                                          m_indexCount = 0;

        // Variables used with the rendering loop.
        bool                                            m_loadingComplete = false;
        float                                           m_degreesPerSecond = 45.f;

        // If the current D3D Device supports VPRT, we can avoid using a geometry
        // shader just to set the render target array index.
        bool                                            m_usingVprtShaders = false;
    };
}

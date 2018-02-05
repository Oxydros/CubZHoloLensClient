#pragma once

#include "3D\Shaders\ShaderStructures.h"
#include "3D\Objects\ObjectRenderer.h"

namespace HoloLensClient
{
	class GazeRenderer : public ObjectRenderer
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout>       _inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer>            _vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>            _indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>      _vertexShader;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader>    _geometryShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>       _pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>            _modelConstantBuffer;

		ModelConstantBuffer                             _modelConstantBufferData;
		uint32                                          _indexCount = 0;

		bool                                            _loadingComplete = false;
		bool                                            _usingVprtShaders = false;

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


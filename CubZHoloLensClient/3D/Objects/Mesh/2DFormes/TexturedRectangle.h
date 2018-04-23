#pragma once

#include <3D\Shaders\ShaderStructures.h>
#include <3D\Objects\Mesh\Interfaces\ITexturedObject.h>

namespace HoloLensClient
{
	class TexturedRectangle : public ITexturedObject
	{
	protected:
		Microsoft::WRL::ComPtr<ID3D11InputLayout>       _inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer>            _vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>            _indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>      _vertexShader;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader>    _geometryShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>       _pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>            _modelConstantBuffer;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>      _samplerState;

		ColorModelConstantBuffer                        _modelConstantBufferData;
		uint32                                          _indexCount = 0;

		bool                                            _loadingComplete = false;
		bool                                            _usingVprtShaders = false;

		std::shared_ptr<DX::DeviceResources>			_deviceResources;

		Windows::Foundation::Numerics::float4			_color = { 0.0f, 0.0f, 0.0f, 0.0f };
		Windows::Foundation::Numerics::float2           _size;

		DirectX::XMMATRIX								_modelTransform;

		DirectX::XMFLOAT4X4								_transform;
		DirectX::BoundingOrientedBox					_boundingBox;

		std::shared_ptr<Texture2D>						_texture;

	public:
		TexturedRectangle(std::shared_ptr<DX::DeviceResources> devicesResources,
						Windows::Foundation::Numerics::float2 size = { 1.0f, 1.0f });
		virtual ~TexturedRectangle();

	public:
		virtual void CreateDeviceDependentResources() override;

		virtual void ReleaseDeviceDependentResources() override;

		virtual void Render() override;

		virtual void SetModelTransform(DirectX::XMMATRIX const & modelTransform) override;

		virtual std::shared_ptr<DX::DeviceResources> getDeviceResources() const override { return _deviceResources; };

		virtual void GetBoundingBox(DirectX::BoundingOrientedBox & boundingBox) override;

		virtual void SetColor(float4 color) override { _color = color; };

		virtual float4 GetColor() const override { return _color; }

		virtual void SetTexture(std::shared_ptr<Texture2D>& texture) override;

		virtual std::shared_ptr<Texture2D> getTexture() const override { return _texture; }

	public:
		// Required for align of 16B for XMMAtrix
		// https://stackoverflow.com/questions/20104815/warning-c4316-object-allocated-on-the-heap-may-not-be-aligned-16
		void* operator new(size_t i)
		{
			return _mm_malloc(i, 16);
		}

		// Required for align of 16B for XMMAtrix
		// https://stackoverflow.com/questions/20104815/warning-c4316-object-allocated-on-the-heap-may-not-be-aligned-16
		void operator delete(void* p)
		{
			_mm_free(p);
		}
	};
}
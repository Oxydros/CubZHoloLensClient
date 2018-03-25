#pragma once

#include "3D\Shaders\ShaderStructures.h"
#include <3D\Objects\GUI\Interfaces\ITextObject.h>
#include <3D\Objects\GUI\DistanceFieldRenderer.h>
#include <3D\Objects\GUI\TextRenderer.h>
#include <3D\Resources\Texture2D.h>

namespace HoloLensClient
{
	class TextObject : public ITextObject
	{
	protected:
		std::wstring									_text;

		Microsoft::WRL::ComPtr<ID3D11InputLayout>       _inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer>            _vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>            _indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>      _vertexShader;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader>    _geometryShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>       _pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>            _modelConstantBuffer;

		// Direct3D resources for the default texture.
		//Microsoft::WRL::ComPtr<ID3D11Resource>              _quadTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    _quadTextureView;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>          _quadTextureSamplerState;

		ColorModelConstantBuffer							_modelConstantBufferData;
		uint32												_indexCount = 0;

		bool												_loadingComplete = false;
		bool												_usingVprtShaders = false;
		bool												_updateText = false;

		std::shared_ptr<DX::DeviceResources>					_deviceResources;

		Windows::Foundation::Numerics::float3					_position = { 0.0f, 0.0f, 0.0f };
		Windows::Foundation::Numerics::float3					_rotation = { 0.0f, 0.0f, 0.0f };
		Windows::Foundation::Numerics::float3					_scale = { 1.0f, 1.0f, 1.0f };
		Windows::Foundation::Numerics::float4					_color = { 0.2f, 0.2f, 0.2f, 0.0f };
		Windows::Foundation::Numerics::float2					_size;


		DirectX::XMMATRIX										_modelRotation;
		DirectX::XMMATRIX										_modelTranslation;

		DirectX::XMFLOAT4X4										_transform;
		DirectX::BoundingOrientedBox							_boundingBox;

		std::unique_ptr<TextRenderer>							_textRenderer;
		std::unique_ptr<DistanceFieldRenderer>					_distanceFieldRenderer;
	public:
		TextObject(std::shared_ptr<DX::DeviceResources> &deviceResources,
					Windows::Foundation::Numerics::float2 size,
					Windows::Foundation::Numerics::float4 color = { 0.2f, 0.2f, 0.2f, 0.0f },
					std::wstring const &text = L"Default text");
		virtual ~TextObject() = default;

	public:
		virtual void CreateDeviceDependentResources() override;
		virtual void ReleaseDeviceDependentResources() override;
		virtual void Update() override;
		virtual void Render() override;
		virtual void ApplyMatrix(DirectX::XMMATRIX const & modelTransform) override;
		virtual void Translate(Windows::Foundation::Numerics::float3 translation) override;
		virtual void SetPosition(Windows::Foundation::Numerics::float3 pos) override;
		virtual void SetRotation(Windows::Foundation::Numerics::float3 rot) override;
		virtual void SetScale(Windows::Foundation::Numerics::float3 scale) override { _scale = scale; }
		virtual Windows::Foundation::Numerics::float3 GetPosition() const override { return (_position); }
		virtual Windows::Foundation::Numerics::float3 GetRotation() const override { return (_rotation); }
		virtual Windows::Foundation::Numerics::float3 GetScale() const override { return (_scale); }
		virtual void SetPosition(DirectX::XMMATRIX const &posMatrix) override { _modelTranslation = posMatrix; };
		virtual void SetRotation(DirectX::XMMATRIX const &rotMatrix) override { _modelRotation = rotMatrix; }
		virtual DirectX::XMMATRIX const &getPositionMatrix() const override { return(_modelTranslation); }
		virtual DirectX::XMMATRIX const &getRotationMatrix() const override { return(_modelRotation); }
		virtual std::shared_ptr<DX::DeviceResources> getDeviceResources() const override { return _deviceResources; }
		virtual void GetBoundingBox(DirectX::BoundingOrientedBox & boundingBox) override;

	public:
		virtual std::wstring const &getText() const override { return _text; };
		virtual void setText(std::wstring const &text) override { 
			_text = text; 
			_updateText = true;
		}

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
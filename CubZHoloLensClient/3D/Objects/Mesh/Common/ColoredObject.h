#pragma once

#include "3D\Shaders\ShaderStructures.h"
#include "3D\Utility\DirectXHelper.h"
#include <3D\Objects\Mesh\Interfaces\IColoredObject.h>

using namespace Windows::Foundation::Numerics;

namespace HoloLensClient
{
	//Abstract class representing an object to be draw in
	//the holographic space
	class ColoredObject : public IColoredObject
	{
	protected:
		std::wstring 									_vertexShaderString;
		std::wstring 									_pixelShaderString;
		std::wstring 									_geometryShaderString;
		std::wstring 									_vprtVertexShaderString;

		Microsoft::WRL::ComPtr<ID3D11InputLayout>       _inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer>            _vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>            _indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>      _vertexShader;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader>    _geometryShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>       _pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>            _modelConstantBuffer;

		ColorModelConstantBuffer                        _modelConstantBufferData;
		uint32                                          _indexCount = 0;

		bool                                            _loadingComplete = false;
		bool                                            _usingVprtShaders = false;

	protected:
		std::shared_ptr<DX::DeviceResources>					_deviceResources;

		Windows::Foundation::Numerics::float3					_position = {0.0f, 0.0f, 0.0f};
		Windows::Foundation::Numerics::float3					_rotation = { 0.0f, 0.0f, 0.0f };
		Windows::Foundation::Numerics::float3					_scale = { 1.0f, 1.0f, 1.0f };
		Windows::Foundation::Numerics::float4					_color = { 1.0f, 1.0f, 1.0f, 1.0f };


		DirectX::XMMATRIX										_modelRotation;
		DirectX::XMMATRIX										_modelTranslation;

		DirectX::XMFLOAT4X4										_transform;
		DirectX::BoundingOrientedBox							_boundingBox;

	public:
		ColoredObject(std::shared_ptr<DX::DeviceResources> &devicesResources,
			std::wstring const &vertexShaderString = L"ms-appx:///VertexShaderColor.cso",
			std::wstring const &pixelShaderString = L"ms-appx:///PixelShader.cso",
			std::wstring const &geometryShaderString = L"ms-appx:///GeometryShader.cso",
			std::wstring const &vprtVertexShaderString = L"ms-appx:///VPRTVertexShader.cso"
		);
		virtual ~ColoredObject();

	public:
		void									CreateDeviceDependentResources() override;
		void									ReleaseDeviceDependentResources() override;
		void									Render() override;
		void									Update() override;
		std::shared_ptr<DX::DeviceResources>	getDeviceResources() const override;
		void									ApplyMatrix(DirectX::XMMATRIX const &m) override;

	private:
		Concurrency::task<void>				InitializeShaders();
		virtual void						CreateMesh() = 0;

	public:
		void Translate(Windows::Foundation::Numerics::float3 translation) override;

		void SetPosition(Windows::Foundation::Numerics::float3 position) override;
		void SetRotation(Windows::Foundation::Numerics::float3 rotation) override;
		void SetScale(Windows::Foundation::Numerics::float3 scale) override { _scale = scale; }

		void SetPosition(DirectX::XMMATRIX const &posMatrix) override { _modelTranslation = posMatrix; };

		void SetRotation(DirectX::XMMATRIX const &rotMatrix) override { _modelRotation = rotMatrix; }

		DirectX::XMMATRIX const &getPositionMatrix() const override { return(_modelTranslation); }

		DirectX::XMMATRIX const &getRotationMatrix() const override { return(_modelRotation); }

		Windows::Foundation::Numerics::float3 GetPosition() const override { return (_position); }
		Windows::Foundation::Numerics::float3 GetRotation() const override { return (_rotation); }
		Windows::Foundation::Numerics::float3 GetScale() const override { return (_scale); }

		void GetBoundingBox(DirectX::BoundingOrientedBox &boundingBox) override;

		void SetColor(Windows::Foundation::Numerics::float4 color) override { _color = color; }
		float4 GetColor() const override { return _color; }

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
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
		std::wstring 									_vertexShaderString{};
		std::wstring 									_pixelShaderString{};
		std::wstring 									_geometryShaderString{};
		std::wstring 									_vprtVertexShaderString{};

		Microsoft::WRL::ComPtr<ID3D11InputLayout>       _inputLayout{};
		Microsoft::WRL::ComPtr<ID3D11Buffer>            _vertexBuffer{};
		Microsoft::WRL::ComPtr<ID3D11Buffer>            _indexBuffer{};
		Microsoft::WRL::ComPtr<ID3D11VertexShader>      _vertexShader{};
		Microsoft::WRL::ComPtr<ID3D11GeometryShader>    _geometryShader{};
		Microsoft::WRL::ComPtr<ID3D11PixelShader>       _pixelShader{};
		Microsoft::WRL::ComPtr<ID3D11Buffer>            _modelConstantBuffer{};

		ColorModelConstantBuffer                        _modelConstantBufferData{};
		uint32                                          _indexCount = 0;

		bool                                            _loadingComplete = false;
		bool                                            _usingVprtShaders = false;

	protected:
		std::shared_ptr<DX::DeviceResources>					_deviceResources;
		Windows::Foundation::Numerics::float4					_color = { 1.0f, 1.0f, 1.0f, 1.0f };

		DirectX::XMMATRIX										_modelTransform;
		DirectX::XMFLOAT4X4										_transform;
		DirectX::BoundingOrientedBox							_boundingBox;

	public:
		ColoredObject(std::shared_ptr<DX::DeviceResources> devicesResources,
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
		std::shared_ptr<DX::DeviceResources>	getDeviceResources() const override;
		void									SetModelTransform(DirectX::XMMATRIX const &m) override;

	private:
		Concurrency::task<void>				InitializeShaders();
		virtual void						CreateMesh() = 0;

	public:
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
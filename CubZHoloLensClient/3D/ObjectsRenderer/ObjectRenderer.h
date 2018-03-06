#pragma once

#include "3D\Shaders\ShaderStructures.h"
#include "3D\Resources\DeviceResources.h"
#include "3D\Utility\DirectXHelper.h"
#include "3D\Utility\StepTimer.h"

using namespace Windows::Foundation::Numerics;

namespace HoloLensClient
{
	//Abstract class representing an object to be draw in
	//the holographic space
	class ObjectRenderer
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

		ModelConstantBuffer                             _modelConstantBufferData;
		uint32                                          _indexCount = 0;

		bool                                            _loadingComplete = false;
		bool                                            _usingVprtShaders = false;

	protected:
		std::shared_ptr<DX::DeviceResources>					_deviceResources;



		Windows::Foundation::Numerics::float3					_pos;
		Windows::Foundation::Numerics::float3					_angle;

	public:
		ObjectRenderer(std::shared_ptr<DX::DeviceResources> devicesResources,
			std::wstring const &vertexShaderString = L"ms-appx:///BasicVertexShader.cso",
			std::wstring const &pixelShaderString = L"ms-appx:///BasicPixelShader.cso",
			std::wstring const &geometryShaderString = L"ms-appx:///BasicGeometryShader.cso",
			std::wstring const &vprtVertexShaderString = L"ms-appx:///BasicVprtVertexShader.cso"
		);
		virtual ~ObjectRenderer();

	public:
		void									Initialize();
		bool									isInitialized() const;
		void									Release();
		void									Render();
		void									Update();
		void									ApplyMatrix(DirectX::XMMATRIX const &m);
		std::shared_ptr<DX::DeviceResources>	getDeviceResources();

	private:
		Concurrency::task<void>				InitializeShaders();
		virtual void						CreateMesh() = 0;

	public:
		virtual void Rotate(float3 vecRot);
		virtual void setRotation(float3 rot);
		virtual void Move(float3 vecPos);
		virtual void setPosition(Windows::Foundation::Numerics::float3 pos);
		virtual Windows::Foundation::Numerics::float3 getPosition() const;
	};
}
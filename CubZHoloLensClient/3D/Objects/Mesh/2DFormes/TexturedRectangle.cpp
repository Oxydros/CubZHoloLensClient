#include <pch.h>
#include <3D\Objects\Mesh\2DFormes\TexturedRectangle.h>

using namespace HoloLensClient;
using namespace DirectX;

HoloLensClient::TexturedRectangle::TexturedRectangle(std::shared_ptr<DX::DeviceResources> devicesResources,
										Windows::Foundation::Numerics::float2 size)
	: _deviceResources(devicesResources), _size(size)
{
}

HoloLensClient::TexturedRectangle::~TexturedRectangle()
{
}

void HoloLensClient::TexturedRectangle::CreateDeviceDependentResources()
{
	if (_loadingComplete)
	{
		return;
	}
	_usingVprtShaders = _deviceResources->GetDeviceSupportsVprt();

	std::wstring const &vertexShaderFileName = _usingVprtShaders ? L"ms-appx:///VPRTVertexShaderTexture.cso" : L"ms-appx:///VertexShaderTexture.cso";

	// Load shaders asynchronously.
	Concurrency::task<std::vector<byte>> loadVSTask = DX::ReadDataAsync(vertexShaderFileName);
	Concurrency::task<std::vector<byte>> loadPSTask = DX::ReadDataAsync(L"ms-appx:///PixelShaderTexture.cso");

	Concurrency::task<std::vector<byte>> loadGSTask;

	if (!_usingVprtShaders)
		loadGSTask = DX::ReadDataAsync(L"ms-appx:///GeometryShaderTexture.cso");

	// After the vertex shader file is loaded, create the shader and input layout.
	Concurrency::task<void> createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData)
	{
		DX::ThrowIfFailed(
			_deviceResources->GetD3DDevice()->CreateVertexShader(
				fileData.data(),
				fileData.size(),
				nullptr,
				&_vertexShader
			)
		);

		const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		DX::ThrowIfFailed(
			_deviceResources->GetD3DDevice()->CreateInputLayout(
				vertexDesc,
				static_cast<UINT>(ARRAYSIZE(vertexDesc)),
				fileData.data(),
				static_cast<UINT>(fileData.size()),
				&_inputLayout
			)
		);
	});

	// After the pixel shader file is loaded, create the shader and constant buffer.
	Concurrency::task<void> createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData)
	{
		DX::ThrowIfFailed(
			_deviceResources->GetD3DDevice()->CreatePixelShader(
				fileData.data(),
				fileData.size(),
				nullptr,
				&_pixelShader
			)
		);

		const CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ColorModelConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			_deviceResources->GetD3DDevice()->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				&_modelConstantBuffer
			)
		);
	});

	Concurrency::task<void> createGSTask;
	if (!_usingVprtShaders)
	{
		// After the pass-through geometry shader file is loaded, create the shader.
		createGSTask = loadGSTask.then([this](const std::vector<byte>& fileData)
		{
			DX::ThrowIfFailed(
				_deviceResources->GetD3DDevice()->CreateGeometryShader(
					fileData.data(),
					fileData.size(),
					nullptr,
					&_geometryShader
				)
			);
		});
	}

	auto textureLoad = _texture->Load();
	auto meshTask = _usingVprtShaders ? (createPSTask && createVSTask && textureLoad) : (createPSTask && createVSTask && createGSTask && textureLoad);

	meshTask.then([this]()
	{
		float halfWidth = _size.x * 0.5f;
		float halfHeight = _size.y * 0.5f;

		_boundingBox = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(halfWidth, halfHeight, 0.1f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

		// Load mesh vertices. Each vertex has a position and a texture UV.
		const std::array<VertexPositionTexture, 4> cubeVertices =
		{
			{
				{ XMFLOAT3(-halfWidth, -halfHeight,  0.0f), XMFLOAT2(0.0f, 1.0f) }, // 0 (1)
				{ XMFLOAT3(-halfWidth,  halfHeight,  0.0f), XMFLOAT2(0.0f, 0.0f) }, // 1 (3)
				{ XMFLOAT3(halfWidth, -halfHeight,  0.0f), XMFLOAT2(1.0f, 1.0f) }, // 2 (5)
				{ XMFLOAT3(halfWidth,  halfHeight,  0.0f), XMFLOAT2(1.0f, 0.0f) }, // 3 (7)
			}
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
		vertexBufferData.pSysMem = cubeVertices.data();
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		const CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(VertexPositionTexture) * cubeVertices.size(), D3D11_BIND_VERTEX_BUFFER);
		DX::ThrowIfFailed(
			_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc,
				&vertexBufferData,
				&_vertexBuffer
			)
		);

		// Load mesh indices. Each trio of indices represents
		// a triangle to be rendered on the screen.
		// For example: 2,1,0 means that the vertices with indexes
		// 2, 1, and 0 from the vertex buffer compose the
		// first triangle of this mesh.
		// Note that the winding order is clockwise by default.
		static constexpr unsigned short cubeIndices[] =
		{
			0,1,3,
			0,3,2,
		};

		_indexCount = ARRAYSIZE(cubeIndices);

		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = cubeIndices;
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		const CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDesc,
				&indexBufferData,
				&_indexBuffer
			)
		);
		// Create the sample state
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

		DX::ThrowIfFailed(
			_deviceResources->GetD3DDevice()->CreateSamplerState(
				&sampDesc,
				_samplerState.ReleaseAndGetAddressOf()));
	}).then([this]() {
		_loadingComplete = true;
	});
}

void HoloLensClient::TexturedRectangle::ReleaseDeviceDependentResources()
{
	_loadingComplete = false;
	_usingVprtShaders = false;
	_vertexShader.Reset();
	_inputLayout.Reset();
	_pixelShader.Reset();
	_geometryShader.Reset();
	_modelConstantBuffer.Reset();
	_vertexBuffer.Reset();
	_indexBuffer.Reset();
}
//
//void HoloLensClient::TexturedRectangle::Update()
//{
//	// Multiply to get the transform matrix.
//	// Note that this transform does not enforce a particular coordinate system. The calling
//	// class is responsible for rendering this content in a consistent manner.
//
//	if (!_useForcedTransform) _modelTransform = _modelRotation * _modelTranslation;
//	XMStoreFloat4x4(&_transform, _modelTransform);
//
//	// The view and projection matrices are provided by the system; they are associated
//	// with holographic cameras, and updated on a per-camera basis.
//	// Here, we provide the model transform for the sample hologram. The model transform
//	// matrix is transposed to prepare it for the shader.
//	XMStoreFloat4x4(&_modelConstantBufferData.model, XMMatrixTranspose(_modelTransform));
//
//	_modelConstantBufferData.color = DirectX::XMFLOAT4(_color.x, _color.y, _color.z, _color.w);
//}

void HoloLensClient::TexturedRectangle::Render()
{
	// Only render when vertex and mesh are loaded
	if (!_loadingComplete)
		return;

	const auto context = _deviceResources->GetD3DDeviceContext();

	_modelConstantBufferData.color = DirectX::XMFLOAT4(_color.x, _color.y, _color.z, _color.w);

	// Update the model transform buffer for the hologram.
	context->UpdateSubresource(
		_modelConstantBuffer.Get(),
		0,
		nullptr,
		&_modelConstantBufferData,
		0,
		0
	);

	// Each vertex is one instance of the VertexPositionColor struct.
	const UINT stride = sizeof(VertexPositionTexture);
	const UINT offset = 0;
	context->IASetVertexBuffers(
		0,
		1,
		_vertexBuffer.GetAddressOf(),
		&stride,
		&offset
	);
	context->IASetIndexBuffer(
		_indexBuffer.Get(),
		DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
		0
	);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(_inputLayout.Get());
	// Attach the vertex shader.
	context->VSSetShader(
		_vertexShader.Get(),
		nullptr,
		0
	);
	// Apply the model constant buffer to the vertex shader.
	context->VSSetConstantBuffers(
		0,
		1,
		_modelConstantBuffer.GetAddressOf()
	);

	if (!_usingVprtShaders)
	{
		// On devices that do not support the D3D11_FEATURE_D3D11_OPTIONS3::
		// VPAndRTArrayIndexFromAnyShaderFeedingRasterizer optional feature,
		// a pass-through geometry shader is used to set the render target 
		// array index.
		context->GSSetShader(
			_geometryShader.Get(),
			nullptr,
			0
		);
	}

	// Set Texture Resource view
	context->PSSetShaderResources(
		0,
		1,
		_texture->getTextureView().GetAddressOf());

	// Set Sampler
	context->PSSetSamplers(
		0,
		1,
		_samplerState.GetAddressOf());

	// Attach the pixel shader.
	context->PSSetShader(
		_pixelShader.Get(),
		nullptr,
		0
	);

	// Draw the objects.
	// Two instances: one for each eye
	context->DrawIndexedInstanced(
		_indexCount,   // Index count per instance.
		2,              // Instance count.
		0,              // Start index location.
		0,              // Base vertex location.
		0               // Start instance location.
	);
}

void HoloLensClient::TexturedRectangle::SetModelTransform(DirectX::XMMATRIX const & modelTransform)
{
	_modelTransform = modelTransform;
	XMStoreFloat4x4(&_transform, _modelTransform);
	XMStoreFloat4x4(&_modelConstantBufferData.model, XMMatrixTranspose(_modelTransform));
}
//
//void HoloLensClient::TexturedRectangle::Translate(Windows::Foundation::Numerics::float3 translation)
//{
//	_position += translation;
//	_modelTranslation = XMMatrixTranslationFromVector(XMLoadFloat3(&_position));
//}
//
//void HoloLensClient::TexturedRectangle::SetPosition(Windows::Foundation::Numerics::float3 position)
//{
//	_position = position;
//	_modelTranslation = XMMatrixTranslationFromVector(XMLoadFloat3(&position));
//}
//
//void HoloLensClient::TexturedRectangle::SetRotation(Windows::Foundation::Numerics::float3 rotation)
//{
//	_rotation = rotation;
//	_modelRotation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rotation));
//}

void HoloLensClient::TexturedRectangle::GetBoundingBox(DirectX::BoundingOrientedBox & boundingBox)
{
	_boundingBox.Transform(boundingBox, DirectX::XMLoadFloat4x4(&_transform));
}

void HoloLensClient::TexturedRectangle::SetTexture(std::shared_ptr<Texture2D>& texture)
{
	_texture = texture;
}
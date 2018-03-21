#include <pch.h>
#include <3D\Utility\DirectXHelper.h>
#include <3D\Objects\GUI\TextQuad.h>
#include <3D\Utility\CFW1StateSaver.h>

using namespace HoloLensClient;
using namespace DirectX;

TextQuad::TextQuad(std::shared_ptr<DX::DeviceResources> &deviceResources)
	: _deviceResources(deviceResources)
{
	constexpr unsigned int offscreenRenderTargetWidth = 2048;
	_textRenderer = std::make_unique<TextRenderer>(_deviceResources, offscreenRenderTargetWidth, offscreenRenderTargetWidth);

	constexpr unsigned int blurTargetWidth = 256;
	_distanceFieldRenderer = std::make_unique<DistanceFieldRenderer>(_deviceResources, blurTargetWidth, blurTargetWidth);

	_texture = std::make_shared<Texture2D>(deviceResources, "ms-appx:////Assets//folderIcon.png");
}

void HoloLensClient::TextQuad::CreateDeviceDependentResources()
{
	/*FW1FontWrapper::CFW1StateSaver saver;
	saver.saveCurrentState(_deviceResources->GetD3DDeviceContext());*/

	_textRenderer->CreateDeviceDependentResources();
	//_distanceFieldRenderer->CreateDeviceDependentResources();

	_textRenderer->RenderTextOffscreen(L"Hello world!");
	_quadTextureView = _textRenderer->GetTexture();

	//saver.restoreSavedState();

	_usingVprtShaders = _deviceResources->GetDeviceSupportsVprt();

	// If the optional VPRT feature is supported by the graphics device, we
	// can avoid using geometry shaders to set the render target array index.
	std::wstring vertexShaderFileName = _usingVprtShaders ? L"ms-appx:///VPRTVertexShaderTexture.cso" : L"ms-appx:///VertexShaderTexture.cso";

	// Load shaders asynchronously.
	Concurrency::task<std::vector<byte>> loadVSTask = DX::ReadDataAsync(vertexShaderFileName);
	Concurrency::task<std::vector<byte>> loadPSTask = DX::ReadDataAsync(L"ms-appx:///PixelShaderTexture.cso");

	Concurrency::task<std::vector<byte>> loadGSTask;
	if (!_usingVprtShaders)
	{
		// Load the pass-through geometry shader.
		loadGSTask = DX::ReadDataAsync(L"ms-appx:///GeometryShaderTexture.cso");
	}

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

		constexpr std::array<D3D11_INPUT_ELEMENT_DESC, 2> vertexDesc =
		{ {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,							0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		} };

		DX::ThrowIfFailed(
			_deviceResources->GetD3DDevice()->CreateInputLayout(
				vertexDesc.data(),
				vertexDesc.size(),
				fileData.data(),
				fileData.size(),
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
		// After the geometry shader file is loaded, create the shader.
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

	// Once all shaders are loaded, create the mesh.
	Concurrency::task<void> shaderTaskGroup = _usingVprtShaders ? (createPSTask && createVSTask && textureLoad) : (createPSTask && createVSTask && createGSTask && textureLoad);
	Concurrency::task<void> finishLoadingTask = shaderTaskGroup.then([this]()
	{
		_boundingBox = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0, 0, 0), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

		// Load mesh vertices. Each vertex has a position and a color.
		// Note that the quad size has changed from the default DirectX app
		// template. Windows Holographic is scaled in meters, so to draw the
		// quad at a comfortable size we made the quad width 0.2 m (20 cm).
		static const std::array<VertexPositionTexture, 4> quadVertices =
		{ {
			{ XMFLOAT3(-0.2f,  0.2f, 0.f), XMFLOAT2(0.f, 0.f) },
			{ XMFLOAT3(0.2f,  0.2f, 0.f), XMFLOAT2(1.f, 0.f) },
			{ XMFLOAT3(0.2f, -0.2f, 0.f), XMFLOAT2(1.f, 1.f) },
			{ XMFLOAT3(-0.2f, -0.2f, 0.f), XMFLOAT2(0.f, 1.f) },
		} };

		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
		vertexBufferData.pSysMem = quadVertices.data();
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		const CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(VertexPositionTexture) * quadVertices.size(), D3D11_BIND_VERTEX_BUFFER);
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
		constexpr std::array<unsigned short, 12> quadIndices =
		{ {
				// -z
				0,2,3,
				0,1,2,

				// +z
				2,0,3,
				1,0,2,
			} };

		_indexCount = quadIndices.size();

		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = quadIndices.data();
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		const CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned short) * quadIndices.size(), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDesc,
				&indexBufferData,
				&_indexBuffer
			)
		);

		//DX::ThrowIfFailed(CreateDDSTextureFromFile(
		//	_deviceResources->GetD3DDevice(),
		//	L"ms-appx:///Assets//placeholder.dds",
		//	_quadTexture.GetAddressOf(),
		//	_quadTextureView.GetAddressOf()
		//)
		//);

		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));

		desc.Filter = D3D11_FILTER_ANISOTROPIC;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.MaxAnisotropy = 3;
		desc.MinLOD = 0;
		desc.MaxLOD = 3;
		desc.MipLODBias = 0.f;
		desc.BorderColor[0] = 0.f;
		desc.BorderColor[1] = 0.f;
		desc.BorderColor[2] = 0.f;
		desc.BorderColor[3] = 0.f;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

		DX::ThrowIfFailed(
			_deviceResources->GetD3DDevice()->CreateSamplerState(
				&desc,
				&_quadTextureSamplerState
			)
		);

		// After the assets are loaded, the quad is ready to be rendered.
		_loadingComplete = true;
	});
}

void HoloLensClient::TextQuad::ReleaseDeviceDependentResources()
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

	//_quadTexture.Reset();
	_quadTextureView.Reset();
	_quadTextureSamplerState.Reset();
}

void HoloLensClient::TextQuad::Update()
{
	// Multiply to get the transform matrix.
	// Note that this transform does not enforce a particular coordinate system. The calling
	// class is responsible for rendering this content in a consistent manner.

	const XMMATRIX modelTransform = _modelRotation * _modelTranslation;
	XMStoreFloat4x4(&_transform, modelTransform);

	// The view and projection matrices are provided by the system; they are associated
	// with holographic cameras, and updated on a per-camera basis.
	// Here, we provide the model transform for the sample hologram. The model transform
	// matrix is transposed to prepare it for the shader.
	XMStoreFloat4x4(&_modelConstantBufferData.model, XMMatrixTranspose(modelTransform));

	_modelConstantBufferData.color = DirectX::XMFLOAT4(_color.x, _color.y, _color.z, _color.w);
}

void HoloLensClient::TextQuad::Render()
{
	//if (_distanceFieldRenderer->GetRenderCount() == 0)
	//{
	//	FW1FontWrapper::CFW1StateSaver saver;
	//	saver.saveCurrentState(_deviceResources->GetD3DDeviceContext());

	//	_distanceFieldRenderer->RenderDistanceField(_textRenderer->GetTexture());
	//	_quadTextureView = _distanceFieldRenderer->GetTexture();
	//	// The 2048x2048 texture will not be needed again, unless we get hit DeviceLost scenario.
	//	_textRenderer->ReleaseDeviceDependentResources();

	//	saver.restoreSavedState();
	//}

	// Loading is asynchronous. Resources must be created before drawing can occur.
	if (!_loadingComplete)
	{
		return;
	}

	const auto context = _deviceResources->GetD3DDeviceContext();

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
		// a pass-through geometry shader sets the render target ID.
		context->GSSetShader(
			_geometryShader.Get(),
			nullptr,
			0
		);
	}

	// Attach the pixel shader.
	context->PSSetShader(
		_pixelShader.Get(),
		nullptr,
		0
	);
	context->PSSetShaderResources(
		0,
		1,
		//_texture->getTextureView().GetAddressOf()
		_quadTextureView.GetAddressOf()
	);
	context->PSSetSamplers(
		0,
		1,
		_quadTextureSamplerState.GetAddressOf()
	);

	// Draw the objects.
	context->DrawIndexedInstanced(
		_indexCount,   // Index count per instance.
		2,              // Instance count.
		0,              // Start index location.
		0,              // Base vertex location.
		0               // Start instance location.
	);
}

void HoloLensClient::TextQuad::ApplyMatrix(DirectX::XMMATRIX const & modelTransform)
{
	XMStoreFloat4x4(&_modelConstantBufferData.model, XMMatrixTranspose(modelTransform));
}

void HoloLensClient::TextQuad::SetPosition(Windows::Foundation::Numerics::float3 position)
{
	_position = position;
	_modelTranslation = XMMatrixTranslationFromVector(XMLoadFloat3(&position));
}

void HoloLensClient::TextQuad::SetRotation(Windows::Foundation::Numerics::float3 rotation)
{
	_rotation = rotation;
	_modelRotation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rotation));
}

void HoloLensClient::TextQuad::GetBoundingBox(DirectX::BoundingOrientedBox & boundingBox)
{
	_boundingBox.Transform(boundingBox, DirectX::XMLoadFloat4x4(&_transform));
}
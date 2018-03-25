#include "pch.h"
#include "3D\Objects\Mesh\Common\ColoredObject.h"

using namespace HoloLensClient;
using namespace DirectX;

ColoredObject::ColoredObject(std::shared_ptr<DX::DeviceResources> &deviceResources,
			std::wstring const &vShader, std::wstring const &pShader, std::wstring const &gShader,
			std::wstring const &vpVShader)
	: _vertexShaderString(vShader), _pixelShaderString(pShader),
	_geometryShaderString(gShader), _vprtVertexShaderString(vpVShader), _deviceResources(deviceResources)
{
}

ColoredObject::~ColoredObject()
{}

void ColoredObject::CreateDeviceDependentResources()
{
	Concurrency::task<void> initShadersTask = InitializeShaders();
	Concurrency::task<void> createMeshTask = initShadersTask.then([this]()
	{
		CreateMesh();
	}).then([this]()
	{
		_loadingComplete = true;
	});
}

Concurrency::task<void> ColoredObject::InitializeShaders()
{
	_usingVprtShaders = _deviceResources->GetDeviceSupportsVprt();

	std::wstring const &vertexShaderFileName = _usingVprtShaders ? _vprtVertexShaderString : _vertexShaderString;

	// Load shaders asynchronously.
	Concurrency::task<std::vector<byte>> loadVSTask = DX::ReadDataAsync(vertexShaderFileName);
	Concurrency::task<std::vector<byte>> loadPSTask = DX::ReadDataAsync(_pixelShaderString);

	Concurrency::task<std::vector<byte>> loadGSTask;

	if (!_usingVprtShaders)
	{
		// Load the pass-through geometry shader.
		loadGSTask = DX::ReadDataAsync(_geometryShaderString);
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

		constexpr std::array<D3D11_INPUT_ELEMENT_DESC, 1> vertexDesc =
		{ {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			} };

		DX::ThrowIfFailed(
			_deviceResources->GetD3DDevice()->CreateInputLayout(
				vertexDesc.data(),
				static_cast<UINT>(vertexDesc.size()),
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

	return _usingVprtShaders ? (createPSTask && createVSTask) : (createPSTask && createVSTask && createGSTask);
}

void HoloLensClient::ColoredObject::Translate(Windows::Foundation::Numerics::float3 translation)
{
	_position += translation;
	_modelTranslation = XMMatrixTranslationFromVector(XMLoadFloat3(&_position));
}

void ColoredObject::SetPosition(Windows::Foundation::Numerics::float3 position)
{
	_position = position;
	_modelTranslation = XMMatrixTranslationFromVector(XMLoadFloat3(&position));
}

void ColoredObject::SetRotation(Windows::Foundation::Numerics::float3 rotation)
{
	_rotation = rotation;
	_modelRotation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rotation));
}

void ColoredObject::GetBoundingBox(DirectX::BoundingOrientedBox & boundingBox)
{
	_boundingBox.Transform(boundingBox, DirectX::XMLoadFloat4x4(&_transform));
}

// Renders one frame using the vertex and pixel shaders.
// On devices that do not support the D3D11_FEATURE_D3D11_OPTIONS3::
// VPAndRTArrayIndexFromAnyShaderFeedingRasterizer optional feature,
// a pass-through geometry shader is also used to set the render 
// target array index.
void ColoredObject::Render()
{
	// Only render when vertex and mesh are loaded
	if (!_loadingComplete)
		return;

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
	const UINT stride = sizeof(VertexPosition);
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

void ColoredObject::Update()
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

std::shared_ptr<DX::DeviceResources> ColoredObject::getDeviceResources() const
{
	return (_deviceResources);
}

void ColoredObject::ApplyMatrix(DirectX::XMMATRIX const &modelTransform)
{
	XMStoreFloat4x4(&_modelConstantBufferData.model, XMMatrixTranspose(modelTransform));
}

void ColoredObject::ReleaseDeviceDependentResources()
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
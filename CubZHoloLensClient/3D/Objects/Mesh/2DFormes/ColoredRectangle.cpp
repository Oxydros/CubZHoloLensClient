#include "pch.h"

#include "3D\Scene\HolographicScene.h"
#include "3D\Utility\DirectXHelper.h"
#include "3D\Objects\Mesh\2DFormes\ColoredRectangle.h"

using namespace HoloLensClient;
using namespace DirectX;
using namespace Windows::Foundation::Numerics;

ColoredRectangle::ColoredRectangle(std::shared_ptr<DX::DeviceResources> devicesResources,
								Windows::Foundation::Numerics::float2 size,
								Windows::Foundation::Numerics::float4 color)
	: ColoredObject(devicesResources), _size(size)
{
	SetColor(color);
	/*SetPosition({ 0.0f, 0.0f, -2.0f });*/
}

ColoredRectangle::~ColoredRectangle()
{
}

void ColoredRectangle::CreateMesh()
{

	float halfWidth = _size.x * 0.5f;
	float halfHeight = _size.y * 0.5f;

	_boundingBox = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(halfWidth, halfHeight, 0.001f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	// Load mesh vertices. Each vertex has a position and a texture UV.
	const std::array<VertexPosition, 4> rectangleVertices =
	{
		{
			{ XMFLOAT3(-halfWidth, -halfHeight,  0.0f) }, // 0 (1)
			{ XMFLOAT3(-halfWidth,  halfHeight,  0.0f) }, // 1 (3)
			{ XMFLOAT3(halfWidth, -halfHeight,  0.0f) }, // 2 (5)
			{ XMFLOAT3(halfWidth,  halfHeight,  0.0f) }, // 3 (7)
		}
	};

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = rectangleVertices.data();
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	const CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(VertexPosition) * static_cast<UINT>(rectangleVertices.size()), D3D11_BIND_VERTEX_BUFFER);
	DX::ThrowIfFailed(
		_deviceResources->GetD3DDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			&_vertexBuffer
		)
	);

	static constexpr unsigned short rectangleIndices[] =
	{
		0,1,3,
		0,3,2,
	};

	_indexCount = ARRAYSIZE(rectangleIndices);

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = rectangleIndices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(rectangleIndices), D3D11_BIND_INDEX_BUFFER);
	DX::ThrowIfFailed(
		_deviceResources->GetD3DDevice()->CreateBuffer(
			&indexBufferDesc,
			&indexBufferData,
			&_indexBuffer
		)
	);
}
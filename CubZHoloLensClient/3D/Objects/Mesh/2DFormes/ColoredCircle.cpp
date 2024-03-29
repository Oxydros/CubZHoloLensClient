#include "pch.h"

#include "3D\Scene\HolographicScene.h"
#include "3D\Utility\DirectXHelper.h"
#include "3D\Objects\Mesh\2DFormes\ColoredCircle.h"

using namespace HoloLensClient;
using namespace DirectX;
using namespace Windows::Foundation::Numerics;

ColoredCircle::ColoredCircle(std::shared_ptr<DX::DeviceResources> devicesResources,
								float radius,
								float radians,
								Windows::Foundation::Numerics::float4 color)
	: ColoredObject(devicesResources), _radius(radius), _precision(64), _radians(radians)
{
	SetColor(color);
	/*SetPosition({ 0.0f, 0.0f, -2.0f });*/
}

ColoredCircle::~ColoredCircle()
{
}

// From HolographicPartialStage / SpatialStageManager.cpp
// Creates a 2D circle in the x-z plane, with the specified properties.
std::vector<float3> ColoredCircle::CreateCircle(float3 center, float radius, int divisions, float radians)
{
	std::vector<float3> vertices(divisions + 1);

	vertices[0] = center;
	for (int i = 0; i < divisions; ++i)
	{
		float radiansThisIteration = radians * float(i) / float(divisions);
		vertices[i + 1] =
		{
			center.x + (radius * -cos(radiansThisIteration)),
			center.y + (radius * sin(radiansThisIteration)),
			center.z,
		};
	}
	return vertices;
}

void ColoredCircle::CreateMesh()
{
	//Create shape
	std::vector<float3> triangleVertices = CreateCircle(0.0f, _radius, _precision, _radians);
	//Add center vertex

	std::vector<VertexPosition> colorTriangleVertices(triangleVertices.size());

	XMFLOAT3 color(_color.x, _color.y, _color.z);

	for (size_t i = 0; i < triangleVertices.size(); i++)
	{
		colorTriangleVertices[i].pos = DirectX::XMFLOAT3(triangleVertices[i].x, triangleVertices[i].y, triangleVertices[i].z);
	}

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = colorTriangleVertices.data();
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	const CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(VertexPosition) * static_cast<UINT>(colorTriangleVertices.size()), D3D11_BIND_VERTEX_BUFFER);
	DX::ThrowIfFailed(
		_deviceResources->GetD3DDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			&_vertexBuffer
		)
	);

	//Number of indices = number of vertex - 1 (the center)
	//Must draw in clock time order
	std::vector<unsigned short> triangleIndices((colorTriangleVertices.size() - 1) * 3);
	for (size_t i = 0; i < triangleIndices.size(); i += 3)
	{
		unsigned short prev = i > 0 ? triangleIndices[i - 1] : 1;
		triangleIndices[i] = 0;
		triangleIndices[i + 1] = prev;
		triangleIndices[i + 2] = (prev % (colorTriangleVertices.size() - 1)) + 1;
		//TRACE("I[" << i << "] = " << triangleIndices[i] << " I[" << i + 1 << "] = " << triangleIndices[i + 1]
		//	<< " I[" << i + 2 << "] = " << triangleIndices[i + 2] << std::endl;);
	}

	_indexCount = static_cast<unsigned int>(triangleIndices.size());

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = triangleIndices.data();
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned short) * static_cast<UINT>(triangleIndices.size()), D3D11_BIND_INDEX_BUFFER);
	DX::ThrowIfFailed(
		_deviceResources->GetD3DDevice()->CreateBuffer(
			&indexBufferDesc,
			&indexBufferData,
			&_indexBuffer
		)
	);
}
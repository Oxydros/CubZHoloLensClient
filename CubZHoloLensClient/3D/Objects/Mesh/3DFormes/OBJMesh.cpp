#include "pch.h"

#include "3D\Scene\HolographicScene.h"
#include "3D\Utility\DirectXHelper.h"
#include "3D\Objects\Mesh\3DFormes\OBJMesh.h"

using namespace HoloLensClient;
using namespace DirectX;
using namespace Windows::Foundation::Numerics;

OBJMesh::OBJMesh(std::shared_ptr<DX::DeviceResources> devicesResources,
				 std::string const &meshFile,
				 Windows::Foundation::Numerics::float4 color)
	: ColoredObject(devicesResources), _meshFile(meshFile)
{
	SetColor(color);

	std::string err;
	bool ret = tinyobj::LoadObj(&_attrib, &_shapes, &_materials, &err, meshFile.c_str(), Utility::GetDirectory(meshFile).c_str(), true);
	if (!err.empty())
		TRACE("Error while loading " << meshFile << ": " << err);
	if (!ret)
		throw std::runtime_error("Couln't load mesh file");
	TRACE("Mesh file loaded !");
	if (_shapes.size() != 1)
		throw std::runtime_error("OBJ file contain multiple shape, not supported right now");
	for (size_t i = 0; i < _materials.size(); i++)
	{
		TRACE("Material: " << _materials[i].name << " " << _materials[i].diffuse_texname << std::endl);
	}
}

OBJMesh::~OBJMesh()
{
}

void OBJMesh::CreateMesh()
{
	std::vector<VertexPosition> triangleVertices;

	// Wrap in parenthesis to prevent macro expension of min and max
	float min_x, min_y, min_z;
	min_x = min_y = min_z = (std::numeric_limits<float>::max)();
	float max_x, max_y, max_z;
	max_x = max_y = max_z = (std::numeric_limits<float>::min)();

	//Load triangle vertices
	for (size_t i = 0; i < _attrib.vertices.size() / 3; i++)
	{
		VertexPosition v;
		v.pos.x = _attrib.vertices[3 * i];
		v.pos.y = _attrib.vertices[3 * i + 1];
		v.pos.z = _attrib.vertices[3 * i + 2];

		min_x = MIN(min_x, v.pos.x);
		min_y = MIN(min_y, v.pos.y);
		min_z = MIN(min_z, v.pos.z);

		max_x = MAX(max_x, v.pos.x);
		max_y = MAX(max_y, v.pos.y);
		max_z = MAX(max_z, v.pos.z);

		triangleVertices.push_back(v);
		//TRACE("v " << vx << " " << vy << " " << vz << std::endl);
	}

	float width = max_x - min_x;
	float height = max_y - min_y;
	float depht = max_z - min_z;

	float3 maxPoint = { max_x, max_y, max_z };
	float3 minPoint = { min_x, min_y, min_z };
	float3 centerPoint = (maxPoint + minPoint) / 2;

	_boundingBox = BoundingOrientedBox(XMFLOAT3(centerPoint.x, centerPoint.y, centerPoint.z),
										XMFLOAT3(width / 2.0f, height / 2.0f, depht / 2.0f),
										XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	TRACE("For " << _meshFile << " width: " << width << " height: " << height << " depht: " << depht
		<< " maxPoint: " << maxPoint << " minPoint: " << minPoint << " centerPoint: " << centerPoint << std::endl);

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = triangleVertices.data();
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	const CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(VertexPosition) * static_cast<UINT>(triangleVertices.size()), D3D11_BIND_VERTEX_BUFFER);
	DX::ThrowIfFailed(
		_deviceResources->GetD3DDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			&_vertexBuffer
		)
	);

	std::vector<unsigned short> indices;

	//Load vertex indices
	for (size_t f = 0; f < _shapes[0].mesh.indices.size() / 3; f++) {
		tinyobj::index_t idx0 = _shapes[0].mesh.indices[3 * f];
		tinyobj::index_t idx1 = _shapes[0].mesh.indices[3 * f + 1];
		tinyobj::index_t idx2 = _shapes[0].mesh.indices[3 * f + 2];

		indices.push_back(idx0.vertex_index);
		indices.push_back(idx1.vertex_index);
		indices.push_back(idx2.vertex_index);
		/*TRACE("f " << idx0.vertex_index + 1 << " " << idx1.vertex_index + 1 << " " << idx2.vertex_index + 1 << std::endl);*/
	}

	_indexCount = static_cast<unsigned int>(indices.size());

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = indices.data();
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned short) * static_cast<UINT>(indices.size()), D3D11_BIND_INDEX_BUFFER);
	DX::ThrowIfFailed(
		_deviceResources->GetD3DDevice()->CreateBuffer(
			&indexBufferDesc,
			&indexBufferData,
			&_indexBuffer
		)
	);
}
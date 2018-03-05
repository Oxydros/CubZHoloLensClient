#include "pch.h"

#include "3D\Scene\HolographicScene.h"
#include "3D\Utility\DirectXHelper.h"
#include "GazeRenderer.h"

using namespace HoloLensClient;
using namespace DirectX;
using namespace Windows::Foundation::Numerics;

GazeRenderer::GazeRenderer(std::shared_ptr<DX::DeviceResources> devicesResources, std::shared_ptr<HolographicScene> sharedScene)
	: ObjectRenderer(devicesResources)
{
	setPosition({ 0.f, 0.f, -2.f });
}

GazeRenderer::~GazeRenderer()
{
}

void HoloLensClient::GazeRenderer::Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState ^pointerState)
{
}

void GazeRenderer::Update(DX::StepTimer const &timer)
{
	//Windows::UI::Input::Spatial::SpatialPointerPose ^pointerPose = _scene->getPointerPose();
	//if (pointerPose != nullptr)
	//{
	//	// Get the gaze direction relative to the given coordinate system.
	//	const float3 headPosition = pointerPose->Head->Position;
	//	const float3 headDirection = pointerPose->Head->ForwardDirection;

	//	// The tag-along hologram follows a point 2.0m in front of the user's gaze direction.
	//	static const float distanceFromUser = 2.0f; // meters
	//	const float3 gazeAtTwoMeters = headPosition + (distanceFromUser * headDirection);

	//	// Lerp the position, to keep the hologram comfortably stable.
	//	//auto lerpedPosition = lerp(getPosition(), gazeAtTwoMeters, dtime * c_lerpRate);

	//	// This will be used as the translation component of the hologram's
	//	// model transform.
	//	setPosition(gazeAtTwoMeters);
	//}
	////Position of gaze
	//const XMMATRIX modelTranslation = XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&getPosition()));

	//// Create a direction normal from the hologram's position to the origin of person space.
	//// This is the z-axis rotation.
	//XMVECTOR facingNormal = XMVector3Normalize(-XMLoadFloat3(&getPosition()));

	//// Rotate the x-axis around the y-axis.
	//// This is a 90-degree angle from the normal, in the xz-plane.
	//// This is the x-axis rotation.
	//XMVECTOR xAxisRotation = XMVector3Normalize(XMVectorSet(XMVectorGetZ(facingNormal), 0.f, -XMVectorGetX(facingNormal), 0.f));

	//// Create a third normal to satisfy the conditions of a rotation matrix.
	//// The cross product  of the other two normals is at a 90-degree angle to
	//// both normals. (Normalize the cross product to avoid floating-point math
	//// errors.)
	//// Note how the cross product will never be a zero-matrix because the two normals
	//// are always at a 90-degree angle from one another.
	//XMVECTOR yAxisRotation = XMVector3Normalize(XMVector3Cross(facingNormal, xAxisRotation));

	//// Construct the 4x4 rotation matrix.

	//// Rotate the quad to face the user.
	//XMMATRIX rotationMatrix = XMMATRIX(
	//	xAxisRotation,
	//	yAxisRotation,
	//	facingNormal,
	//	XMVectorSet(0.f, 0.f, 0.f, 1.f)
	//);

	//// The view and projection matrices are provided by the system; they are associated
	//// with holographic cameras, and updated on a per-camera basis.
	//// Here, we provide the model transform for the sample hologram. The model transform
	//// matrix is transposed to prepare it for the shader.
	//XMStoreFloat4x4(&_modelConstantBufferData.model, XMMatrixTranspose(rotationMatrix * modelTranslation));

	//// Loading is asynchronous. Resources must be created before they can be updated.
	//if (!_loadingComplete)
	//{
	//	return;
	//}

	//// Use the D3D device context to update Direct3D device-based resources.
	//const auto context = _deviceResources->GetD3DDeviceContext();

	//// Update the model transform buffer for the hologram.
	//context->UpdateSubresource(
	//	_modelConstantBuffer.Get(),
	//	0,
	//	nullptr,
	//	&_modelConstantBufferData,
	//	0,
	//	0
	//);
}

void GazeRenderer::CreateMesh()
{
	static const std::array<VertexPositionColor, 8> triangleVertices =
	{ {
		{ XMFLOAT3(-0.05f, -0.05f, -0.05f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-0.05f, -0.05f,  0.05f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.05f,  0.05f, -0.05f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(-0.05f,  0.05f,  0.05f), XMFLOAT3(0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(0.05f, -0.05f, -0.05f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(0.05f, -0.05f,  0.05f), XMFLOAT3(1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.05f,  0.05f, -0.05f), XMFLOAT3(1.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(0.05f,  0.05f,  0.05f), XMFLOAT3(1.0f, 1.0f, 1.0f) },
		} };

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = triangleVertices.data();
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	const CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(VertexPositionColor) * static_cast<UINT>(triangleVertices.size()), D3D11_BIND_VERTEX_BUFFER);
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
	constexpr std::array<unsigned short, 36> triangleIndices =
	{ {
			2,1,0, // -x
			2,3,1,

			6,4,5, // +x
			6,5,7,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		} };

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
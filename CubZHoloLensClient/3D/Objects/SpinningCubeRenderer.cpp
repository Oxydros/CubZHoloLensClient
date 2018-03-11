#include "pch.h"
#include "SpinningCubeRenderer.h"
#include "3D\Scene\HolographicScene.h"
#include "3D\Utility\DirectXHelper.h"

using namespace HoloLensClient;
using namespace Concurrency;
using namespace DirectX;
using namespace Windows::Foundation::Numerics;
using namespace Windows::UI::Input::Spatial;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
SpinningCubeRenderer::SpinningCubeRenderer(std::shared_ptr<DX::DeviceResources> &deviceResources)
	: SceneObject(deviceResources)
{
	SetPosition({ 0.f, 0.f, -2.f });
}
//
//void HoloLensClient::SpinningCubeRenderer::Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState ^pointerState)
//{
//	//PositionHologram(
//	//	pointerState->TryGetPointerPose(_scene->getCoordinateSystem())
//	//);
//}

// This function uses a SpatialPointerPose to position the world-locked hologram
// two meters in front of the user's heading.
void SpinningCubeRenderer::PositionHologram(SpatialPointerPose^ pointerPose)
{
	if (pointerPose != nullptr)
	{
		// Get the gaze direction relative to the given coordinate system.
		const float3 headPosition = pointerPose->Head->Position;
		const float3 headDirection = pointerPose->Head->ForwardDirection;

		// The hologram is positioned two meters along the user's gaze direction.
		constexpr float distanceFromUser = 2.0f; // meters
		const float3 gazeAtTwoMeters = headPosition + (distanceFromUser * headDirection);

		// This will be used as the translation component of the hologram's
		// model transform.
		SetPosition(gazeAtTwoMeters);
	}
}
//
//// Called once per frame. Rotates the cube, and calculates and sets the model matrix
//// relative to the position transform indicated by hologramPositionTransform.
//void SpinningCubeRenderer::Update(const DX::StepTimer& timer)
//{
//	//// Rotate the cube.
//	//// Convert degrees to radians, then convert seconds to rotation angle.
//	//const float    radiansPerSecond = XMConvertToRadians(m_degreesPerSecond);
//	//const double   totalRotation = timer.GetTotalSeconds() * radiansPerSecond;
//	//const float    radians = static_cast<float>(fmod(totalRotation, XM_2PI));
//	//const XMMATRIX modelRotation = XMMatrixRotationY(-radians);
//
//	//// Position the cube.
//	//const XMMATRIX modelTranslation = XMMatrixTranslationFromVector(XMLoadFloat3(&getPosition()));
//
//	//// Multiply to get the transform matrix.
//	//// Note that this transform does not enforce a particular coordinate system. The calling
//	//// class is responsible for rendering this content in a consistent manner.
//	//const XMMATRIX modelTransform = XMMatrixMultiply(modelRotation, modelTranslation);
//
//	//// The view and projection matrices are provided by the system; they are associated
//	//// with holographic cameras, and updated on a per-camera basis.
//	//// Here, we provide the model transform for the sample hologram. The model transform
//	//// matrix is transposed to prepare it for the shader.
//	//XMStoreFloat4x4(&m_modelConstantBufferData.model, XMMatrixTranspose(modelTransform));
//
//	//// Loading is asynchronous. Resources must be created before they can be updated.
//	//if (!m_loadingComplete)
//	//{
//	//	return;
//	//}
//
//	//// Use the D3D device context to update Direct3D device-based resources.
//	//const auto context = _deviceResources->GetD3DDeviceContext();
//
//	//// Update the model transform buffer for the hologram.
//	//context->UpdateSubresource(
//	//	m_modelConstantBuffer.Get(),
//	//	0,
//	//	nullptr,
//	//	&m_modelConstantBufferData,
//	//	0,
//	//	0
//	//);
//}

void SpinningCubeRenderer::CreateMesh()
{

	// Load mesh vertices. Each vertex has a position and a color.
	// Note that the cube size has changed from the default DirectX app
	// template. Windows Holographic is scaled in meters, so to draw the
	// cube at a comfortable size we made the cube width 0.2 m (20 cm).
	static const std::array<VertexPositionColor, 8> cubeVertices =
	{ {
		{ XMFLOAT3(-0.1f, -0.1f, -0.1f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-0.1f, -0.1f,  0.1f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.1f,  0.1f, -0.1f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(-0.1f,  0.1f,  0.1f), XMFLOAT3(0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(0.1f, -0.1f, -0.1f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(0.1f, -0.1f,  0.1f), XMFLOAT3(1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.1f,  0.1f, -0.1f), XMFLOAT3(1.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(0.1f,  0.1f,  0.1f), XMFLOAT3(1.0f, 1.0f, 1.0f) },
		} };

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = cubeVertices.data();
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	const CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(VertexPositionColor) * static_cast<UINT>(cubeVertices.size()), D3D11_BIND_VERTEX_BUFFER);
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
	constexpr std::array<unsigned short, 36> cubeIndices =
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

	_indexCount = static_cast<unsigned int>(cubeIndices.size());

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = cubeIndices.data();
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned short) * static_cast<UINT>(cubeIndices.size()), D3D11_BIND_INDEX_BUFFER);
	DX::ThrowIfFailed(
		_deviceResources->GetD3DDevice()->CreateBuffer(
			&indexBufferDesc,
			&indexBufferData,
			&_indexBuffer
		)
	);
}
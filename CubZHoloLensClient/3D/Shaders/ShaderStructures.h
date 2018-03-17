#pragma once

namespace HoloLensClient
{
	struct ColorModelConstantBuffer
	{
		DirectX::XMFLOAT4X4 model;
		DirectX::XMFLOAT4	color;
	};

	// Assert that the constant buffer remains 16-byte aligned (best practice).
	static_assert((sizeof(ColorModelConstantBuffer) % (sizeof(float) * 4)) == 0, "Color Model constant buffer size must be 16-byte aligned (16 bytes is the length of four floats).");


    // Used to send per-vertex data to the vertex shader.
	struct VertexPosition
	{
		DirectX::XMFLOAT3 pos;
	};

	struct VertexPositionTexture
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};
}
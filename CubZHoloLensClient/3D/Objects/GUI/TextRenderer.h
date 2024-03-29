//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#pragma once

#define TEXT_HD_SIZE	(2048)

#include <memory>
#include <3D\Resources\DeviceResources.h>

namespace HoloLensClient
{
	class TextRenderer
	{
	private:
		// Cached pointer to device resources.
		const std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// Direct3D resources for rendering text to an off-screen render target.
		Microsoft::WRL::ComPtr<ID3D11Texture2D>             m_texture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_shaderResourceView;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>          m_pointSampler;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>      m_renderTargetView;
		Microsoft::WRL::ComPtr<ID2D1RenderTarget>           m_d2dRenderTarget;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>        m_whiteBrush;
		Microsoft::WRL::ComPtr<IDWriteTextFormat>           m_textFormat;

		// CPU-based variables for configuring the offscreen render target.
		float												_x;
		float												_y;

	public:
		TextRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources, float x, float y);

		void RenderTextOffscreen(const std::wstring& str, float fontSize);

		void CreateDeviceDependentResources();
		void ReleaseDeviceDependentResources();

		ID3D11ShaderResourceView* GetTexture() const { return m_shaderResourceView.Get(); };
		ID3D11SamplerState*       GetSampler() const { return m_pointSampler.Get(); };
	};
}
﻿#pragma once

#include <pch.h>
#include <ppltasks.h>    // For create_task
#include <vector>

namespace DX
{
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            // Set a breakpoint on this line to catch Win32 API errors.
            throw Platform::Exception::CreateException(hr);
        }
    }

    // Function that reads from a binary file asynchronously.
    inline Concurrency::task<std::vector<byte>> ReadDataAsync(const std::wstring& filename)
    {
        using namespace Windows::Storage;
        using namespace Concurrency;

		auto s = Platform::StringReference(filename.c_str());
        return create_task(PathIO::ReadBufferAsync(s)).then(
            [] (Streams::IBuffer^ fileBuffer) -> std::vector<byte>
            {
                std::vector<byte> returnBuffer;
                returnBuffer.resize(fileBuffer->Length);
                Streams::DataReader::FromBuffer(fileBuffer)->ReadBytes(Platform::ArrayReference<byte>(returnBuffer.data(), returnBuffer.size()));
                return returnBuffer;
            });
    }

	// Function that reads from a text file asynchronously.
	inline Concurrency::task<std::string> ReadTextFileAsync(const std::wstring& filename)
	{
		using namespace Windows::Storage;
		using namespace Concurrency;

		auto s = Platform::StringReference(filename.c_str());

		return create_task(PathIO::ReadTextAsync(s)).then(
			[](Platform::String ^text) -> std::string
		{
			TRACE("GOT IT !" << std::endl);
			std::wstring fooW(text->Begin());
			std::string fooA(fooW.begin(), fooW.end());
			return fooA;
		});
	}

    // Converts a length in device-independent pixels (DIPs) to a length in physical pixels.
    inline float ConvertDipsToPixels(float dips, float dpi)
    {
        constexpr float dipsPerInch = 96.0f;
        return floorf(dips * dpi / dipsPerInch + 0.5f); // Round to nearest integer.
    }

	HRESULT CreateWICTextureFromMemoryEx(ID3D11Device* d3dDevice,
		const uint8_t* wicData,
		size_t wicDataSize,
		size_t maxsize,
		D3D11_USAGE usage,
		unsigned int bindFlags,
		unsigned int cpuAccessFlags,
		unsigned int miscFlags,
		bool forceSRGB,
		ID3D11Resource** texture,
		ID3D11ShaderResourceView** textureView);

	HRESULT CreateWICTextureFromMemory(ID3D11Device* d3dDevice,
		const uint8_t* wicData,
		size_t wicDataSize,
		ID3D11Resource** texture,
		ID3D11ShaderResourceView** textureView,
		size_t maxsize = 0);

#if defined(_DEBUG)
    // Check for SDK Layer support.
    inline bool SdkLayersAvailable()
    {
        HRESULT hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
            0,
            D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
            nullptr,                    // Any feature level will do.
            0,
            D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
            nullptr,                    // No need to keep the D3D device reference.
            nullptr,                    // No need to know the feature level.
            nullptr                     // No need to keep the D3D device context reference.
            );

        return SUCCEEDED(hr);
    }
#endif

	// Triangulates a 2D shape, such as the spatial stage movement bounds.
	// This function expects a set of vertices that define the boundaries of a shape, in
	// clockwise order.
	std::vector<unsigned short> TriangulatePoints2DShape(std::vector<Windows::Foundation::Numerics::float3> const& vertices);
}

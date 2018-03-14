#pragma once

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

        return create_task(PathIO::ReadBufferAsync(Platform::StringReference(filename.c_str()))).then(
            [] (Streams::IBuffer^ fileBuffer) -> std::vector<byte>
            {
                std::vector<byte> returnBuffer;
                returnBuffer.resize(fileBuffer->Length);
                Streams::DataReader::FromBuffer(fileBuffer)->ReadBytes(Platform::ArrayReference<byte>(returnBuffer.data(), static_cast<unsigned int>(returnBuffer.size())));
                return returnBuffer;
            });
    }

    // Converts a length in device-independent pixels (DIPs) to a length in physical pixels.
    inline float ConvertDipsToPixels(float dips, float dpi)
    {
        constexpr float dipsPerInch = 96.0f;
        return floorf(dips * dpi / dipsPerInch + 0.5f); // Round to nearest integer.
    }

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
	inline std::vector<unsigned short> TriangulatePoints2DShape(std::vector<Windows::Foundation::Numerics::float3> const& vertices)
	{
		size_t const& vertexCount = vertices.size();

		// Segments of the shape are removed as they are triangularized.
		std::vector<bool> vertexRemoved;
		vertexRemoved.resize(vertexCount, false);
		unsigned int vertexRemovedCount = 0;

		// Indices are used to define triangles.
		std::vector<unsigned short> indices;

		// Decompose into convex segments.
		unsigned short currentVertex = 0;
		while (vertexRemovedCount < (vertexCount - 2))
		{
			// Get next triangle:
			// Start with the current vertex.
			unsigned short index1 = currentVertex;

			// Get the next available vertex.
			unsigned short index2 = index1 + 1;

			// This cycles to the next available index.
			auto CycleIndex = [=](unsigned short indexToCycle, unsigned short stopIndex)
			{
				// Make sure the index does not exceed bounds.
				if (indexToCycle >= unsigned short(vertexCount))
				{
					indexToCycle -= unsigned short(vertexCount);
				}

				while (vertexRemoved[indexToCycle])
				{
					// If the vertex is removed, go to the next available one.
					++indexToCycle;

					// Make sure the index does not exceed bounds.
					if (indexToCycle >= unsigned short(vertexCount))
					{
						indexToCycle -= unsigned short(vertexCount);
					}

					// Prevent cycling all the way around.
					// Should not be needed, as we limit with the vertex count.
					if (indexToCycle == stopIndex)
					{
						break;
					}
				}

				return indexToCycle;
			};
			index2 = CycleIndex(index2, index1);

			// Get the next available vertex after that.
			unsigned short index3 = index2 + 1;
			index3 = CycleIndex(index3, index1);

			// Vertices that may define a triangle inside the 2D shape.
			auto& v1 = vertices[index1];
			auto& v2 = vertices[index2];
			auto& v3 = vertices[index3];

			// If the projection of the first segment (in clockwise order) onto the second segment is 
			// positive, we know that the clockwise angle is less than 180 degrees, which tells us 
			// that the triangle formed by the two segments is contained within the bounding shape.
			auto v2ToV1 = v1 - v2;
			auto v2ToV3 = v3 - v2;
			Windows::Foundation::Numerics::float3 normalToV2ToV3 = { -v2ToV3.z, 0.f, v2ToV3.x };
			float projectionOntoNormal = dot(v2ToV1, normalToV2ToV3);
			if (projectionOntoNormal >= 0)
			{
				// Triangle is contained within the 2D shape.

				// Remove peak vertex from the list.
				vertexRemoved[index2] = true;
				++vertexRemovedCount;

				// Create the triangle.
				indices.push_back(index1);
				indices.push_back(index2);
				indices.push_back(index3);

				// Continue on to the next outer triangle.
				currentVertex = index3;
			}
			else
			{
				// Triangle is a cavity in the 2D shape.
				// The next triangle starts at the inside corner.
				currentVertex = index2;
			}
		}

		indices.shrink_to_fit();
		return indices;
	}
}

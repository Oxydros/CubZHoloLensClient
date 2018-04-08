#pragma once

#include <3D\Objects\Common\IObject.h>

using namespace Windows::Foundation::Numerics;

namespace HoloLensClient
{
	// Define the properties of a colored object
	class IColoredObject : public virtual IObject
	{
	public:
		virtual ~IColoredObject() = default;

	public:
		// Set the color of the object
		virtual void SetColor(float4 color) = 0;

		// Get the color of the object
		virtual float4 GetColor() const = 0;
	};
}
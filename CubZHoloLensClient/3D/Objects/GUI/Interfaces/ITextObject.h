#pragma once

#include <3D\Objects\Common\IObject.h>

namespace HoloLensClient
{
	class ITextObject : public virtual IObject
	{
	public:
		virtual ~ITextObject() = default;

	public:
		virtual std::wstring const &getText() const = 0;
		virtual void setText(std::wstring const &text) = 0;

		virtual float const getFontSize() const = 0;
		virtual void setFontSize(float font) = 0;

		virtual void setColor(Windows::Foundation::Numerics::float4) = 0;
		virtual Windows::Foundation::Numerics::float4 getColor() const = 0;
	};
}
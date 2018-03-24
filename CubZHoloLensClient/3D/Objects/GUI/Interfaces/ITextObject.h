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
	};
}
#pragma once

#include <3D\Objects\Interfaces\IObject.h>

namespace HoloLensClient
{
	class ITextObject : public IObject
	{
	public:
		virtual ~ITextObject() = default;

	public:
		virtual Platform::String ^getText() const = 0;
		virtual void setText(Platform::String ^text) = 0;
	};
}
#pragma once

#include <3D\Resources\Texture2D.h>
#include <3D\Objects\Mesh\Interfaces\IColoredObject.h>

namespace HoloLensClient
{
	class ITexturedObject : public IColoredObject
	{
	public:
		virtual ~ITexturedObject() = default;

	public:
		virtual void SetTexture(std::shared_ptr<Texture2D> &texture) = 0;
		virtual std::shared_ptr<Texture2D> getTexture() const = 0;
	};
}
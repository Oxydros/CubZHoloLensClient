#pragma once

#include <3D\Objects\ICollidableObject.h>
#include <3D\Objects\SceneObject.h>

namespace HoloLensClient
{
	class CollidableSceneObject : public virtual ICollidableObject, public SceneObject
	{
	protected:
		DirectX::BoundingOrientedBox            _boundingBox;

	public:
		CollidableSceneObject(std::shared_ptr<DX::DeviceResources> &devicesResources);
		virtual ~CollidableSceneObject();

	public:
		void GetBoundingBox(DirectX::BoundingOrientedBox &boundingBox) override;
	};
}
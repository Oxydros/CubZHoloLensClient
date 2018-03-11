#include <pch.h>
#include <3D\Objects\CollidableSceneObject.h>

using namespace HoloLensClient;

CollidableSceneObject::CollidableSceneObject(std::shared_ptr<DX::DeviceResources>& devicesResources)
	: SceneObject(devicesResources)
{
}

CollidableSceneObject::~CollidableSceneObject()
{
}

void CollidableSceneObject::GetBoundingBox(DirectX::BoundingOrientedBox &boundingBox)
{
	throw std::runtime_error("Function not yet available");

	//_boundingBox.Transform(boundingBox, DirectX::XMLoadFloat4x4(&_transform));
}

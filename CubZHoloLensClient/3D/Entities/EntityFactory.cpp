#include <pch.h>
#include <3D\Entities\EntityFactory.h>
#include <3D\Entities\CubeEntity.h>

using namespace HoloLensClient;

HoloLensClient::EntityFactory::EntityFactory(std::shared_ptr<DX::DeviceResources> deviceResources,
	std::shared_ptr<HolographicScene> scene)
	: _deviceResources(deviceResources), _scene(scene)
{
}

IEntity::IEntityPtr HoloLensClient::EntityFactory::createEntity(CubZHoloLensClient::WinNetwork::EntityDescription const &entityDesc)
{
	if (entityDesc.type == CubZHoloLensClient::WinNetwork::EntityType::CUBE)
		return (createCube(entityDesc));
	return (createCube(entityDesc));
}

IEntity::IEntityPtr HoloLensClient::EntityFactory::createCube(CubZHoloLensClient::WinNetwork::EntityDescription const &entityDesc)
{
	auto cube = std::make_unique<CubeEntity>(_deviceResources, _scene, entityDesc);

	return (std::move(cube));
}

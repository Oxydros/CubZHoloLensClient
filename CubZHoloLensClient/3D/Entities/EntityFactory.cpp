#include <pch.h>
#include <3D\Entities\EntityFactory.h>
#include <3D\Entities\CubeEntity.h>
#include <3D\Entities\ObjEntity.h>

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
	else if (entityDesc.type == CubZHoloLensClient::WinNetwork::EntityType::SPHERE)
		return (createSphere(entityDesc));
	return (createCube(entityDesc));
}

IEntity::IEntityPtr HoloLensClient::EntityFactory::createCube(CubZHoloLensClient::WinNetwork::EntityDescription const &entityDesc)
{
	auto cube = std::make_unique<ObjEntity>(_deviceResources, _scene, Utility::GetRealInstallPath() + "//Assets//Models//Cube.obj", entityDesc);
	return (std::move(cube));
}

IEntity::IEntityPtr HoloLensClient::EntityFactory::createSphere(CubZHoloLensClient::WinNetwork::EntityDescription const & entityDesc)
{
	auto sphere = std::make_unique<ObjEntity>(_deviceResources, _scene, Utility::GetRealInstallPath() + "//Assets//Models//Sphere.obj", entityDesc);
	return (std::move(sphere));
}

IEntity::IEntityPtr HoloLensClient::EntityFactory::createFromFile(std::string const & fileName, CubZHoloLensClient::WinNetwork::EntityDescription const & entityDesc)
{
	return nullptr;
}

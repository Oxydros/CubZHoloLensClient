#pragma once

#include "Network\Objects\TypeDescription.h"
#include "3D\Entities\Common\IEntity.h"

namespace DX
{
	class DeviceResources;
}

namespace HoloLensClient
{
	class HolographicScene;

	class EntityFactory
	{
	private:
		std::shared_ptr<DX::DeviceResources>	_deviceResources;
		std::shared_ptr<HolographicScene>		_scene;
	public:
		EntityFactory(std::shared_ptr<DX::DeviceResources> deviceResources, std::shared_ptr<HolographicScene>);
		~EntityFactory() = default;

	public:
		IEntity::IEntityPtr	createEntity(CubZHoloLensClient::WinNetwork::EntityDescription const &entityDesc);
		IEntity::IEntityPtr	createCube(CubZHoloLensClient::WinNetwork::EntityDescription const &entityDesc);
		IEntity::IEntityPtr	createSphere(CubZHoloLensClient::WinNetwork::EntityDescription const &entityDesc);
		IEntity::IEntityPtr	createFromFile(std::string const &fileName,
											CubZHoloLensClient::WinNetwork::EntityDescription const &entityDesc);
	};
}
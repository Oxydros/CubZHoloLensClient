#include "pch.h"
#include "HolographicScene.h"
#include "3D\Entities\GazeEntity.h"

using namespace HoloLensClient;

HolographicScene::HolographicScene(std::shared_ptr<DX::DeviceResources> deviceResources)
	: _deviceResources(deviceResources)
{
}


HolographicScene::~HolographicScene()
{
}

void HolographicScene::Initialize()
{
	auto safe = shared_from_this();
	auto gaze = std::make_unique<GazeEntity>(_deviceResources, safe);
	_entities.emplace_back(std::move(gaze));
}

void HolographicScene::Update(DX::StepTimer const& timer)
{
	std::for_each(_entities.begin(), _entities.end(),
		[&timer](auto &entity)
	{
		std::cout << "Updating " << entity.get() << std::endl;
		entity->Update(timer);
	});
	std::for_each(_newEntities.begin(), _newEntities.end(),
		[this](auto &entity)
	{
		TRACE("Adding new entity from pending list " << entity.get() << std::endl);
		_entities.emplace_back(std::move(entity));
	});
	_newEntities.clear();
}

void HolographicScene::Render()
{
	std::for_each(_entities.begin(), _entities.end(),
		[](auto &entity)
	{
		TRACE("Rendering " << entity.get() << std::endl);
		entity->Render();
	});
}

void HoloLensClient::HolographicScene::UpdateCoordinateSystem(Windows::Perception::Spatial::SpatialCoordinateSystem ^coordinateSystem)
{
	_coordinateSystem = coordinateSystem;
}

void HoloLensClient::HolographicScene::UpdatePointerPose(Windows::UI::Input::Spatial::SpatialPointerPose ^ pointerPose)
{
	_pointerPose = pointerPose;
}

void HoloLensClient::HolographicScene::Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState)
{
	std::for_each(_entities.begin(), _entities.end(),
		[&pointerState](auto &entity)
	{
		TRACE("Entity = " << entity.get() << " " << std::endl);
		entity->Inputs(pointerState);
	});
}

void HolographicScene::OnDeviceLost()
{
	std::for_each(_entities.begin(), _entities.end(),
		[](auto &entity)
	{
		entity->ReleaseMesh();
	});
}

void HolographicScene::OnDeviceRestored()
{
	std::for_each(_entities.begin(), _entities.end(),
		[](auto &entity)
	{
		entity->InitializeMesh();
	});
}

void HoloLensClient::HolographicScene::addEntity(std::unique_ptr<Entity> e)
{
	TRACE("Adding new entity to pending list " << e.get() << std::endl);
	_newEntities.emplace_back(std::move(e));
}

Windows::Perception::Spatial::SpatialCoordinateSystem ^ HoloLensClient::HolographicScene::getCoordinateSystem() const
{
	return (_coordinateSystem);
}

Windows::UI::Input::Spatial::SpatialPointerPose ^ HoloLensClient::HolographicScene::getPointerPose() const
{
	return (_pointerPose);
}

#include "pch.h"
#include "HolographicScene.h"
#include "3D\Entities\CursorEntity.h"
#include "3D\Entities\GUI\Button.h"

using namespace HoloLensClient;

HolographicScene::HolographicScene(std::shared_ptr<DX::DeviceResources> &deviceResources)
	: _deviceResources(deviceResources)
{
}


HolographicScene::~HolographicScene()
{
}

void HolographicScene::Initialize()
{
	auto safe = shared_from_this();
	auto gaze = std::make_unique<CursorEntity>(_deviceResources, safe);
	addEntity(std::move(gaze));
	auto button1 = std::make_unique<Button>(_deviceResources, safe);
	auto button2 = std::make_unique<Button>(_deviceResources, safe);
	button1->setFollowGaze(true, false, { 0.0f, 0.0f, 4.5f });
	button1->AddChild(button2.get());
	button2->SetRelativePosition({-0.5f, 0.0f, 0.0f});
	TRACE("Button 1 is " << button1.get() << " Button 2 is " << button2.get() << std::endl);
	addEntity(std::move(button1));
	addEntity(std::move(button2));
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
		//TRACE("Adding new entity from pending list " << entity.get() << std::endl);
		_entities.emplace_back(std::move(entity));
	});
	_newEntities.clear();
	_entities.erase(
		std::remove_if(_entities.begin(), _entities.end(),
			[](auto &entity) {
				return entity->isDead();
			}),
		_entities.end());
}

void HolographicScene::Render()
{
	std::for_each(_entities.begin(), _entities.end(),
		[](auto &entity)
	{
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
		//TRACE("Entity = " << entity.get() << " " << std::endl);
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

void HoloLensClient::HolographicScene::addEntity(IEntity::IEntityPtr e)
{
	//TRACE("Adding new entity to pending list " << e.get() << std::endl);
	_newEntities.emplace_back(std::move(e));
}

void HoloLensClient::HolographicScene::addEntityInFront(IEntity::IEntityPtr e, float dist)
{
	if (_pointerPose != nullptr)
	{
		// Get the gaze direction relative to the given coordinate system.
		const Windows::Foundation::Numerics::float3 headPosition = _pointerPose->Head->Position;
		const Windows::Foundation::Numerics::float3 headDirection = _pointerPose->Head->ForwardDirection;

		// The tag-along hologram follows a point 2.0m in front of the user's gaze direction.
		const Windows::Foundation::Numerics::float3 gazeAtTwoMeters = headPosition + (dist * headDirection);

		e->SetRealPosition(gazeAtTwoMeters);
		e->SetRealRotation({ 0, 0, 0 });
		addEntity(std::move(e));
	}
}

Windows::Perception::Spatial::SpatialCoordinateSystem ^ HoloLensClient::HolographicScene::getCoordinateSystem() const
{
	return (_coordinateSystem);
}

Windows::UI::Input::Spatial::SpatialPointerPose ^ HoloLensClient::HolographicScene::getPointerPose() const
{
	return (_pointerPose);
}

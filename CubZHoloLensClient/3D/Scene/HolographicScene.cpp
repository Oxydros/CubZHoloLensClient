#include "pch.h"
#include "HolographicScene.h"
#include "3D\Entities\CursorEntity.h"
#include "3D\Entities\CubeEntity.h"
#include "3D\Entities\GUI\Button.h"
#include "3D\Entities\GUI\Panel.h"
#include "3D\Loaders\OBJLoader.h"

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

	//auto panel = std::make_unique<Panel>(_deviceResources, safe, float2(0.45f, 0.35f), float4(0.7f, 0.1f, 0.2f, 0.6f));
	///*panel->setFollowGaze(true, true, { -0.2f, 0,  2.0f});*/
	//panel->SetRelativePosition({ 0.0f, 0.0f, -3.0f });

	//auto button1 = std::make_unique<Button>(_deviceResources, safe,
	//	[]() {
	//		TRACE("Got click on button" << std::endl);
	//	},
	//	float2(0.15f, 0.1f));
	//button1->SetRelativePosition({-0.1f, 0.1f, 0.1f});
	//button1->setLabel(L"Button 1");

	//auto button2 = std::make_unique<Button>(_deviceResources, safe, nullptr, float2(0.15f, 0.1f));
	//button2->SetRelativePosition({ -0.1f, -0.1f, 0.1f });
	//button2->setLabel(L"Button 2");

	//panel->AddChild(button1.get());
	//panel->AddChild(button2.get());

	//addEntity(std::move(panel));
	//addEntity(std::move(button1));
	//addEntity(std::move(button2));
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

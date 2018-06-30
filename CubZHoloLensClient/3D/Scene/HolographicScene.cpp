#include "pch.h"
#include "HolographicScene.h"
#include <Objects\HoloLensContext.h>
#include "3D\Entities\CursorEntity.h"
#include "3D\Entities\CubeEntity.h"
#include "3D\Entities\ObjEntity.h"
#include "3D\Loaders\OBJLoader.h"

using namespace HoloLensClient;

HolographicScene::HolographicScene(std::shared_ptr<DX::DeviceResources> &deviceResources)
	: _deviceResources(deviceResources)
{
	_updateEntityEventToken =
		CubZHoloLensClient::HoloLensContext::Instance()->getUDPClient()->EntityUpdated +=
		ref new CubZHoloLensClient::WinNetwork::EntityUpdateEvent(
			std::bind(&HolographicScene::UpdateEntity, this,
				std::placeholders::_1, std::placeholders::_2));

	_entityEventToken =
		CubZHoloLensClient::HoloLensContext::Instance()->getTCPClient()->EntityEvent +=
		ref new CubZHoloLensClient::WinNetwork::EntityEvent(
			std::bind(&HolographicScene::CreateDeleteEntity, this,
				std::placeholders::_1, std::placeholders::_2));
}

HolographicScene::~HolographicScene()
{
	CubZHoloLensClient::HoloLensContext::Instance()->getUDPClient()->EntityUpdated -= _updateEntityEventToken;
	CubZHoloLensClient::HoloLensContext::Instance()->getTCPClient()->EntityEvent -= _entityEventToken;
}

void HolographicScene::Initialize()
{
	auto safeScene{ shared_from_this() };

	_root = std::make_unique<EmptyEntity>(safeScene, "Root", true);

	//Declare gaze
	auto gaze = std::make_unique<CursorEntity>(_deviceResources, safeScene);
	_cursor = gaze.get();
	addEntity(std::move(gaze));

	////Declare basic cube for test purposes
	auto cube = std::make_unique<CubeEntity>(_deviceResources, safeScene);
	cube->SetRelativePosition({ 2.0f, 0.0f, -3.0f });
	addEntity(std::move(cube));

	auto mainMenu = std::make_unique<MainMenu>(_deviceResources, safeScene);
	mainMenu->SetRelativePosition({ 0.0f, 0.0f, -3.0f });

	mainMenu->InitializeMenu();
	_mainMenu = mainMenu.get();

	addEntity(std::move(mainMenu));

	auto modificationMenu = std::make_unique<ModificationMenu>(_deviceResources, safeScene);
	modificationMenu->InitializeMenu();
	_modifMenu = modificationMenu.get();

	addEntity(std::move(modificationMenu));

	//auto entityMenu = std::make_unique<EntityMenu>(_deviceResources, safeScene);
	//entityMenu->SetRelativePosition({ 0.0f, -5.0f, -3.0f });

	//entityMenu->InitializeMenu();

	//addEntity(std::move(entityMenu));
}

void HoloLensClient::HolographicScene::InteractionDetectedEvent(Windows::UI::Input::Spatial::SpatialInteractionManager ^sender,
	Windows::UI::Input::Spatial::SpatialInteractionDetectedEventArgs ^ args)
{	
	if (_focusedEntity)
		_focusedEntity->CaptureInteraction(args->Interaction);
}

void HolographicScene::Update(DX::StepTimer const& timer)
{
	auto pair = _root->getNearestInGazeEntity();

	//Focus only nearest entity in gaze direction
	if (pair.second >= 0)
	{
		/*TRACE("Nearest entity is " << pair.first << " " << pair.first->GetLabel() << " " << pair.second << std::endl);*/
		if (_focusedEntity != nullptr && _focusedEntity != pair.first)
			_focusedEntity->setFocus(false);
		pair.first->setFocus(true);
		_focusedEntity = pair.first;
	}
	else if (_focusedEntity != nullptr)
	{
		//No entity found in gaze, remove focus from actual entity
		_focusedEntity->setFocus(false);
		_focusedEntity = nullptr;
	}

	_root->Update(timer);
}

void HolographicScene::Render()
{
	_root->Render();
}

void HoloLensClient::HolographicScene::UpdateCoordinateSystem(Windows::Perception::Spatial::SpatialCoordinateSystem ^coordinateSystem)
{
	_coordinateSystem = coordinateSystem;
}

void HoloLensClient::HolographicScene::UpdatePointerPose(Windows::UI::Input::Spatial::SpatialPointerPose ^ pointerPose)
{
	_pointerPose = pointerPose;
}

void HolographicScene::OnDeviceLost()
{
	_root->ReleaseMesh();
}

void HolographicScene::OnDeviceRestored()
{
	_root->InitializeMesh();
}

void HoloLensClient::HolographicScene::UpdateEntity(CubZHoloLensClient::WinNetwork::EntityDescription entity, CubZHoloLensClient::WinNetwork::SpaceDescription space)
{
	TRACE("Received update on Entity " << entity.id << std::endl);
}

void HoloLensClient::HolographicScene::CreateDeleteEntity(CubZHoloLensClient::WinNetwork::EntityAction action, CubZHoloLensClient::WinNetwork::EntityDescription entity)
{
	TRACE("Received create/delete on Entity " << entity.id << std::endl);
}

void HoloLensClient::HolographicScene::addEntity(IEntity::IEntityPtr e)
{
	//TRACE("Adding new entity to pending list " << e.get() << std::endl);
	if (_root == nullptr)
		std::runtime_error("The root entity is not allocated yet !");
	_root->AddChild(std::move(e));
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

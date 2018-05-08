#include "pch.h"
#include "HolographicScene.h"
#include <Objects\HoloLensContext.h>
#include "3D\Entities\CursorEntity.h"
#include "3D\Entities\CubeEntity.h"
#include "3D\Entities\ObjEntity.h"
#include "3D\Loaders\OBJLoader.h"
#include "3D\Entities\GUI\EntityMenu.h"

using namespace HoloLensClient;

void HoloLensClient::HolographicScene::OnListFile(Windows::Foundation::Collections::IVector<Platform::String^>^ fileList)
{
	TRACE("Got file list");
	auto cube = std::make_unique<ObjEntity>(_deviceResources, shared_from_this(), Utility::GetRealInstallPath() + "\\Assets\\Models\\Special.obj");
	cube->SetRelativePosition({ 2.0f, 0.0f, -3.0f });
	addEntity(std::move(cube));
}

HolographicScene::HolographicScene(std::shared_ptr<DX::DeviceResources> &deviceResources)
	: _deviceResources(deviceResources)
{
}


HolographicScene::~HolographicScene()
{
	CubZHoloLensClient::HoloLensContext::Instance()->getTCPClient()->ListFileEvent -= _listFileToken;
}

void HolographicScene::Initialize()
{
	auto safeScene{ shared_from_this() };

	//Initialize file list actualization
	_listFileToken = CubZHoloLensClient::HoloLensContext::Instance()->getTCPClient()->ListFileEvent +=
		ref new CubZHoloLensClient::WinNetwork::FileListEvent(std::bind(&HoloLensClient::HolographicScene::OnListFile, this, std::placeholders::_1));

	//Update file scene every 3 seconds
	_timeSpanListFile.Duration = 50000000;

	Windows::System::Threading::ThreadPoolTimer::CreatePeriodicTimer(
		ref new Windows::System::Threading::TimerElapsedHandler([](Windows::System::Threading::ThreadPoolTimer^ source)
		{
			CubZHoloLensClient::HoloLensContext::Instance()->getTCPClient()->listServerFiles(".");
		}), _timeSpanListFile);

	_root = std::make_unique<EntityRoot>(safeScene);

	//Declare gaze
	auto gaze = std::make_unique<CursorEntity>(_deviceResources, safeScene);
	_cursor = gaze.get();
	addEntity(std::move(gaze));

	////Declare basic cube for test purposes
	//auto cube = std::make_unique<CubeEntity>(_deviceResources, safeScene);
	//cube->SetRelativePosition({ 2.0f, 0.0f, -3.0f });
	//addEntity(std::move(cube));

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

void HolographicScene::Update(DX::StepTimer const& timer)
{
	_root->Update(timer);

	auto pair = _root->getNearestInGazeEntity();

	//Focus only nearest entity in gaze direction
	if (pair.second >= 0)
	{
		/*TRACE("Nearest entity is " << pair.first << " " << pair.first->GetLabel() << " " << pair.second << std::endl);*/
		if (_previousEntityFocused != nullptr && _previousEntityFocused != pair.first)
			_previousEntityFocused->setFocus(false);
		pair.first->setFocus(true);
		_previousEntityFocused = pair.first;
	}
	else if (_previousEntityFocused != nullptr)
	{
		//No entity found in gaze, remove focus from actual entity
		_previousEntityFocused->setFocus(false);
		_previousEntityFocused = nullptr;
	}
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

void HoloLensClient::HolographicScene::Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState)
{
	_root->Inputs(pointerState);
}

void HolographicScene::OnDeviceLost()
{
	_root->ReleaseMesh();
}

void HolographicScene::OnDeviceRestored()
{
	_root->InitializeMesh();
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

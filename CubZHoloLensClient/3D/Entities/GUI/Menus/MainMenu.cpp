#include "pch.h"
#include <3D\Entities\GUI\Menus\MainMenu.h>
#include <3D\Entities\GUI\Widgets\Panel.h>
#include <3D\Entities\GUI\Widgets\Button2D.h>
#include <3D\Entities\CubeEntity.h>
#include "3D\Scene\HolographicScene.h"
#include "Objects\HoloLensContext.h"

using namespace Windows::UI::Input;

HoloLensClient::MainMenu::MainMenu(std::shared_ptr<DX::DeviceResources> devicesResources,
								   std::shared_ptr<HolographicScene> scene)
	: Entity(scene), _devicesResources(devicesResources)
{
}

HoloLensClient::MainMenu::~MainMenu()
{
}

void HoloLensClient::MainMenu::InitializeMenu()
{
	//Lambda generating a lambda using the given EntityType
	static	auto GenerateEntity = [](CubZHoloLensClient::WinNetwork::EntityType type) {
		return ([type](Spatial::SpatialGestureRecognizer ^, Spatial::SpatialTappedEventArgs ^) {
			CubZHoloLensClient::HoloLensContext::Instance()->getTCPClient()->createEntity({ type, {},	0 });
		});
	};

	auto _background = std::make_unique<Panel>(_devicesResources, _scene, float2(0.65f, 0.35f), float4(0.7f, 0.1f, 0.2f, 0.6f));
	_background->SetRelativePosition({ 0.0f, 0.0f, 0.0f });

	auto buttonCube = std::make_unique<Button2D>(_devicesResources, _scene, float2(0.15f, 0.1f));
	buttonCube->setLabel(L"Spawn cube");

	//Unused setup to help compiler allow further lambda -> callback conversions
	buttonCube->SetAirTapCallback(nullptr);

	buttonCube->SetAirTapCallback(GenerateEntity(CubZHoloLensClient::WinNetwork::EntityType::CUBE));

	auto buttonSphere = std::make_unique<Button2D>(_devicesResources, _scene, float2(0.15f, 0.1f));
	buttonSphere->setLabel(L"Spawn sphere");
	buttonSphere->SetAirTapCallback(GenerateEntity(CubZHoloLensClient::WinNetwork::EntityType::SPHERE));

	auto safeScene{ _scene };

	auto _button2 = std::make_unique<Button2D>(_devicesResources, _scene, float2(0.15f, 0.1f));
	_button2->setLabel(L"Leave 3D");
	_button2->SetAirTapCallback([safeScene](Spatial::SpatialGestureRecognizer ^, Spatial::SpatialTappedEventArgs ^) {
		TRACE("Killing the scene" << std::endl);
		safeScene->kill();
	});

	_background->AddGUIEntity(std::move(buttonCube), { -0.1f, 0.1f });
	_background->AddGUIEntity(std::move(buttonSphere), { 0.1f, 0.1f });
	_background->AddGUIEntity(std::move(_button2), { -0.1f, -0.1f });

	AddChild(std::move(_background));
}

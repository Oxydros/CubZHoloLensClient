#include "pch.h"
#include "MainMenu.h"
#include "3D\Scene\HolographicScene.h"

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
	auto _background = std::make_unique<Panel>(_devicesResources, _scene, float2(0.45f, 0.35f), float4(0.7f, 0.1f, 0.2f, 0.6f));
	/*panel->setFollowGaze(true, true, { -0.2f, 0,  2.0f});*/
	_background->SetRelativePosition({ 0.0f, 0.0f, 0.0f });

	auto _button1 = std::make_unique<Button>(_devicesResources, _scene,
			[]() {
			},
		float2(0.15f, 0.1f));
	_button1->setLabel(L"Test 1");

	auto safeScene{ _scene };

	auto _button2 = std::make_unique<Button>(_devicesResources, _scene,
		[safeScene]() {
		TRACE("Killing the scene" << std::endl);
		safeScene->kill();
	},
		float2(0.15f, 0.1f));
	_button2->setLabel(L"Leave 3D");

	_background->AddGUIEntity(std::move(_button1), { -0.1f, 0.1f });
	_background->AddGUIEntity(std::move(_button2), { -0.1f, -0.1f });

	AddChild(std::move(_background));
}

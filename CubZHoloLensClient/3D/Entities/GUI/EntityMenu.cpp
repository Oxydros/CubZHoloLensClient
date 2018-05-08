#include "pch.h"
#include "EntityMenu.h"
#include "3D\Scene\HolographicScene.h"

HoloLensClient::EntityMenu::EntityMenu(std::shared_ptr<DX::DeviceResources> devicesResources,
	std::shared_ptr<HolographicScene> scene)
	: Entity(scene), _devicesResources(devicesResources)
{
}

HoloLensClient::EntityMenu::~EntityMenu()
{
}

void HoloLensClient::EntityMenu::InitializeMenu()
{
	auto _background = std::make_unique<Panel>(_devicesResources, _scene, float2(0.45f, 0.35f), float4(0.7f, 0.1f, 0.2f, 0.6f));
	_background->SetRelativePosition({ 0.0f, 0.0f, 0.0f });

	auto _button1 = std::make_unique<Button>(_devicesResources, _scene,
		[]() {
	},
		float2(0.15f, 0.1f));
	_button1->SetRelativePosition({ -0.1f, 0.1f, 0.1f });
	_button1->setLabel(L"Test 1");

	auto safeScene{ _scene };

	auto _button2 = std::make_unique<Button>(_devicesResources, _scene,
		[safeScene]() {
		TRACE("Killing the scene" << std::endl);
		safeScene->kill();
	},
		float2(0.15f, 0.1f));
	_button2->SetRelativePosition({ -0.1f, -0.1f, 0.1f });
	_button2->setLabel(L"Leave 3D");

	_background->AddChild(std::move(_button1));

	AddChild(std::move(_background));
}

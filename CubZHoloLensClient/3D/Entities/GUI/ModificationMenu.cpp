#include "pch.h"
#include "ModificationMenu.h"

HoloLensClient::ModificationMenu::ModificationMenu(std::shared_ptr<DX::DeviceResources> devicesResources,
													std::shared_ptr<HolographicScene> scene)
	: Entity(scene), _devicesResources(devicesResources)
{
	setFollowGaze(false, true);
}

HoloLensClient::ModificationMenu::~ModificationMenu()
{
}

void HoloLensClient::ModificationMenu::InitializeMenu()
{
	auto _background = std::make_unique<Panel>(_devicesResources, _scene, float2(0.25f, 0.07f), float4(0.7f, 0.1f, 0.2f, 0.6f));
	_background->SetRelativePosition({ 0.0f, 0.0f, 0.0f });

	auto _button1 = std::make_unique<Button>(_devicesResources, _scene,
		[]() {
		},
		float2(0.09f, 0.07f));
	_button1->SetRelativePosition({ -0.08f, 0.0f, 0.01f });
	_button1->setLabel(L"Rotate");

	auto _button2 = std::make_unique<Button>(_devicesResources, _scene,
		[]() {
		},
		float2(0.09f, 0.07f));
	_button2->SetRelativePosition({ 0.08f, 0.0f, 0.01f });
	_button2->setLabel(L"Move");

	_background->AddChild(std::move(_button1));
	_background->AddChild(std::move(_button2));

	AddChild(std::move(_background));
}

#include "pch.h"
#include "ModificationMenu.h"

HoloLensClient::ModificationMenu::ModificationMenu(std::shared_ptr<DX::DeviceResources> devicesResources,
													std::shared_ptr<HolographicScene> scene)
	: Entity(scene), _devicesResources(devicesResources)
{
	setFollowGaze(false, true);
	setVisible(false);
}

HoloLensClient::ModificationMenu::~ModificationMenu()
{
}

void HoloLensClient::ModificationMenu::InitializeMenu()
{
	auto _background = std::make_unique<Panel>(_devicesResources, _scene, float2(0.35f, 0.07f), float4(0.7f, 0.1f, 0.2f, 0.6f));
	_background->SetRelativePosition({ 0.0f, 0.0f, 0.0f });

	auto _button1 = std::make_unique<Button>(_devicesResources, _scene, nullptr, float2(0.045f, 0.07f));
	_button1->setLabel(L"RotL");
	_button1->disableMultipleClick(false);

	auto _button3 = std::make_unique<Button>(_devicesResources, _scene, nullptr, float2(0.045f, 0.07f));
	_button3->setLabel(L"RotR");
	_button3->disableMultipleClick(false);

	auto _button4 = std::make_unique<Button>(_devicesResources, _scene, nullptr, float2(0.045f, 0.07f));
	_button4->setLabel(L"Kill");
	_button4->disableMultipleClick(false);

	auto _button2 = std::make_unique<Button>(_devicesResources, _scene, nullptr, float2(0.09f, 0.07f));
	_button2->setLabel(L"Move");

	_move = _button2.get();
	_rotateLeft = _button1.get();
	_rotateRight = _button3.get();
	_kill = _button4.get();

	_background->AddGUIEntity(std::move(_button1), { -0.14f, 0.0f });
	_background->AddGUIEntity(std::move(_button2), { 0.08f, 0.0f });
	_background->AddGUIEntity(std::move(_button3), { -0.058f, 0.0f });
	_background->AddGUIEntity(std::move(_button4), { -0.01f, 0.0f });

	AddChild(std::move(_background));
}

void HoloLensClient::ModificationMenu::DoUpdate(DX::StepTimer const & timer)
{
	if (_visible && _attachedEntity != nullptr)
	{
		float3 basePosition = _attachedEntity->GetRealPosition();
		float3 offset = {0, 0.25f, 0};

		SetRealPosition(basePosition + offset);
	}
}

void HoloLensClient::ModificationMenu::AttachEntity(EditableEntity *entity)
{
	DetachEntity();
	_attachedEntity = entity;
	_move->setCallback(std::bind(&EditableEntity::OnMoveClick, _attachedEntity));
	_rotateLeft->setCallback(std::bind(&EditableEntity::OnRotateLeftClick, _attachedEntity));
	_rotateRight->setCallback(std::bind(&EditableEntity::OnRotateRightClick, _attachedEntity));
	_kill->setCallback(std::bind(&EditableEntity::OnKillClick, _attachedEntity));
	setVisible(true);
}

void HoloLensClient::ModificationMenu::DetachEntity()
{
	_attachedEntity = nullptr;
	_move->setCallback(nullptr);
	_rotateLeft->setCallback(nullptr);
	_rotateRight->setCallback(nullptr);
	_kill->setCallback(nullptr);
	setVisible(false);
}

#include "pch.h"
#include <3D\Entities\GUI\Button2D.h>
#include <3D\Entities\GUI\Button3D.h>
#include <3D\Entities\GUI\Panel.h>
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
	initializeBasicMenu();
	initializeAdjustMenu();
	initializeRotateMenu();
	initializeAdjustBox();
}

void HoloLensClient::ModificationMenu::DoUpdate(DX::StepTimer const & timer)
{
	//Update pos when moving attached entity to always stay near
	//TODO add as child ?
	if (_visible && _attachedEntity != nullptr)
	{
		float3 basePosition = _attachedEntity->GetRealPosition();
		float3 offset = {0, 0.25f, 0};

		SetRealPosition(basePosition + offset);
	}
}

void HoloLensClient::ModificationMenu::AttachEntity(EditableEntity *entity)
{
	if (_attachedEntity == entity)
		return;
	DetachEntity();
	_attachedEntity = entity;
	_scaleLeft->SetRelativePosition({ -_attachedEntity->GetSize().x - 0.1f, -0.25f, 0.0f });
	_scaleRight->SetRelativePosition({ _attachedEntity->GetSize().x + 0.1f, -0.25f, 0.0f });
	_scaleTop->SetRelativePosition({ 0.0f, -_attachedEntity->GetSize().y - 0.1f - 0.25f, 0.0f });
	_scaleBot->SetRelativePosition({ 0.0f, _attachedEntity->GetSize().y + 0.1f - 0.25f, 0.0f });
	setVisible(true);
	_adjustMenu->setVisible(false);
}

void HoloLensClient::ModificationMenu::DetachEntity()
{
	_attachedEntity = nullptr;
	_mode = BASIC;
	_basicMenu->setVisible(true);
	_adjustMenu->setVisible(false);
	setVisible(false);
}

void HoloLensClient::ModificationMenu::initializeBasicMenu()
{
	//Build basic menu
	auto basicMenu = std::make_unique<EmptyEntity>(_scene, "Menu_Adjust_Remove");

	auto _backgroundBasic = std::make_unique<Panel>(_devicesResources, _scene, float2(0.35f, 0.07f), float4(0.0f, 0.392f, 0.0f, 0.6f));
	_backgroundBasic->SetRelativePosition({ 0.0f, 0.0f, 0.0f });

	auto removeButton = std::make_unique<Button2D>(_devicesResources, _scene,
		[this]() {
		if (_attachedEntity)
			_attachedEntity->OnKillClick();
	}
	, float2(0.12f, 0.07f));
	removeButton->setLabel(L"Remove");

	auto adjustButton = std::make_unique<Button2D>(_devicesResources, _scene,
		[this]() {
		_mode = ADJUST;
		_basicMenu->setVisible(false);
		_adjustMenu->setVisible(true);
		if (_attachedEntity)
			_attachedEntity->setEditableState(EditableEntity::State::ADJUST);
	}, float2(0.12f, 0.07f));
	adjustButton->setLabel(L"Adjust");

	auto rotateButton = std::make_unique<Button2D>(_devicesResources, _scene,
		[this]() {
		_mode = ROTATE;
		_basicMenu->setVisible(false);
		_rotateMenu->setVisible(true);
		_attachedEntity->setEditableState(EditableEntity::State::ROTATE);
	}, float2(0.12f, 0.07f));
	rotateButton->setLabel(L"Rotate");

	_backgroundBasic->AddGUIEntity(std::move(rotateButton), { -0.115f, 0.0f });
	_backgroundBasic->AddGUIEntity(std::move(adjustButton), { -0.015f, 0.0f });
	_backgroundBasic->AddGUIEntity(std::move(removeButton), { 0.12f, 0.0f });

	basicMenu->AddChild(std::move(_backgroundBasic));

	_basicMenu = basicMenu.get();

	AddChild(std::move(basicMenu));
}

void HoloLensClient::ModificationMenu::initializeAdjustMenu()
{
	//Build adjust menu
	auto adjustMenu = std::make_unique<EmptyEntity>(_scene, "Menu_Adjust_Scale_Move");

	auto _backgroundAdjust = std::make_unique<Panel>(_devicesResources, _scene, float2(0.35f, 0.07f), float4(0.0f, 0.392f, 0.0f, 0.6f));
	_backgroundAdjust->SetRelativePosition({ 0.0f, 0.0f, 0.0f });

	auto doneAdjust = std::make_unique<Button2D>(_devicesResources, _scene,
		[this]() {
		_mode = BASIC;
		_basicMenu->setVisible(true);
		_adjustMenu->setVisible(false);
		_attachedEntity->setEditableState(EditableEntity::State::IDLE);
	}, float2(0.12f, 0.07f));
	doneAdjust->setLabel(L"Done");

	_backgroundAdjust->AddGUIEntity(std::move(doneAdjust), { 0.12f, 0.0f });

	adjustMenu->AddChild(std::move(_backgroundAdjust));
	adjustMenu->setVisible(false);

	_adjustMenu = adjustMenu.get();

	AddChild(std::move(adjustMenu));
}

void HoloLensClient::ModificationMenu::initializeRotateMenu()
{
	//Build rotate menu
	auto rotateMenu = std::make_unique<EmptyEntity>(_scene, "Menu_Rotate");

	auto _backgroundRotate = std::make_unique<Panel>(_devicesResources, _scene, float2(0.35f, 0.07f), float4(0.0f, 0.392f, 0.0f, 0.6f));
	_backgroundRotate->SetRelativePosition({ 0.0f, 0.0f, 0.0f });

	auto doneAdjust = std::make_unique<Button2D>(_devicesResources, _scene,
		[this]() {
		_mode = BASIC;
		_basicMenu->setVisible(true);
		_rotateMenu->setVisible(false);
		_attachedEntity->setEditableState(EditableEntity::State::IDLE);
	}, float2(0.12f, 0.07f));
	doneAdjust->setLabel(L"Done");

	_backgroundRotate->AddGUIEntity(std::move(doneAdjust), { 0.12f, 0.0f });

	rotateMenu->AddChild(std::move(_backgroundRotate));
	rotateMenu->setVisible(false);

	_rotateMenu = rotateMenu.get();

	AddChild(std::move(rotateMenu));
}

void HoloLensClient::ModificationMenu::initializeAdjustBox()
{
	auto scaleLeft = std::make_unique<Button3D>(_devicesResources, _scene, 
		[this]() {
		_scaleLeftSelected = !_scaleLeftSelected;
	}
		,float3(0.04f, 0.09f, 0.1f));
	auto scaleRight = std::make_unique<Button3D>(_devicesResources, _scene, 
		[this]() {
		_scaleRigthSelected = !_scaleRigthSelected;
	}
		, float3(0.04f, 0.09f, 0.1f));

	auto scaleTop = std::make_unique<Button3D>(_devicesResources, _scene,
		[this]() {
		_scaleTopSelected = !_scaleTopSelected;
	}
	, float3(0.09f, 0.04f, 0.1f));
	auto scaleBot = std::make_unique<Button3D>(_devicesResources, _scene,
		[this]() {
		_scaleBotSelected = !_scaleBotSelected;
	}
	, float3(0.09f, 0.04f, 0.1f));


	auto callbackXAxis = [this](IEntity* scalePtr, float3, float3 directionMotion) {
		if (_scaleLeftSelected) {
			auto realPos = scalePtr->GetRealPosition();
			realPos.x += directionMotion.x;
			scalePtr->SetRealPosition(realPos);
		}
	};

	scaleLeft->setMotionCallback(callbackXAxis);
	scaleRight->setMotionCallback(callbackXAxis);

	auto callbackYAxis = [this](IEntity* scalePtr, float3, float3 directionMotion) {
		if (_scaleLeftSelected) {
			auto realPos = scalePtr->GetRealPosition();
			realPos.y += directionMotion.y;
			scalePtr->SetRealPosition(realPos);
		}
	};

	scaleTop->setMotionCallback(callbackYAxis);
	scaleBot->setMotionCallback(callbackYAxis);

	_scaleLeft = scaleLeft.get();
	_scaleRight = scaleRight.get();
	_scaleTop = scaleTop.get();
	_scaleBot = scaleBot.get();
	_adjustMenu->AddChild(std::move(scaleLeft));
	_adjustMenu->AddChild(std::move(scaleRight));
	_adjustMenu->AddChild(std::move(scaleTop));
	_adjustMenu->AddChild(std::move(scaleBot));
}
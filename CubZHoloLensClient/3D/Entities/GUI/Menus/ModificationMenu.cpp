#include "pch.h"
#include <3D\Entities\GUI\Widgets\Button2D.h>
#include <3D\Entities\GUI\Widgets\Button3D.h>
#include <3D\Entities\GUI\Widgets\Panel.h>
#include <3D\Scene\HolographicScene.h>
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
		auto boundingBox = _attachedEntity->GetBoundingBox();
		
		//Display menu 15cm above bounding box of entity
		//No orientation take in consideration
		float3 offset = {0, boundingBox.Extents.y + 0.15f, 0};

		float3 boundingBoxCenter = { boundingBox.Center.x, boundingBox.Center.y, boundingBox.Center.z };

		//BoundingBox is calculated from the current position of the mesh in space

		SetRealPosition(boundingBoxCenter + offset);

		_scaleLeft->SetRealPosition(boundingBoxCenter +
			float3(-boundingBox.Extents.x - _scaleLeft->GetBoundingBox().Extents.x, 0.0f, 0.0f));
		_scaleRight->SetRealPosition(boundingBoxCenter +
			float3(boundingBox.Extents.x + _scaleLeft->GetBoundingBox().Extents.x, 0.0f, 0.0f));
		_scaleTop->SetRealPosition(boundingBoxCenter +
			float3(0.0f, boundingBox.Extents.y + _scaleLeft->GetBoundingBox().Extents.y, 0.0f));
		_scaleBot->SetRealPosition(boundingBoxCenter +
			float3(0.0f, -boundingBox.Extents.y - _scaleLeft->GetBoundingBox().Extents.y, 0.0f));
	}
}

void HoloLensClient::ModificationMenu::AttachEntity(EditableEntity *entity)
{
	if (_attachedEntity == entity)
		return;
	DetachEntity();
	_attachedEntity = entity;

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

void HoloLensClient::ModificationMenu::TryDetachEntity(EditableEntity * entity)
{
	if (_attachedEntity == entity)
		DetachEntity();
}

void HoloLensClient::ModificationMenu::initializeBasicMenu()
{
	//Build basic menu
	auto basicMenu = std::make_unique<EmptyEntity>(_scene, "Menu_Adjust_Remove");

	auto _backgroundBasic = std::make_unique<Panel>(_devicesResources, _scene, float2(0.35f, 0.07f), float4(0.0f, 0.392f, 0.0f, 0.6f));
	_backgroundBasic->SetRelativePosition({ 0.0f, 0.0f, 0.0f });

	auto removeButton = std::make_unique<Button2D>(_devicesResources, _scene, float2(0.12f, 0.07f));
	removeButton->setLabel(L"Remove");

	//Unused setup to help compiler allow further lambda -> callback conversions
	removeButton->SetAirTapCallback(nullptr);

	removeButton->SetAirTapCallback([this](Spatial::SpatialGestureRecognizer ^s, Spatial::SpatialTappedEventArgs ^a) {
		if (_attachedEntity)
			_attachedEntity->OnKillClick();
	});

	auto adjustButton = std::make_unique<Button2D>(_devicesResources, _scene, float2(0.12f, 0.07f));
	adjustButton->setLabel(L"Adjust");
	adjustButton->SetAirTapCallback([this](Spatial::SpatialGestureRecognizer ^, Spatial::SpatialTappedEventArgs ^) {
		_mode = ADJUST;
		_basicMenu->setVisible(false);
		_adjustMenu->setVisible(true);
		if (_attachedEntity)
			_attachedEntity->setEditableState(EditableEntity::State::ADJUST);
	});

	auto rotateButton = std::make_unique<Button2D>(_devicesResources, _scene, float2(0.12f, 0.07f));
	rotateButton->setLabel(L"Rotate");
	rotateButton->SetAirTapCallback([this](Spatial::SpatialGestureRecognizer ^, Spatial::SpatialTappedEventArgs ^) {
		_mode = ROTATE;
		_basicMenu->setVisible(false);
		_rotateMenu->setVisible(true);
		_attachedEntity->setEditableState(EditableEntity::State::ROTATE);
	});

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

	auto doneAdjust = std::make_unique<Button2D>(_devicesResources, _scene, float2(0.12f, 0.07f));
	doneAdjust->setLabel(L"Done");
	doneAdjust->SetAirTapCallback([this](Spatial::SpatialGestureRecognizer ^, Spatial::SpatialTappedEventArgs ^) {
		_mode = BASIC;
		_basicMenu->setVisible(true);
		_adjustMenu->setVisible(false);
		_attachedEntity->setEditableState(EditableEntity::State::IDLE);
	});

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

	auto doneAdjust = std::make_unique<Button2D>(_devicesResources, _scene, float2(0.12f, 0.07f));
	doneAdjust->setLabel(L"Done");
	doneAdjust->SetAirTapCallback([this](Spatial::SpatialGestureRecognizer ^, Spatial::SpatialTappedEventArgs ^) {
		_mode = BASIC;
		_basicMenu->setVisible(true);
		_rotateMenu->setVisible(false);
		_attachedEntity->setEditableState(EditableEntity::State::IDLE);
	});

	_backgroundRotate->AddGUIEntity(std::move(doneAdjust), { 0.12f, 0.0f });

	rotateMenu->AddChild(std::move(_backgroundRotate));
	rotateMenu->setVisible(false);

	_rotateMenu = rotateMenu.get();

	AddChild(std::move(rotateMenu));
}

void HoloLensClient::ModificationMenu::initializeAdjustBox()
{
	auto scaleLeft = std::make_unique<Button3D>(_devicesResources, _scene, float3(0.04f, 0.09f, 0.1f));
	auto scaleRight = std::make_unique<Button3D>(_devicesResources, _scene, float3(0.04f, 0.09f, 0.1f));

	auto scaleTop = std::make_unique<Button3D>(_devicesResources, _scene, float3(0.09f, 0.04f, 0.1f));
	auto scaleBot = std::make_unique<Button3D>(_devicesResources, _scene, float3(0.09f, 0.04f, 0.1f));

	scaleLeft->SetManipulationUpdatedCallback([this](Spatial::SpatialGestureRecognizer^, Spatial::SpatialManipulationUpdatedEventArgs ^args) {
		Spatial::SpatialManipulationDelta ^delta = args->TryGetCumulativeDelta(_scene->getCoordinateSystem());

		if (_attachedEntity)
		{
			auto width = _attachedEntity->GetBoundingBox().Extents.x * 2;
			auto percentage = (delta->Translation.x / width);
			auto newScaleX = _attachedEntity->GetScale().x * (1 - percentage);

			auto spaceDesc = CubZHoloLensClient::WinNetwork::SpaceDescription();

			if (newScaleX <= 0.1)
				return;

			spaceDesc.position = _attachedEntity->GetRealPosition();
			spaceDesc.rotation = _attachedEntity->GetRealRotation();
			spaceDesc.scale = _attachedEntity->GetScale();
			spaceDesc.scale.x = newScaleX;

			CubZHoloLensClient::HoloLensContext::Instance()->getUDPClient()->notifyEntityUpdate(_attachedEntity->GetNetworkDescription(), spaceDesc);
		}
	});

	scaleRight->SetManipulationUpdatedCallback([this](Spatial::SpatialGestureRecognizer^, Spatial::SpatialManipulationUpdatedEventArgs ^args) {
		Spatial::SpatialManipulationDelta ^delta = args->TryGetCumulativeDelta(_scene->getCoordinateSystem());

		if (_attachedEntity)
		{
			auto width = _attachedEntity->GetBoundingBox().Extents.x * 2;
			auto percentage = (delta->Translation.x / width);
			auto newScaleX = _attachedEntity->GetScale().x * (1 + percentage);

			auto spaceDesc = CubZHoloLensClient::WinNetwork::SpaceDescription();

			if (newScaleX <= 0.1)
				return;

			spaceDesc.position = _attachedEntity->GetRealPosition();
			spaceDesc.rotation = _attachedEntity->GetRealRotation();
			spaceDesc.scale = _attachedEntity->GetScale();
			spaceDesc.scale.x = newScaleX;

			CubZHoloLensClient::HoloLensContext::Instance()->getUDPClient()->notifyEntityUpdate(_attachedEntity->GetNetworkDescription(), spaceDesc);
		}
	});

	scaleTop->SetManipulationUpdatedCallback([this](Spatial::SpatialGestureRecognizer^, Spatial::SpatialManipulationUpdatedEventArgs ^args) {
		Spatial::SpatialManipulationDelta ^delta = args->TryGetCumulativeDelta(_scene->getCoordinateSystem());

		if (_attachedEntity)
		{
			auto width = _attachedEntity->GetBoundingBox().Extents.x * 2;
			auto percentage = (delta->Translation.y / width);
			auto newScaleY = _attachedEntity->GetScale().x * (1 + percentage);

			auto spaceDesc = CubZHoloLensClient::WinNetwork::SpaceDescription();

			if (newScaleY <= 0.1)
				return;

			spaceDesc.position = _attachedEntity->GetRealPosition();
			spaceDesc.rotation = _attachedEntity->GetRealRotation();
			spaceDesc.scale = _attachedEntity->GetScale();
			spaceDesc.scale.y = newScaleY;

			CubZHoloLensClient::HoloLensContext::Instance()->getUDPClient()->notifyEntityUpdate(_attachedEntity->GetNetworkDescription(), spaceDesc);
		}
	});

	scaleBot->SetManipulationUpdatedCallback([this](Spatial::SpatialGestureRecognizer^, Spatial::SpatialManipulationUpdatedEventArgs ^args) {
		Spatial::SpatialManipulationDelta ^delta = args->TryGetCumulativeDelta(_scene->getCoordinateSystem());

		if (_attachedEntity)
		{
			auto width = _attachedEntity->GetBoundingBox().Extents.x * 2;
			auto percentage = (delta->Translation.y / width);
			auto newScaleY = _attachedEntity->GetScale().x * (1 - percentage);

			auto spaceDesc = CubZHoloLensClient::WinNetwork::SpaceDescription();

			if (newScaleY <= 0.1)
				return;

			spaceDesc.position = _attachedEntity->GetRealPosition();
			spaceDesc.rotation = _attachedEntity->GetRealRotation();
			spaceDesc.scale = _attachedEntity->GetScale();
			spaceDesc.scale.y = newScaleY;

			CubZHoloLensClient::HoloLensContext::Instance()->getUDPClient()->notifyEntityUpdate(_attachedEntity->GetNetworkDescription(), spaceDesc);
		}
	});

	_scaleLeft = scaleLeft.get();
	_scaleRight = scaleRight.get();
	_scaleTop = scaleTop.get();
	_scaleBot = scaleBot.get();
	_adjustMenu->AddChild(std::move(scaleLeft));
	_adjustMenu->AddChild(std::move(scaleRight));
	_adjustMenu->AddChild(std::move(scaleTop));
	_adjustMenu->AddChild(std::move(scaleBot));
}
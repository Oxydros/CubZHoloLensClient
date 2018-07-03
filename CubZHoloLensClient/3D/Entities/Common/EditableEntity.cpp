#include <pch.h>
#include <3D\Entities\GUI\Menus\ModificationMenu.h>
#include "3D\Scene\HolographicScene.h"
#include <3D\Objects\Mesh\Common\ColoredObject.h>

#include "EditableEntity.h"

HoloLensClient::EditableEntity::EditableEntity(std::shared_ptr<DX::DeviceResources> devicesResources,
	std::shared_ptr<HolographicScene> scene, CubZHoloLensClient::WinNetwork::EntityDescription const &entityDesc)
	: Entity(scene, entityDesc)
{
	Spatial::SpatialGestureRecognizer ^spatial = ref new Spatial::SpatialGestureRecognizer(
		Spatial::SpatialGestureSettings::Tap | Spatial::SpatialGestureSettings::ManipulationTranslate);

	_tappedToken = spatial->Tapped +=
		ref new Windows::Foundation::TypedEventHandler<Spatial::SpatialGestureRecognizer^,
		Spatial::SpatialTappedEventArgs ^>(
			std::bind(&EditableEntity::OnAirTap, this, std::placeholders::_1, std::placeholders::_2)
			);

	_manipStartedToken = spatial->ManipulationStarted +=
		ref new Windows::Foundation::TypedEventHandler<Spatial::SpatialGestureRecognizer^,
		Spatial::SpatialManipulationStartedEventArgs ^>(
			std::bind(&EditableEntity::OnManipulationStarted, this, std::placeholders::_1, std::placeholders::_2)
			);

	_manipUpdatedToken = spatial->ManipulationUpdated +=
		ref new Windows::Foundation::TypedEventHandler<Spatial::SpatialGestureRecognizer^,
		Spatial::SpatialManipulationUpdatedEventArgs ^>(
			std::bind(&EditableEntity::OnManipulationUpdated, this, std::placeholders::_1, std::placeholders::_2)
			);

	_manipCompletedToken = spatial->ManipulationCompleted +=
		ref new Windows::Foundation::TypedEventHandler<Spatial::SpatialGestureRecognizer^,
		Spatial::SpatialManipulationCompletedEventArgs ^>(
			std::bind(&EditableEntity::OnManipulationCompleted, this, std::placeholders::_1, std::placeholders::_2)
			);

	_manipCanceledToken = spatial->ManipulationCanceled +=
		ref new Windows::Foundation::TypedEventHandler<Spatial::SpatialGestureRecognizer^,
		Spatial::SpatialManipulationCanceledEventArgs ^>(
			std::bind(&EditableEntity::OnManipulationCanceled, this, std::placeholders::_1, std::placeholders::_2)
			);

	SetSpatialGestureRecognizer(spatial);
}

HoloLensClient::EditableEntity::~EditableEntity()
{
	auto spatial = GetSpatialGestureRecognizer();
	if (spatial)
		spatial->Tapped -= _tappedToken;
}

bool HoloLensClient::EditableEntity::OnGetFocus()
{
	_scene->getModificationMenu()->AttachEntity(this);
	return true;
}

bool HoloLensClient::EditableEntity::OnLostFocus()
{
	return false;
}

void HoloLensClient::EditableEntity::kill()
{
	Entity::kill();
	_scene->getModificationMenu()->TryDetachEntity(this);
}

void HoloLensClient::EditableEntity::OnKillClick()
{
	CubZHoloLensClient::HoloLensContext::Instance()->getTCPClient()->deleteEntity(GetNetworkDescription());
}

void HoloLensClient::EditableEntity::OnAirTap(Spatial::SpatialGestureRecognizer ^sender,
	Spatial::SpatialTappedEventArgs ^args)
{
}

void HoloLensClient::EditableEntity::OnManipulationStarted(Spatial::SpatialGestureRecognizer ^recognizer, Spatial::SpatialManipulationStartedEventArgs ^args)
{
	if (_state == ADJUST)
		StartMoving();
}

void HoloLensClient::EditableEntity::OnManipulationUpdated(Spatial::SpatialGestureRecognizer ^recognizer, Spatial::SpatialManipulationUpdatedEventArgs ^args)
{
	Spatial::SpatialManipulationDelta ^delta = args->TryGetCumulativeDelta(_scene->getCoordinateSystem());

	if (_state == ADJUST)
	{
		/*TRACE("GOT DELTA OF " << delta->Translation << std::endl);*/
		/*this->Move(delta->Translation);*/
		auto spaceDesc = CubZHoloLensClient::WinNetwork::SpaceDescription();

		spaceDesc.position = GetRealPosition() + delta->Translation;
		spaceDesc.rotation = GetRealRotation();
		spaceDesc.scale = GetScale();

		CubZHoloLensClient::HoloLensContext::Instance()->getUDPClient()->notifyEntityUpdate(GetNetworkDescription(), spaceDesc);
	}
}

void HoloLensClient::EditableEntity::OnManipulationCompleted(Spatial::SpatialGestureRecognizer ^recognizer, Spatial::SpatialManipulationCompletedEventArgs ^args)
{
	if (_state == ADJUST)
		StopMoving();
}

void HoloLensClient::EditableEntity::OnManipulationCanceled(Spatial::SpatialGestureRecognizer ^recognizer, Spatial::SpatialManipulationCanceledEventArgs ^args)
{
	if (_state == ADJUST)
		StopMoving();
}

void HoloLensClient::EditableEntity::StopMoving()
{
	auto coloredMesh = dynamic_cast<ColoredObject*>(_mesh.get());

	if (coloredMesh)
	{
		float4 actualColor = coloredMesh->GetColor();
		actualColor.x -= 0.1f;
		actualColor.y -= 0.1f;
		actualColor.z -= 0.1f;
		coloredMesh->SetColor(actualColor);
	}

	_moving = false;
}

void HoloLensClient::EditableEntity::StartMoving()
{
	auto coloredMesh = dynamic_cast<ColoredObject*>(_mesh.get());

	if (coloredMesh)
	{
		float4 actualColor = coloredMesh->GetColor();
		actualColor.x += 0.1f;
		actualColor.y += 0.1f;
		actualColor.z += 0.1f;
		coloredMesh->SetColor(actualColor);
	}
	_moving = true;
}
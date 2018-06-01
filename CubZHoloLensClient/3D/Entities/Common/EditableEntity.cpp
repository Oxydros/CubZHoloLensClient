#include <pch.h>
#include <3D\Entities\GUI\ModificationMenu.h>
#include "3D\Scene\HolographicScene.h"
#include <3D\Objects\Mesh\Common\ColoredObject.h>

#include "EditableEntity.h"

HoloLensClient::EditableEntity::EditableEntity(std::shared_ptr<DX::DeviceResources> devicesResources,
											   std::shared_ptr<HolographicScene> scene)
	: InteractableEntity(scene)
{
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

bool HoloLensClient::EditableEntity::OnAirTap()
{
	//Retrieve Colored cube mesh
	auto coloredMesh = dynamic_cast<ColoredObject*>(_mesh.get());	

	if (_state == MOVE)
	{
		if (IsMoving())
			StopMoving();
		else
			StartMoving();
	}
	else if (_state == ROTATE)
	{
		TRACE("ROTATING STATE" << std::endl);
	}
	return (true);
}

void HoloLensClient::EditableEntity::OnRotateLeftClick()
{
	TRACE("Rotate left" << std::endl);
	Rotate({ 0, 20, 0 });
}

void HoloLensClient::EditableEntity::OnRotateRightClick()
{
	TRACE("Rotate Right" << std::endl);
	Rotate({ 0, -20, 0 });
}

void HoloLensClient::EditableEntity::OnMoveClick()
{
	_moving = true;
	//Distance should min between 2meters and distance of a wall / real object
	setFollowGaze(true, false, { 0, 0, 2.0f });
}

void HoloLensClient::EditableEntity::OnKillClick()
{
	_scene->getModificationMenu()->DetachEntity();
	kill();
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
	setFollowGaze(false, false);
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

	setFollowGaze(true, false);
	_moving = true;
}

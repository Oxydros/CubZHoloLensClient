#include "pch.h"

#include <3D\Objects\Mesh\3DFormes\OBJMesh.h>

#include "3D\Entities\ObjEntity.h"
#include "3D\Entities\GUI\ModificationMenu.h"
#include "3D\Scene\HolographicScene.h"

using namespace HoloLensClient;
using namespace DirectX;
using namespace Windows::Foundation::Numerics;

ObjEntity::ObjEntity(std::shared_ptr<DX::DeviceResources> devicesResources, std::shared_ptr<HolographicScene> scene,
					 std::string const&fileName)
	: EditableEntity(devicesResources, scene)
{

	auto mesh = std::make_unique<OBJMesh>(devicesResources, fileName);

	addMesh(std::move(mesh));
}

ObjEntity::~ObjEntity()
{
}

void ObjEntity::DoUpdate(DX::StepTimer const & timer)
{
}

bool HoloLensClient::ObjEntity::OnAirTap()
{
	TRACE("Got input on " << this << std::endl;);
	//Retrieve Colored cube mesh
	auto coloredMesh = dynamic_cast<OBJMesh*>(_mesh.get());

	float4 actualColor = coloredMesh->GetColor();

	if (_selected)
	{
		actualColor.x -= 0.1f;
		actualColor.y -= 0.1f;
		actualColor.z -= 0.1f;
		coloredMesh->SetColor(actualColor);
		_selected = false;
		this->setFollowGaze(false, false);
	}
	return true;
}

bool HoloLensClient::ObjEntity::OnGetFocus()
{
	/*TRACE("Got Focus on " << this << std::endl);*/
	_scene->getModificationMenu()->AttachEntity(this);
	return true;
}

bool HoloLensClient::ObjEntity::OnLostFocus()
{
	/*TRACE("Lost focus on " << this << std::endl);*/
	/*_scene->getModificationMenu()->DetachEntity();*/
	return true;
}

void HoloLensClient::ObjEntity::OnMoveClick()
{
	auto coloredMesh = dynamic_cast<OBJMesh*>(_mesh.get());

	float4 actualColor = coloredMesh->GetColor();
	actualColor.x += 0.1f;
	actualColor.y += 0.1f;
	actualColor.z += 0.1f;
	coloredMesh->SetColor(actualColor);
	_selected = true;
	//Distance should min between 2meters and distance of a wall / real object
	this->setFollowGaze(true, false, { 0, 0, 2.0f });
}

void HoloLensClient::ObjEntity::OnRotateLeftClick()
{
	TRACE("Rotate left" << std::endl);
	Rotate({ 0, 20, 0 });
}

void HoloLensClient::ObjEntity::OnRotateRightClick()
{
	TRACE("Rotate Right" << std::endl);
	Rotate({ 0, -20, 0 });
}

void HoloLensClient::ObjEntity::OnKillClick()
{
	_scene->getModificationMenu()->DetachEntity();
	kill();
}

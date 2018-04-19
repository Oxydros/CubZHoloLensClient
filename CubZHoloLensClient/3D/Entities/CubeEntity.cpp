#include "pch.h"

#include <3D\Objects\Mesh\3DFormes\ColoredCube.h>
#include <3D\Objects\Mesh\2DFormes\TexturedRectangle.h>
#include <3D\Objects\Mesh\2DFormes\ColoredRectangle.h>
#include <3D\Objects\GUI\TextObject.h>
#include <3D\Objects\GUI\ButtonObject.h>

#include <3D\Objects\Mesh\3DFormes\OBJMesh.h>

#include "3D\Entities\CubeEntity.h"
#include "3D\Scene\HolographicScene.h"

using namespace HoloLensClient;
using namespace DirectX;
using namespace Windows::Foundation::Numerics;

CubeEntity::CubeEntity(std::shared_ptr<DX::DeviceResources> &devicesResources, std::shared_ptr<HolographicScene> &scene)
	: InteractableEntity(scene)
{
	//auto texture = std::make_shared<Texture2D>(devicesResources, "ms-appx:////Assets//folderIcon.png");
	//auto cube = std::make_unique<TexturedRectangle>(devicesResources, float2(0.1f, 0.1f));
	//cube->SetTexture(texture);

	auto cube = std::make_unique<ColoredCube>(devicesResources, float4(0.2f, 0.3f, 0.5f, 1.0f), float3(0.1f, 0.1f, 0.1f));

	/*auto cube = std::make_unique<TextObject>(devicesResources, float2(0.2f, 0.2f));*/

	/*auto cube = std::make_unique<ButtonObject>(devicesResources, float2(0.4f, 0.2f));*/

	/*auto cube = std::make_unique<ColoredRectangle>(devicesResources, float2(0.5f, 0.25f));*/

	addMesh(std::move(cube));
}

CubeEntity::~CubeEntity()
{
}

void CubeEntity::DoUpdate(DX::StepTimer const & timer)
{
	InteractableEntity::DoUpdate(timer);
}

bool HoloLensClient::CubeEntity::OnAirTap()
{
	TRACE("Got input on " << this << std::endl;);
	//Retrieve Colored cube mesh
	auto coloredMesh = dynamic_cast<ColoredCube*>(_mesh.get());

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
	else {
		actualColor.x += 0.1f;
		actualColor.y += 0.1f;
		actualColor.z += 0.1f;
		coloredMesh->SetColor(actualColor);
		_selected = true;
		//Distance should min between 2meters and distance of a wall / real object
		this->setFollowGaze(true, false, {0, 0, 2.0f});
	}
	return true;
}

bool HoloLensClient::CubeEntity::OnGetFocus()
{
	TRACE("Got Focus on " << this << std::endl;);
	return true;
}

bool HoloLensClient::CubeEntity::OnLostFocus()
{
	TRACE("Lost focus on " << this << std::endl;);
	return true;
}

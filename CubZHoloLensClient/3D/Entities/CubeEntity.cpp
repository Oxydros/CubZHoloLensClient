#include "pch.h"

#include <3D\Objects\Mesh\3DFormes\ColoredCube.h>
#include <3D\Objects\Mesh\2DFormes\TexturedRectangle.h>
#include <3D\Objects\Mesh\2DFormes\ColoredRectangle.h>
#include <3D\Objects\GUI\TextObject.h>
#include <3D\Objects\GUI\ButtonObject.h>

#include "3D\Entities\CubeEntity.h"
#include "3D\Scene\HolographicScene.h"

using namespace HoloLensClient;
using namespace DirectX;
using namespace Windows::Foundation::Numerics;

CubeEntity::CubeEntity(std::shared_ptr<DX::DeviceResources> &devicesResources, std::shared_ptr<HolographicScene> &scene)
	: InteractableEntity(scene)
{
	//auto texture = std::make_shared<Texture2D>(devicesResources, "ms-appx:////Assets//folderIcon.png");
	//auto cube = std::make_unique<TexturedRectangle>(devicesResources, float3(0.1f, 0.1f));
	//cube->SetTexture(texture);

	//auto cube = std::make_unique<ColoredCube>(devicesResources, float4(0.2f, 0.3f, 0.5f), float3(0.1f, 0.1f, 0.1f));

	/*auto cube = std::make_unique<TextObject>(devicesResources, float2(0.2f, 0.2f));*/

	auto cube = std::make_unique<ButtonObject>(devicesResources, float2(0.4f, 0.2f));

	/*auto cube = std::make_unique<ColoredRectangle>(devicesResources, float2(0.5f, 0.25f));*/
	addMesh(std::move(cube));
	InitializeMesh();
}

CubeEntity::~CubeEntity()
{
}

void CubeEntity::DoUpdate(DX::StepTimer const & timer)
{
}

void HoloLensClient::CubeEntity::OnAirTap()
{
	TRACE("Got input on " << this << std::endl;);
	//getMesh()->SetColor({ 0.1f, 0.8f, 0.1f, 1.0f });
	kill();
	/*dynamic_cast<TextObject *>(getMesh().get())->setText(L"TEST");*/
}

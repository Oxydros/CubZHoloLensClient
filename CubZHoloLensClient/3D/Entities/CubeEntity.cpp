#include "pch.h"

#include <3D\Objects\ColoredCube.h>
#include <3D\Objects\TexturedCube.h>
#include "3D\Entities\CubeEntity.h"
#include "3D\Scene\HolographicScene.h"

using namespace HoloLensClient;
using namespace DirectX;
using namespace Windows::Foundation::Numerics;

CubeEntity::CubeEntity(std::shared_ptr<DX::DeviceResources> &devicesResources, std::shared_ptr<HolographicScene> &scene)
	: InteractableEntity(scene)
{
	//auto texture = std::make_shared<Texture2D>(devicesResources, "ms-appx:////Assets//folderIcon.png");
	//auto cube = std::make_unique<TexturedCube>(devicesResources, float3(0.1f, 0.1f, 0.1f));
	//cube->SetTexture(texture);

	auto cube = std::make_unique<ColoredCube>(devicesResources, float4(0.2f, 0.3f, 0.5f), float3(0.1f, 0.1f, 0.1f));
	setMesh(std::move(cube));
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
}

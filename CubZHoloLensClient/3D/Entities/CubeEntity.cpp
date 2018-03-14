#include "pch.h"

#include <3D\Objects\ColoredCube.h>
#include "3D\Entities\CubeEntity.h"
#include "3D\Scene\HolographicScene.h"

using namespace HoloLensClient;
using namespace DirectX;
using namespace Windows::Foundation::Numerics;

CubeEntity::CubeEntity(std::shared_ptr<DX::DeviceResources> &devicesResources, std::shared_ptr<HolographicScene> &scene)
	: InteractableEntity(scene)
{
	setMesh(std::make_unique<ColoredCube>(devicesResources, float3(0.3f, 0.5f, 0.2f), float3(0.1f, 0.1f, 0.1f)));
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
	kill();
}

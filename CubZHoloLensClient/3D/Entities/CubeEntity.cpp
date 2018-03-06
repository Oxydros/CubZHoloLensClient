#include "pch.h"

#include "3D\Entities\CubeEntity.h"
#include "3D\Utility\DirectXHelper.h"
#include "3D\Scene\HolographicScene.h"

using namespace HoloLensClient;
using namespace DirectX;
using namespace Windows::Foundation::Numerics;

CubeEntity::CubeEntity(std::shared_ptr<DX::DeviceResources> devicesResources, std::shared_ptr<HolographicScene> scene)
	: _mesh(devicesResources), _scene(std::move(scene))
{
	InitializeMesh();
}

CubeEntity::~CubeEntity()
{
}

void CubeEntity::Update(DX::StepTimer const & timer)
{
	const XMMATRIX modelTranslation = XMMatrixTranslationFromVector(XMLoadFloat3(&_mesh.getPosition()));
	_mesh.ApplyMatrix(XMMatrixTranspose(modelTranslation));

	// Loading is asynchronous. Resources must be created before they can be updated.
	_mesh.Update();
}

void CubeEntity::Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState ^pointerState)
{
}

void CubeEntity::InitializeMesh()
{
	_mesh.Initialize();
}

void CubeEntity::ReleaseMesh()
{
	_mesh.Release();
}

void CubeEntity::Render()
{
	_mesh.Render();
}

ObjectRenderer const &CubeEntity::getMesh() const
{
	return (_mesh);
}

void HoloLensClient::CubeEntity::setPosition(Windows::Foundation::Numerics::float3 pos)
{
	_mesh.setPosition(pos);
}

Windows::Foundation::Numerics::float3 HoloLensClient::CubeEntity::getPosition() const
{
	return (_mesh.getPosition());
}

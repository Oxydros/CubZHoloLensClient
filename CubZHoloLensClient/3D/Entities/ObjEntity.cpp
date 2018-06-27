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
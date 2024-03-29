#include "pch.h"
#include "Panel.h"
#include <3D\Objects\Mesh\3DFormes\ColoredCube.h>

using namespace HoloLensClient;

Panel::Panel(std::shared_ptr<DX::DeviceResources> devicesResources,
			 std::shared_ptr<HolographicScene> scene, float2 size, float4 color)
	: GUIEntity(scene), _size(size), _color(color)
{
	float3 panelSize{ size.x, size.y, 0.05f };
	SetSize(panelSize);
	auto background = std::make_unique<ColoredCube>(devicesResources, panelSize, color);
	addMesh(std::move(background));
	SetIgnoreInGaze(true);
}

void HoloLensClient::Panel::AddGUIEntity(IEntity::IEntityPtr child, Windows::Foundation::Numerics::float2 offsetXY)
{
	child->SetRelativePosition({ offsetXY.x, offsetXY.y, (GetSize().z / 2) + 0.025f });
	AddChild(std::move(child));
}

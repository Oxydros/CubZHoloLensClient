#include "pch.h"
#include "Panel.h"
#include <3D\Objects\Mesh\2DFormes\ColoredRectangle.h>

using namespace HoloLensClient;

Panel::Panel(std::shared_ptr<DX::DeviceResources> &devicesResources,
			 std::shared_ptr<HolographicScene> &scene, float2 size, float4 color)
	: GUIEntity(scene), _size(size), _color(color)
{
	auto background = std::make_unique<ColoredRectangle>(devicesResources, size, color);
	addMesh(std::move(background));
}

void Panel::DoUpdate(DX::StepTimer const & timer)
{
}

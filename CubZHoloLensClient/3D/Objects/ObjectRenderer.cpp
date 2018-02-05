#include "pch.h"
#include "3D\Objects\ObjectRenderer.h"

using namespace HoloLensClient;

ObjectRenderer::ObjectRenderer(std::shared_ptr<DX::DeviceResources> deviceResources, HolographicScene const *scene)
	: _scene(scene), _deviceResources(deviceResources), _pos(0, 0, 0)
{
}

ObjectRenderer::~ObjectRenderer()
{}

void ObjectRenderer::setPosition(Windows::Foundation::Numerics::float3 pos)
{
	_pos = pos;
}

Windows::Foundation::Numerics::float3 ObjectRenderer::getPosition() const
{
	return (_pos);
}
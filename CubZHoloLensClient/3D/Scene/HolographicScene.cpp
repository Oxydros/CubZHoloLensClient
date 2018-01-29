#include "pch.h"
#include "HolographicScene.h"
#include "3D\Objects\SpinningCubeRenderer.h"

using namespace HoloLensClient;

HolographicScene::HolographicScene(std::shared_ptr<DX::DeviceResources> deviceResources)
	: _deviceResources(deviceResources)
{
	_objects.emplace_back(std::make_unique<SpinningCubeRenderer>(_deviceResources));
}


HolographicScene::~HolographicScene()
{
}

void HolographicScene::Update(DX::StepTimer const& timer)
{
	std::for_each(_objects.begin(), _objects.end(),
		[&timer](auto &object)
	{
		object->Update(timer);
	});
}

void HolographicScene::Render()
{
	std::for_each(_objects.begin(), _objects.end(),
		[](auto &object)
	{
		object->Render();
	});
}

void HolographicScene::OnDeviceLost()
{
	std::for_each(_objects.begin(), _objects.end(),
		[](auto &object)
	{
		object->ReleaseDeviceDependentResources();
	});
}

void HolographicScene::OnDeviceRestored()
{
	std::for_each(_objects.begin(), _objects.end(),
		[](auto &object)
	{
		object->CreateDeviceDependentResources();
	});
}

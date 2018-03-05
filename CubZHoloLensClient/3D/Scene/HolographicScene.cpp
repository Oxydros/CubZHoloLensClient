#include "pch.h"
#include "HolographicScene.h"
#include "3D\Objects\SpinningCubeRenderer.h"
#include "3D\Objects\GazeRenderer.h"

using namespace HoloLensClient;

HolographicScene::HolographicScene(std::shared_ptr<DX::DeviceResources> deviceResources)
	: _deviceResources(deviceResources)
{
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

void HoloLensClient::HolographicScene::UpdateCoordinateSystem(Windows::Perception::Spatial::SpatialCoordinateSystem ^coordinateSystem)
{
	_coordinateSystem = coordinateSystem;
}

void HoloLensClient::HolographicScene::UpdatePointerPose(Windows::UI::Input::Spatial::SpatialPointerPose ^ pointerPose)
{
	_pointerPose = pointerPose;
}

void HoloLensClient::HolographicScene::Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState)
{
	std::for_each(_objects.begin(), _objects.end(),
		[&pointerState](auto &object)
	{
		object->Inputs(pointerState);
	});
}

void HolographicScene::OnDeviceLost()
{
	std::for_each(_objects.begin(), _objects.end(),
		[](auto &object)
	{
		object->Release();
	});
}

void HolographicScene::OnDeviceRestored()
{
	std::for_each(_objects.begin(), _objects.end(),
		[](auto &object)
	{
		object->Initialize();
	});
}

Windows::Perception::Spatial::SpatialCoordinateSystem ^ HoloLensClient::HolographicScene::getCoordinateSystem() const
{
	return (_coordinateSystem);
}

Windows::UI::Input::Spatial::SpatialPointerPose ^ HoloLensClient::HolographicScene::getPointerPose() const
{
	return (_pointerPose);
}

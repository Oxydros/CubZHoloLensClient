#pragma once

#include "3D\Objects\ObjectRenderer.h"

namespace HoloLensClient
{
	class HolographicScene
	{
	private:
		std::shared_ptr<DX::DeviceResources>			_deviceResources;
		std::vector<std::unique_ptr<ObjectRenderer>>	_objects;
	public:
		HolographicScene(std::shared_ptr<DX::DeviceResources> deviceResources);
		~HolographicScene();

	public:
		void Update(DX::StepTimer const& timer);
		void Render();
		void OnDeviceLost();
		void OnDeviceRestored();
	};
}


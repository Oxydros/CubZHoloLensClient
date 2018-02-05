#pragma once

#include "3D\Objects\ObjectRenderer.h"

namespace HoloLensClient
{
	class HolographicScene
	{
	private:
		Windows::Perception::Spatial::SpatialCoordinateSystem^	_coordinateSystem;
		std::shared_ptr<DX::DeviceResources>					_deviceResources;
		std::vector<std::unique_ptr<ObjectRenderer>>			_objects;
	public:
		HolographicScene(std::shared_ptr<DX::DeviceResources> deviceResources);
		~HolographicScene();

	public:
		void Update(DX::StepTimer const& timer);
		void Render();
		void UpdateCoordinateSystem(Windows::Perception::Spatial::SpatialCoordinateSystem^ coordinateSystem);
		void Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState);
		void OnDeviceLost();
		void OnDeviceRestored();
		Windows::Perception::Spatial::SpatialCoordinateSystem^ getCoordinateSystem() const;
	};
}


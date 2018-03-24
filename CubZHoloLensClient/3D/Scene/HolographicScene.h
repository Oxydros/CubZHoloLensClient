#pragma once

#include "3D\Entities\Common\Entity.h"

namespace HoloLensClient
{
	class HolographicScene : public std::enable_shared_from_this<HolographicScene>
	{
	public:
		typedef std::shared_ptr<HolographicScene>				SharedPtr;

	private:
		Windows::Perception::Spatial::SpatialCoordinateSystem^	_coordinateSystem;
		Windows::UI::Input::Spatial::SpatialPointerPose^		_pointerPose;
		std::shared_ptr<DX::DeviceResources>					_deviceResources;
		std::vector<std::unique_ptr<Entity>>					_entities;
		std::vector<std::unique_ptr<Entity>>					_newEntities;

	public:
		HolographicScene(std::shared_ptr<DX::DeviceResources> deviceResources);
		~HolographicScene();

	public:
		void Initialize();
		void Update(DX::StepTimer const& timer);
		void Render();
		void UpdateCoordinateSystem(Windows::Perception::Spatial::SpatialCoordinateSystem^ coordinateSystem);
		void UpdatePointerPose(Windows::UI::Input::Spatial::SpatialPointerPose ^pointerPose);
		void Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState);
		void OnDeviceLost();
		void OnDeviceRestored();

	public:
		void addEntity(std::unique_ptr<Entity> e);

	public:
		Windows::Perception::Spatial::SpatialCoordinateSystem^ getCoordinateSystem() const;
		Windows::UI::Input::Spatial::SpatialPointerPose^ getPointerPose() const;
	};
}


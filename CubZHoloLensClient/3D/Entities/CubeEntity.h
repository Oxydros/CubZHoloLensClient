#pragma once

#include "3D\Entities\InteractableEntity.h"

namespace HoloLensClient
{
	class CubeEntity : public InteractableEntity
	{
	public:
		CubeEntity(std::shared_ptr<DX::DeviceResources> &devicesResources, std::shared_ptr<HolographicScene> &scene);
		~CubeEntity();

	public:
		void DoUpdate(DX::StepTimer const &timer) override;
		void OnAirTap() override;
	};
}
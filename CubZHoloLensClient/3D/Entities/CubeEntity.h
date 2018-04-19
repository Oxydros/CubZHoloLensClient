#pragma once

#include "3D\Entities\Common\InteractableEntity.h"

namespace HoloLensClient
{
	class CubeEntity : public InteractableEntity
	{
	private:
		bool					_selected{ false };

	public:
		CubeEntity(std::shared_ptr<DX::DeviceResources> &devicesResources, std::shared_ptr<HolographicScene> &scene);
		~CubeEntity();

	public:
		void DoUpdate(DX::StepTimer const &timer) override;
		bool OnAirTap() override;
		bool OnGetFocus() override;
		bool OnLostFocus() override;

		std::string const GetLabel() const override { return "Cube"; }
	};
}
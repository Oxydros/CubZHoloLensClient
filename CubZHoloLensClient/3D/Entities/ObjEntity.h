#pragma once

#include "3D\Entities\Common\EditableEntity.h"

namespace HoloLensClient
{
	class ObjEntity : public EditableEntity
	{
	private:
		bool					_selected{ false };

	public:
		ObjEntity(std::shared_ptr<DX::DeviceResources> devicesResources, std::shared_ptr<HolographicScene> scene,
				  std::string const&fileName);
		~ObjEntity();

	public:
		void DoUpdate(DX::StepTimer const &timer) override;
		bool OnAirTap() override;
		bool OnGetFocus() override;
		bool OnLostFocus() override;

		void OnMoveClick() override;
		void OnRotateLeftClick() override;
		void OnRotateRightClick() override;
		void OnKillClick() override;

		std::string const GetLabel() const override { return "ObjEntity"; }
	};
}
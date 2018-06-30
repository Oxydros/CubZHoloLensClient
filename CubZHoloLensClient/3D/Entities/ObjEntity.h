#pragma once

#include "3D\Entities\Common\EditableEntity.h"

namespace HoloLensClient
{
	class ObjEntity : public EditableEntity
	{
	public:
		ObjEntity(std::shared_ptr<DX::DeviceResources> devicesResources, std::shared_ptr<HolographicScene> scene,
				  std::string const&fileName);
		~ObjEntity();

	public:
		void DoUpdate(DX::StepTimer const &timer) override;

		std::string const GetLabel() const override { return "ObjEntity"; }
	};
}
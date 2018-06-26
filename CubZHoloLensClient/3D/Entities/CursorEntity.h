#pragma once

#include "3D\Entities\Common\Entity.h"

namespace HoloLensClient
{
	class CursorEntity : public Entity
	{
	public:
		CursorEntity(std::shared_ptr<DX::DeviceResources> devicesResources, std::shared_ptr<HolographicScene> scene);
		~CursorEntity();

	protected:
		void DoUpdate(DX::StepTimer const &timer) override;

		std::string const GetLabel() const override { return "Cursor"; }

	private:
		void getFaceRotationTranslationModel(DirectX::XMMATRIX &translation, DirectX::XMMATRIX &rotation, float distance,
												Windows::UI::Input::Spatial::SpatialPointerPose ^pointerPose);
	};
}
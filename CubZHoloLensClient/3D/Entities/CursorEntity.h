#pragma once

#include "3D\Entities\Entity.h"

namespace HoloLensClient
{
	class CursorEntity : public Entity
	{
	public:
		CursorEntity(std::shared_ptr<DX::DeviceResources> &devicesResources, std::shared_ptr<HolographicScene> &scene);
		~CursorEntity();

	public:
		void DoUpdate(DX::StepTimer const &timer) override;
		void Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState) override;

	private:
		void getFaceRotationTranslationModel(DirectX::XMMATRIX &translation, DirectX::XMMATRIX &rotation, float distance,
												Windows::UI::Input::Spatial::SpatialPointerPose ^pointerPose);
	};
}
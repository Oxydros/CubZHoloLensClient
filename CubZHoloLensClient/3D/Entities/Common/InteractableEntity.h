#pragma once

#include <3D\Entities\Common\Entity.h>

namespace HoloLensClient
{
	class InteractableEntity : public Entity
	{
	protected:
		bool				_focused{ false };

	public:
		InteractableEntity(std::shared_ptr<HolographicScene> &scene);

		void DoUpdate(DX::StepTimer const &timer) override;
		void OnInputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState) override final;
		bool isFocused() const { return _focused; }

	public:
		virtual void OnAirTap() = 0;
		virtual void OnGetFocus() = 0;
		virtual void OnLostFocus() = 0;

	private:
		void GetBiggestBoundingBox(DirectX::BoundingOrientedBox &boundingBox);
		void setFocus(bool);
	};
}
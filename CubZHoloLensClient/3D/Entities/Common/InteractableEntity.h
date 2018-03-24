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

		void Update(DX::StepTimer const &timer) override;
		void Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState) override;
		bool isFocused() const { return _focused; }

	public:
		virtual void OnAirTap() = 0;
	};
}
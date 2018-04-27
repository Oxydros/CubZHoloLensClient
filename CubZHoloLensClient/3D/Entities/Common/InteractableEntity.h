#pragma once

#include <3D\Entities\Common\Entity.h>

namespace HoloLensClient
{
	class InteractableEntity : public Entity
	{
	protected:
		bool				_disableMultipleClick{ true };
		bool				_clicked{ false };

	public:
		InteractableEntity(std::shared_ptr<HolographicScene> scene,
			bool disableMultipleClick = true);
		virtual ~InteractableEntity() = default;

		void OnInputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState) override final;

	public:
		void disableMultipleClick(bool disable) { _disableMultipleClick = disable; }

	public:
		/// <summary>	Executes the air tap action. </summary>
		virtual bool OnAirTap() = 0;
	};
}
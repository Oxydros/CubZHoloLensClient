#pragma once

#include <3D\Entities\Common\Entity.h>

namespace HoloLensClient
{
	class InteractableEntity : public Entity
	{
	protected:
		bool				_focused{ false };
		bool				_disableMultipleClick{ true };
		bool				_clicked{ false };

	public:
		InteractableEntity(std::shared_ptr<HolographicScene> &scene,
			bool disableMultipleClick = true);

		void DoUpdate(DX::StepTimer const &timer) override;
		void OnInputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState) override final;

	public:

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Query if this object is focused. </summary>
		///
		/// <returns>	True if focused, false if not. </returns>
		///-------------------------------------------------------------------------------------------------
		bool isFocused() const { return _focused; }

	public:
		/// <summary>	Executes the air tap action. </summary>
		virtual bool OnAirTap() = 0;
		/// <summary>	Executes the get focus action. </summary>
		virtual bool OnGetFocus() { return false; }
		/// <summary>	Executes the lost focus action. </summary>
		virtual bool OnLostFocus() { return false; }

	private:

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets biggest bounding box. </summary>
		///
		/// <param name="boundingBox">	[in,out] The bounding box. </param>
		///-------------------------------------------------------------------------------------------------
		void GetBiggestBoundingBox(DirectX::BoundingOrientedBox &boundingBox);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets the focus. </summary>
		///
		/// <param name="parameter1">	True to parameter 1. </param>
		///-------------------------------------------------------------------------------------------------
		void setFocus(bool);
	};
}
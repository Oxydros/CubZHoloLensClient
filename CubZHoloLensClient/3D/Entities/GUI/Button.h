#pragma once

#include <3D\Entities\GUI\GUIEntity.h>

namespace HoloLensClient
{
	class Button : public GUIEntity
	{
	public:
		static Windows::Foundation::Numerics::float4 DEFAULT_COLOR;
		static Windows::Foundation::Numerics::float4 FOCUS_COLOR;

	protected:
		std::function<void()>			_callback = nullptr;

	public:
		Button(std::shared_ptr<DX::DeviceResources> &devicesResources, std::shared_ptr<HolographicScene> &scene,
			   std::function<void()> const &callback = {},
			   Windows::Foundation::Numerics::float2 size = { 0.2f, 0.2f });
		virtual ~Button() = default;

	public:
		void setCallback(std::function<void()> &callback) { _callback = callback; }
		void OnLabelChanged() override;

		void OnGetFocus() override;
		void OnLostFocus() override;

	public:
		// Inherited via InteractableEntity
		void DoUpdate(DX::StepTimer const & timer) override { InteractableEntity::DoUpdate(timer); };
		void OnAirTap() override;

		std::string const GetLabel() const override { return "Button"; }
	};
}
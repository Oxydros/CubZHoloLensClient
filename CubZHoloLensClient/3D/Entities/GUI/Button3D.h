#pragma once

#include <functional>
#include <memory>
#include <3D\Entities\GUI\GUIEntity.h>

using namespace Windows::UI::Input;

namespace HoloLensClient
{
	class Button3D : public GUIEntity
	{
	public:
		static Windows::Foundation::Numerics::float4 DEFAULT_COLOR;
		static Windows::Foundation::Numerics::float4 FOCUS_COLOR;

	protected:
		std::function<void()>			_callback = nullptr;

	public:
		Button3D(std::shared_ptr<DX::DeviceResources> devicesResources, std::shared_ptr<HolographicScene> scene,
			std::function<void()> const &callback = {},
			Windows::Foundation::Numerics::float3 size = { 0.2f, 0.2f });
		virtual ~Button3D() = default;

	public:
		void DoUpdate(DX::StepTimer const &timer) override {};
		void setCallback(std::function<void()> callback) { _callback = callback; }

		bool OnGetFocus() override;
		bool OnLostFocus() override;

	public:
		void OnAirTap(Spatial::SpatialGestureRecognizer ^sender,
			Spatial::SpatialTappedEventArgs ^args);

		std::string const GetLabel() const override { return std::string(_label.begin(), _label.end()); }
	};
}
#pragma once

#include <memory>
#include <functional>
#include <3D\Entities\GUI\GUIEntity.h>

using namespace Windows::UI::Input;

namespace HoloLensClient
{
	class Button2D : public GUIEntity
	{
	private:
		Windows::Foundation::EventRegistrationToken _tappedToken;

	public:
		static Windows::Foundation::Numerics::float4 DEFAULT_COLOR;
		static Windows::Foundation::Numerics::float4 FOCUS_COLOR;

	protected:
		std::function<void()>			_callback = nullptr;

	public:
		Button2D(std::shared_ptr<DX::DeviceResources> devicesResources, std::shared_ptr<HolographicScene> scene,
			   std::function<void()> const &callback = {},
			   Windows::Foundation::Numerics::float2 size = { 0.2f, 0.2f });
		virtual ~Button2D();

	public:
		void DoUpdate(DX::StepTimer const &timer) override {};
		void setCallback(std::function<void()> callback) { _callback = callback; }
		void OnLabelChanged() override;

		bool OnGetFocus() override;
		bool OnLostFocus() override;

	public:
		void OnAirTap(Spatial::SpatialGestureRecognizer ^sender,
						Spatial::SpatialTappedEventArgs ^args);

		std::string const GetLabel() const override { return std::string(_label.begin(), _label.end()); }
	};
}
#pragma once

#include <memory>
#include <functional>
#include <3D\Entities\GUI\GUIEntity.h>

using namespace Windows::UI::Input;

namespace HoloLensClient
{
	class Button2D : public GUIEntity
	{
	public:
		static Windows::Foundation::Numerics::float4 DEFAULT_COLOR;
		static Windows::Foundation::Numerics::float4 FOCUS_COLOR;

	public:
		Button2D(std::shared_ptr<DX::DeviceResources> devicesResources, std::shared_ptr<HolographicScene> scene,
			   Windows::Foundation::Numerics::float2 size = { 0.2f, 0.2f });
		virtual ~Button2D();

	public:
		void DoUpdate(DX::StepTimer const &timer) override {};
		void OnLabelChanged() override;

		bool OnGetFocus() override;
		bool OnLostFocus() override;

	public:
		std::string const GetLabel() const override { return std::string(_label.begin(), _label.end()); }
	};
}
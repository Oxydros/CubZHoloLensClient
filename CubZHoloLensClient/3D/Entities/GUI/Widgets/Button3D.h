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

	public:
		Button3D(std::shared_ptr<DX::DeviceResources> devicesResources, std::shared_ptr<HolographicScene> scene,
			Windows::Foundation::Numerics::float3 size = { 0.2f, 0.2f });
		virtual ~Button3D() = default;

	public:
		void DoUpdate(DX::StepTimer const &timer) override {};

		bool OnGetFocus() override;
		bool OnLostFocus() override;

	public:
		std::string const GetLabel() const override { return std::string(_label.begin(), _label.end()); }
	};
}
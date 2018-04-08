#pragma once

#include <3D\Entities\GUI\GUIEntity.h>

using namespace Windows::Foundation::Numerics;

namespace HoloLensClient
{
	class Panel : public GUIEntity
	{
	protected:
		float2					_size;
		float4					_color;

	public:
		Panel(std::shared_ptr<DX::DeviceResources> &devicesResources,
			  std::shared_ptr<HolographicScene> &scene,
			  float2 size, float4 color);
		virtual ~Panel() = default;

	public:
		// Inherited via Entity
		virtual void DoUpdate(DX::StepTimer const & timer) override;

		// Inherited via GUIEntity
		virtual void OnAirTap() override final {};

		std::string const GetLabel() const override { return "Panel"; }
	};
}
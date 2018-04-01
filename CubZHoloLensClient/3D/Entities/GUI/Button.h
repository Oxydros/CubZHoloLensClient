#pragma once

#include <3D\Entities\Common\InteractableEntity.h>

namespace HoloLensClient
{
	class Button : public InteractableEntity
	{
	protected:
		std::function<void()>			_callback = nullptr;

	public:
		Button(std::shared_ptr<DX::DeviceResources> &devicesResources, std::shared_ptr<HolographicScene> &scene,
			   std::function<void()> const &callback = {},
			   Windows::Foundation::Numerics::float2 size = { 0.2f, 0.2f });
		virtual ~Button() = default;

	public:
		void setCallback(std::function<void()> &callback) { _callback = callback; }

	public:
		// Inherited via InteractableEntity
		void DoUpdate(DX::StepTimer const & timer) override { InteractableEntity::DoUpdate(timer); };
		void OnAirTap() override;
	};
}
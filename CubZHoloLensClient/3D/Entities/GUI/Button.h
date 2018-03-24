#pragma once

#include <3D\Entities\Common\InteractableEntity.h>

namespace HoloLensClient
{
	class Button : public InteractableEntity
	{
	protected:
		std::function<void()>			_callback;

	public:
		Button(std::shared_ptr<HolographicScene> &scene, std::function<void()> &callback,
			   Windows::Foundation::Numerics::float2 size = { 0.5f, 0.5f });
		virtual ~Button() = default;

	public:
		void setCallback(std::function<void()> &callback) { _callback = callback; }

	public:
		// Inherited via InteractableEntity
		void DoUpdate(DX::StepTimer const & timer) override {};
		void OnAirTap() override;
	};
}
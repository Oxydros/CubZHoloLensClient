#pragma once

#include <3D\Entities\Common\Entity.h>

namespace HoloLensClient
{
	class Panel : public Entity
	{
	public:
		Panel(std::shared_ptr<HolographicScene> &scene);
		virtual ~Panel() = default;

	public:
		// Inherited via Entity
		virtual void DoUpdate(DX::StepTimer const & timer) override;
	};
}
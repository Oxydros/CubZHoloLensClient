#pragma once

#include <3D\Entities\Common\Entity.h>

namespace HoloLensClient
{
	class EntityRoot : public Entity
	{
	public:
		EntityRoot(std::shared_ptr<HolographicScene> scene) : Entity(scene) {}
		~EntityRoot() = default;
		// Inherited via Entity
		void DoUpdate(DX::StepTimer const & timer) override {};

		// Inherited via Entity
		std::string const GetLabel() const override { return "RootNode"; }
	};
}
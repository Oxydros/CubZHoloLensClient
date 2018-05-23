#pragma once

#include <3D\Entities\Common\Entity.h>

namespace HoloLensClient
{
	class EmptyEntity : public Entity
	{
	private:
		std::string _label;
	public:
		EmptyEntity(std::shared_ptr<HolographicScene> scene, std::string const &label,
			bool isRoot = false) : Entity(scene), _label(label)
		{
			_isRoot = isRoot;
		}

		~EmptyEntity() = default;
		// Inherited via Entity
		void DoUpdate(DX::StepTimer const & timer) override {};

		// Inherited via Entity
		std::string const GetLabel() const override { return _label; }
	};
}
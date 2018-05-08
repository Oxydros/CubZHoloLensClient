#pragma once

#include <3D\Entities\GUI\Panel.h>
#include <3D\Entities\GUI\Button.h>

using namespace Windows::Foundation::Numerics;

namespace HoloLensClient
{
	class EntityMenu : public Entity
	{
	private:
		std::shared_ptr<DX::DeviceResources>	_devicesResources;

	public:
		EntityMenu(std::shared_ptr<DX::DeviceResources> devicesResources,
			std::shared_ptr<HolographicScene> scene);
		virtual ~EntityMenu();

	public:
		void InitializeMenu();

		// Inherited via Entity
		std::string const GetLabel() const override { return "EntityMenu"; };
		void DoUpdate(DX::StepTimer const & timer) override {};
	};
}
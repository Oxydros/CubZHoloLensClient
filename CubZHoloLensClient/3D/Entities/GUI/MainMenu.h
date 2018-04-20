#pragma once

#include <3D\Entities\GUI\Panel.h>
#include <3D\Entities\GUI\Button.h>

using namespace Windows::Foundation::Numerics;

namespace HoloLensClient
{
	class MainMenu : public Entity
	{
	private:
		std::shared_ptr<DX::DeviceResources>	_devicesResources;

	public:
		MainMenu(std::shared_ptr<DX::DeviceResources> &devicesResources,
				 std::shared_ptr<HolographicScene> &scene);
		virtual ~MainMenu();

	public:
		void InitializeMenu();

		// Inherited via Entity
		std::string const GetLabel() const override { return "MainMenu"; };
		void DoUpdate(DX::StepTimer const & timer) override {};
	};
}
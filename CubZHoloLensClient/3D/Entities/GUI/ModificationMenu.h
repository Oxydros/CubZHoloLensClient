#pragma once
#include <3D\Entities\GUI\Panel.h>
#include <3D\Entities\GUI\Button.h>

using namespace Windows::Foundation::Numerics;

namespace HoloLensClient
{
	class ModificationMenu
	{
	private:
		std::unique_ptr<Panel>					_background;

		std::unique_ptr<Button>					_button1;
		std::unique_ptr<Button>					_button2;
	public:
		ModificationMenu();
		virtual ~ModificationMenu();

	public:
		void InitializeMenu(std::shared_ptr<DX::DeviceResources> &devicesResources,
							std::shared_ptr<HolographicScene> &scene);
	};
}
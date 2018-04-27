#pragma once

#include <3D\Entities\GUI\Panel.h>
#include <3D\Entities\GUI\Button.h>
#include <3D\Entities\Common\EditableEntity.h>

using namespace Windows::Foundation::Numerics;

namespace HoloLensClient
{
	class ModificationMenu : public Entity
	{
	private:
		std::shared_ptr<DX::DeviceResources>	_devicesResources;
		std::function<void()>					_callbackMove{ nullptr };
		std::function<void()>					_callbackRotateL{ nullptr };
		std::function<void()>					_callbackRotateR{ nullptr };
		EditableEntity							*_attachedEntity{ nullptr };

	public:
		ModificationMenu(std::shared_ptr<DX::DeviceResources> devicesResources,
						 std::shared_ptr<HolographicScene> scene);
		virtual ~ModificationMenu();

	public:
		void InitializeMenu();

		// Inherited via Entity
		std::string const GetLabel() const override { return "ModificationMenu"; };
		void DoUpdate(DX::StepTimer const & timer) override;

		void AttachEntity(EditableEntity *entity);
		void DetachEntity();
	};
}
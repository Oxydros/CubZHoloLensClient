#pragma once

#include <3D\Entities\GUI\Widgets\Button3D.h>
#include <3D\Entities\Common\EmptyEntity.h>
#include <3D\Entities\Common\EditableEntity.h>

using namespace Windows::Foundation::Numerics;

namespace HoloLensClient
{
	class ModificationMenu : public Entity
	{

	public:
		enum Mode
		{
			BASIC,
			ADJUST,
			ROTATE,
		};

	private:
		std::shared_ptr<DX::DeviceResources>	_devicesResources;
		Mode									_mode{ BASIC };

		Button3D								*_scaleLeft{ nullptr };
		Button3D								*_scaleRight{ nullptr };
		Button3D								*_scaleTop{ nullptr };
		Button3D								*_scaleBot{ nullptr };

		EmptyEntity								*_basicMenu{ nullptr };
		EmptyEntity								*_adjustMenu{ nullptr };
		EmptyEntity								*_rotateMenu{ nullptr };

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

		// Attach an entity
		void AttachEntity(EditableEntity *entity);
		// Detach an entity
		void DetachEntity();
		// Try to detach an entity
		void TryDetachEntity(EditableEntity *entity);

	private:
		void initializeBasicMenu();
		void initializeAdjustMenu();
		void initializeRotateMenu();
		void initializeAdjustBox();
	};
}
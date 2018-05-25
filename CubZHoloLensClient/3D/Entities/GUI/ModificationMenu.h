#pragma once

#include <3D\Entities\GUI\Button3D.h>
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
		};
	private:
		std::shared_ptr<DX::DeviceResources>	_devicesResources;
		Mode									_mode{ BASIC };

		Button3D								*_scaleLeft{ nullptr };
		Button3D								*_scaleRight{ nullptr };

		EmptyEntity								*_basicMenu{ nullptr };
		EmptyEntity								*_adjustMenu{ nullptr };

		EditableEntity							*_attachedEntity{ nullptr };

		//Button									*_rotateLeft{ nullptr };
		//Button									*_rotateRight{ nullptr };

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

	private:
		void initializeBasicMenu();
		void initializeAdjustMenu();
		void initializeAdjustBox();
	};
}
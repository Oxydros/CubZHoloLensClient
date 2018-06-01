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
			ROTATE,
		};

	private:
		std::shared_ptr<DX::DeviceResources>	_devicesResources;
		Mode									_mode{ BASIC };

		bool									_scaleLeftSelected{ false };
		bool									_scaleRigthSelected{ false };
		bool									_scaleTopSelected{ false };
		bool									_scaleBotSelected{ false };

		bool									_rotateLeftSelected{ false };
		bool									_rotateRigthSelected{ false };
		bool									_rotateTopSelected{ false };
		bool									_rotateBotSelected{ false };

		Button3D								*_scaleLeft{ nullptr };
		Button3D								*_scaleRight{ nullptr };
		Button3D								*_scaleTop{ nullptr };
		Button3D								*_scaleBot{ nullptr };

		Button3D								*_rotateLeft{ nullptr };
		Button3D								*_rotateRight{ nullptr };
		Button3D								*_rotateTop{ nullptr };
		Button3D								*_rotateBot{ nullptr };

		EmptyEntity								*_basicMenu{ nullptr };
		EmptyEntity								*_adjustMenu{ nullptr };

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

	private:
		void initializeBasicMenu();
		void initializeAdjustMenu();
		void initializeAdjustBox();
	};
}
#pragma once

#include "3D\Resources\DeviceResources.h"
#include "3D\Objects\Common\IObject.h"
#include <3D\Entities\Common\IEntity.h>

namespace HoloLensClient
{
	class HolographicScene;

	class Entity : public IEntity
	{
	public:
		typedef std::unique_ptr<Entity>	EntityPtr;

	protected:
		/*std::vector<IObject::IObjectPtr>			_meshs;*/
		IObject::IObjectPtr							_mesh;
		std::vector<IEntity *>						_childs;
		IEntity										*_parent;
		bool										_alive;
		std::shared_ptr<HolographicScene>			_scene;
		bool										_useTranslationMatrix;
		bool										_useRotationMatrix;

		bool										_followGazePosition;
		bool										_followGazeRotation;
		Windows::Foundation::Numerics::float3		_positionOffsetFromGaze;
		Windows::Foundation::Numerics::float3		_rotationOffsetFromGaze;

		Windows::Foundation::Numerics::float3		_relativePosition;
		Windows::Foundation::Numerics::float3		_relativeRotation;
		Windows::Foundation::Numerics::float3		_realPosition;
		Windows::Foundation::Numerics::float3		_realRotation;

	public:
		Entity(std::shared_ptr<HolographicScene> &scene);
		virtual ~Entity();

	public:
		void Update(DX::StepTimer const &timer) override final;
		void Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState) override final;
		void InitializeMesh() override;
		void ReleaseMesh() override;
		void Render() override;
		void kill() override;
		bool isDead() const override;

		void Move(Windows::Foundation::Numerics::float3 offset) override;

		void SetRealPosition(Windows::Foundation::Numerics::float3 position) override;
		void SetRealRotation(Windows::Foundation::Numerics::float3 rotation) override;

		void SetRelativePosition(Windows::Foundation::Numerics::float3 position) override;
		void SetRelativeRotation(Windows::Foundation::Numerics::float3 rotation) override;

		void SetRealPosition(DirectX::XMMATRIX &positionMatrix) override;
		void SetRealRotation(DirectX::XMMATRIX &rotationMatrix) override;

		Windows::Foundation::Numerics::float3 const &GetPosition() const override { return _realPosition; };
		Windows::Foundation::Numerics::float3 const &GetRotation() const override { return _realRotation; };

		Windows::Foundation::Numerics::float3 const &GetRelativePosition() const override { return _relativePosition; };
		Windows::Foundation::Numerics::float3 const &GetRelativeRotation() const override { return _relativeRotation; };

		DirectX::XMMATRIX const &GetPositionMatrix() const override { return _mesh->getPositionMatrix(); };
		DirectX::XMMATRIX const &GetRotationMatrix() const override { return _mesh->getRotationMatrix(); };

		void AddChild(IEntity *child) override;
		void RemoveChild(IEntity *child) override;
		void SetParent(IEntity *parent) override;
		IEntity *getParent() const override;

		void setFollowGaze(bool followGazePosition, bool followGazeRotation,
						   Windows::Foundation::Numerics::float3 positionOffsets = { 0.0f, 0.0f, 0.0f },
						   Windows::Foundation::Numerics::float3 rotationOffsets = { 0.0f, 0.0f, 0.0f }) override
		{
			_followGazePosition = followGazePosition;
			_followGazeRotation = followGazeRotation;
			_positionOffsetFromGaze = positionOffsets;
			_rotationOffsetFromGaze = rotationOffsets;
		}

		void positionInFrontOfGaze(Windows::Foundation::Numerics::float3 offsets) override;
		void rotateTowardGaze(Windows::Foundation::Numerics::float3 offsets) override;

	protected:
		void addMesh(IObject::IObjectPtr mesh);

	public:
		virtual void DoUpdate(DX::StepTimer const &timer) = 0;
		virtual void OnInputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState) {};

	private:
		inline void UpdateReal();
		inline void UpdateRelative();
	};
}
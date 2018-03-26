#pragma once

#include <3D\Utility\StepTimer.h>

namespace HoloLensClient
{
	class IEntity
	{
	public:
		typedef std::unique_ptr<IEntity> IEntityPtr;
	public:
		virtual ~IEntity() = default;

	public:
		// Public update function
		virtual void Update(DX::StepTimer const &timer) = 0;
		// Public input notification function
		virtual void Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState) = 0;

	public:
		// Entity update function. You need to override this function
		// to implement the logic of this entity
		virtual void DoUpdate(DX::StepTimer const &timer) = 0;
		// Entity input notification function. You need to override
		// this function to receive inputs.
		virtual void OnInputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState) = 0;

	public:

		virtual void InitializeMesh() = 0;
		virtual void ReleaseMesh() = 0;
		virtual void Render() = 0;
		virtual void kill() = 0;
		virtual bool isDead() const = 0;

		// Translate of the offset value
		virtual void Move(Windows::Foundation::Numerics::float3 offset) = 0;

		// Set the relative position
		virtual void SetRelativePosition(Windows::Foundation::Numerics::float3 position) = 0;
		// Set the relative rotation
		virtual void SetRelativeRotation(Windows::Foundation::Numerics::float3 rotation) = 0;

		// Set the real position
		virtual void SetRealPosition(Windows::Foundation::Numerics::float3 position) = 0;
		// Set the real rotation
		virtual void SetRealRotation(Windows::Foundation::Numerics::float3 rotation) = 0;

		// Set the real position matrix
		virtual void SetPosition(DirectX::XMMATRIX &positionMatrix) = 0;
		// Set the real rotation matrix
		virtual void SetRotation(DirectX::XMMATRIX &rotationMatrix) = 0;

		// Return the real position
		virtual Windows::Foundation::Numerics::float3 const &GetPosition() const = 0;
		// Return the real rotation
		virtual Windows::Foundation::Numerics::float3 const &GetRotation() const = 0;

		// Return the relative position to its parent
		virtual Windows::Foundation::Numerics::float3 const &GetRelativePosition() const = 0;
		// Return the relative rotation to its parent
		virtual Windows::Foundation::Numerics::float3 const &GetRelativeRotation() const = 0;

		// Return the real position matrix
		virtual DirectX::XMMATRIX const &GetPositionMatrix() const = 0;
		// Return the real rotation matrix
		virtual DirectX::XMMATRIX const &GetRotationMatrix() const = 0;

		virtual void AddChild(IEntity *child) = 0;
		virtual void RemoveChild(IEntity *child) = 0;

		virtual void SetParent(IEntity *parent) = 0;
		virtual IEntity *getParent() const = 0;
	};
}
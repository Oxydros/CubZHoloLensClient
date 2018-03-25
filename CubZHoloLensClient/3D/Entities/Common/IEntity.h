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
		virtual void Update(DX::StepTimer const &timer) = 0;
		virtual void InitializeMesh() = 0;
		virtual void ReleaseMesh() = 0;
		virtual void Render() = 0;
		virtual void kill() = 0;
		virtual bool isDead() const = 0;

		virtual void Move(Windows::Foundation::Numerics::float3 offset) = 0;

		virtual void SetPosition(Windows::Foundation::Numerics::float3 position) = 0;
		virtual void SetRotation(Windows::Foundation::Numerics::float3 rotation) = 0;

		virtual void SetPosition(DirectX::XMMATRIX positionMatrix) = 0;
		virtual void SetRotation(DirectX::XMMATRIX rotationMatrix) = 0;

		virtual void GetPosition(Windows::Foundation::Numerics::float3 &position) = 0;
		virtual void GetRotation(Windows::Foundation::Numerics::float3 &rotation) = 0;

		virtual void GetPosition(DirectX::XMMATRIX &positionMatrix) = 0;
		virtual void GetRotation(DirectX::XMMATRIX &rotationMatrix) = 0;

		virtual void Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState) = 0;

		virtual void AddChild(IEntity *child) = 0;
		virtual void RemoveChild(IEntity *child) = 0;

		virtual void SetParent(IEntity *parent) = 0;
		virtual IEntity *getParent() const = 0;
	};
}
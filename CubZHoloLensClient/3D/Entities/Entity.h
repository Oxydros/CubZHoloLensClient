#pragma once

#include "3D\Resources\DeviceResources.h"
#include "3D\ObjectsRenderer\ObjectRenderer.h"

namespace HoloLensClient
{
	class HolographicScene;

	class Entity
	{
	public:
		virtual ~Entity(){}

	public:
		virtual void InitializeMesh() = 0;
		virtual void ReleaseMesh() = 0;
		virtual void Render() = 0;
		virtual void Update(DX::StepTimer const &timer) = 0;
		virtual void Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState) = 0;
		virtual ObjectRenderer const &getMesh() const = 0;
		virtual void setPosition(Windows::Foundation::Numerics::float3 pos) = 0;
		virtual Windows::Foundation::Numerics::float3 getPosition() const = 0;
		//virtual bool isDead() = 0;
	};
}
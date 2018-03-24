#pragma once

#include "3D\Resources\DeviceResources.h"
#include "3D\Objects\Common\IObject.h"

namespace HoloLensClient
{
	class HolographicScene;

	class Entity
	{
	protected:
		bool								_alive;
		std::unique_ptr<IObject>			_mesh;
		std::shared_ptr<HolographicScene>	_scene;

	public:
		Entity(std::shared_ptr<HolographicScene> &scene);
		virtual ~Entity(){}

	public:
		virtual void Update(DX::StepTimer const &timer);
		virtual void InitializeMesh();
		virtual void ReleaseMesh();
		virtual void Render();
		virtual void kill();
		virtual Windows::Foundation::Numerics::float3 getPosition() const;
		virtual bool isDead() const;

		virtual void SetPosition(Windows::Foundation::Numerics::float3 position);
		virtual void SetRotation(Windows::Foundation::Numerics::float3 rotation);

		virtual void SetPosition(DirectX::XMMATRIX positionMatrix);
		virtual void SetRotation(DirectX::XMMATRIX rotationMatrix);

		virtual std::unique_ptr<IObject> const &getMesh() const;

	protected:
		void setMesh(std::unique_ptr<IObject> mesh);
		std::unique_ptr<IObject> &getMesh();

	public:
		virtual void DoUpdate(DX::StepTimer const &timer) = 0;
		virtual void Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState) = 0;
	};
}
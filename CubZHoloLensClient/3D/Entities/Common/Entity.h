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
		std::vector<IObject::IObjectPtr>			_meshs;
		std::vector<IEntity *>						_childs;
		IEntity										*_parent;
		bool										_alive;
		std::shared_ptr<HolographicScene>			_scene;

		Windows::Foundation::Numerics::float3		_position;
		Windows::Foundation::Numerics::float3		_rotation;

	public:
		Entity(std::shared_ptr<HolographicScene> &scene);
		virtual ~Entity();

	public:
		void Update(DX::StepTimer const &timer) override;
		void InitializeMesh() override;
		void ReleaseMesh() override;
		void Render() override;
		void kill() override;
		bool isDead() const override;

		void Move(Windows::Foundation::Numerics::float3 offset) override;

		void SetPosition(Windows::Foundation::Numerics::float3 position) override;
		void SetRotation(Windows::Foundation::Numerics::float3 rotation) override;

		void SetPosition(DirectX::XMMATRIX positionMatrix) override;
		void SetRotation(DirectX::XMMATRIX rotationMatrix) override;

		void GetPosition(Windows::Foundation::Numerics::float3 &position) override;
		void GetRotation(Windows::Foundation::Numerics::float3 &rotation) override;

		void GetPosition(DirectX::XMMATRIX &positionMatrix) override;
		void GetRotation(DirectX::XMMATRIX &rotationMatrix) override;

		void AddChild(IEntity *child) override;
		void RemoveChild(IEntity *child) override;
		void SetParent(IEntity *parent) override;
		IEntity *getParent() const override;

	protected:
		void addMesh(IObject::IObjectPtr mesh);

	public:
		virtual void DoUpdate(DX::StepTimer const &timer) = 0;
	};
}
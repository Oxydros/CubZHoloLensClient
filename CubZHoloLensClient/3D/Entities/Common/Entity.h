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
		IObject::IObjectPtr							_mesh;
		std::vector<IEntity *>						_childs;
		IEntity										*_parent = nullptr;
		bool										_alive = true;
		std::shared_ptr<HolographicScene>			_scene = nullptr;

		bool										_followGazePosition = false;
		bool										_followGazeRotation = false;
		Windows::Foundation::Numerics::float3		_positionOffsetFromGaze = { 0, 0, 0 };
		Windows::Foundation::Numerics::float3		_rotationOffsetFromGaze = { 0, 0, 0 };

		Windows::Foundation::Numerics::float3		_relativePosition = { 0, 0, 0 };;
		Windows::Foundation::Numerics::float3		_relativeRotation = { 0, 0, 0 };
		Windows::Foundation::Numerics::float3		_realPosition = { 0, 0, 0 };
		Windows::Foundation::Numerics::float3		_realRotation = { 0, 0, 0 };

		DirectX::XMMATRIX							_modelTranslation = {};
		DirectX::XMMATRIX							_modelRotation = {};

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

		DirectX::XMMATRIX const &GetPositionMatrix() const override { return _modelRotation; };
		DirectX::XMMATRIX const &GetRotationMatrix() const override { return _modelTranslation; };

		DirectX::XMMATRIX const GetTransformMatrix() const override;

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
		/*inline void UpdateReal();*/
		/*inline void UpdateRelative();*/

	public:
		// Required for align of 16B for XMMAtrix
		// https://stackoverflow.com/questions/20104815/warning-c4316-object-allocated-on-the-heap-may-not-be-aligned-16
		void* operator new(size_t i)
		{
			return _mm_malloc(i, 16);
		}

		// Required for align of 16B for XMMAtrix
		// https://stackoverflow.com/questions/20104815/warning-c4316-object-allocated-on-the-heap-may-not-be-aligned-16
		void operator delete(void* p)
		{
			_mm_free(p);
		}
	};
}
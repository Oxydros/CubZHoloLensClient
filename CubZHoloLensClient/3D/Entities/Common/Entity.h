#pragma once

#include <vector>
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
		IObject::IObjectPtr							_mesh{ nullptr };
		std::vector<IEntity::IEntityPtr>			_newChilds;
		std::vector<IEntity::IEntityPtr>			_childs;
		IEntity										*_parent{ nullptr };
		bool										_alive{ true };
		std::shared_ptr<HolographicScene>			_scene = nullptr;
		bool										_visible{ true };
		bool										_focused{ false };
		bool										_inGaze{ false };
		bool										_isRoot{ false };
		bool										_ignoreInGaze{ false };
		MotionCallback								_motionCallback{ nullptr };

		bool										_followGazePosition{ false };
		bool										_followGazeRotation{ false };
		Windows::Foundation::Numerics::float3		_positionOffsetFromGaze = { 0, 0, 0 };
		Windows::Foundation::Numerics::float3		_rotationOffsetFromGaze = { 0, 0, 0 };

		Windows::Foundation::Numerics::float3		_relativePosition = { 0, 0, 0 };
		Windows::Foundation::Numerics::float3		_relativeRotation = { 0, 0, 0 };
		Windows::Foundation::Numerics::float3		_scaling = { 0, 0, 0 };
		Windows::Foundation::Numerics::float3		_originalSize = { 0, 0, 0 };

		DirectX::XMMATRIX							_modelTranslation = {};
		DirectX::XMMATRIX							_modelRotation = {};
		DirectX::XMMATRIX							_modelScaling = {};
		float										_distance{ 0 };

	public:
		Entity(std::shared_ptr<HolographicScene> scene);
		virtual ~Entity();

	public:
		void Update(DX::StepTimer const &timer) override final;
		void Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState) override final;

		void InitializeMesh() override;
		void ReleaseMesh() override;
		void Render() override;
		void kill() override;
		bool isDead() const override;
		void setVisible(bool visibility);
		bool isVisible() const override { return (_visible); }
		bool isRoot() const override { return (_isRoot); }
		bool IgnoreInGaze() const { return (_ignoreInGaze); }
		void SetIgnoreInGaze(bool ignore) { _ignoreInGaze = ignore; }

		void setMotionCallback(MotionCallback callback) { _motionCallback = callback; }

		void Move(Windows::Foundation::Numerics::float3 offset) override;
		void Rotate(Windows::Foundation::Numerics::float3 offset) override;
		void Scale(Windows::Foundation::Numerics::float3 offset) override;

		void SetScale(Windows::Foundation::Numerics::float3 scale) override;

		Windows::Foundation::Numerics::float3 GetSize() const override;
		void SetSize(Windows::Foundation::Numerics::float3 originalSize) override { _originalSize = originalSize; }

		void SetRelativeX(float x) override { _relativePosition.x = x; }
		void SetRelativeY(float y) override { _relativePosition.y = y; }
		void SetRelativeZ(float z) override { _relativePosition.z = z; }

		void SetRealPosition(Windows::Foundation::Numerics::float3 position) override;
		void SetRealRotation(Windows::Foundation::Numerics::float3 rotation) override;

		void SetRelativePosition(Windows::Foundation::Numerics::float3 position) override;
		void SetRelativeRotation(Windows::Foundation::Numerics::float3 rotation) override;

		void SetModelPosition(DirectX::XMMATRIX &positionMatrix) override;
		void SetModelRotation(DirectX::XMMATRIX &rotationMatrix) override;

		Windows::Foundation::Numerics::float3 const GetRealPosition() const override;
		Windows::Foundation::Numerics::float3 const GetRealRotation() const override;

		Windows::Foundation::Numerics::float3 const &GetRelativePosition() const override { return _relativePosition; };
		Windows::Foundation::Numerics::float3 const &GetRelativeRotation() const override { return _relativeRotation; };

		DirectX::XMMATRIX const &GetPositionMatrix() const override { return _modelRotation; };
		DirectX::XMMATRIX const &GetRotationMatrix() const override { return _modelTranslation; };

		DirectX::XMMATRIX const GetTransformMatrix() const override;

		void AddChild(IEntity::IEntityPtr child) override;
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

		bool isInGaze() const override { return (_inGaze); };
		bool isFocused() const override { return (_focused); };

		void setFocus(bool focused) override;

		void getInGazeEntities(std::vector<IEntity*> &entities) override final;
		std::pair<IEntity*, float> getNearestInGazeEntity() override final;

	public:
		/// <summary>	Executes the get focus action. </summary>
		virtual bool OnGetFocus() { return false; }

		/// <summary>	Executes the lost focus action. </summary>
		virtual bool OnLostFocus() { return false; }

		/// <summary>	Notify the new position of the gaze at every frame when possible. </summary>
		virtual void GazeMotion(Windows::Foundation::Numerics::float3 gazePosition) 
		{
			if (_motionCallback)
				_motionCallback(this, gazePosition);
		}

	protected:

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds a mesh. </summary>
		///
		/// <param name="mesh">	The mesh. </param>
		///-------------------------------------------------------------------------------------------------
		void addMesh(IObject::IObjectPtr mesh);
		void updateInGaze();

	public:
		virtual void DoUpdate(DX::StepTimer const &timer) = 0;
		virtual void OnInputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState) {};

	private:
		/*inline void UpdateReal();*/
		/*inline void UpdateRelative();*/

	public:

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Required for align of 16B for XMMAtrix
		/// 	https://stackoverflow.com/questions/20104815/warning-c4316-object-allocated-on-the-heap-may-not-be-aligned-16.
		/// </summary>
		///
		/// <param name="i">	Zero-based index of the. </param>
		///-------------------------------------------------------------------------------------------------
		void* operator new(size_t i)
		{
			return _mm_malloc(i, 16);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Required for align of 16B for XMMAtrix
		/// 	https://stackoverflow.com/questions/20104815/warning-c4316-object-allocated-on-the-heap-may-not-be-aligned-16.
		/// </summary>
		///
		/// <param name="p">	[in,out] If non-null, the p to delete. </param>
		///
		/// <returns>	The result of the operation. </returns>
		///-------------------------------------------------------------------------------------------------
		void operator delete(void* p)
		{
			_mm_free(p);
		}
	};
}

std::ostream& operator<<(std::ostream& stream, const DirectX::XMMATRIX& matrix);
std::ostream& operator<<(std::ostream& stream, const Windows::Foundation::Numerics::float3 a);
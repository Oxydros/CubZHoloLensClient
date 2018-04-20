///-------------------------------------------------------------------------------------------------
// file:	3D\Entities\Common\IEntity.h
//
// summary:	Declares the IEntity interface
///-------------------------------------------------------------------------------------------------
#pragma once

#include <3D\Utility\StepTimer.h>


///-------------------------------------------------------------------------------------------------
// namespace: HoloLensClient
//
// summary:	.
///-------------------------------------------------------------------------------------------------
namespace HoloLensClient
{
	/// <summary>	An entity. </summary>
	class IEntity
	{
	public:
		typedef std::unique_ptr<IEntity> IEntityPtr;
	public:
		/// <summary>	Destructor. </summary>
		virtual ~IEntity() = default;

	public:

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Update the entity based on the elapsed time. </summary>
		///
		/// <param name="timer">	StepTimer object used to get the elapsed time since the last frame. </param>
		///-------------------------------------------------------------------------------------------------
		virtual void Update(DX::StepTimer const &timer) = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Inputs the given pointer state. </summary>
		///
		/// <param name="pointerState">	State of the pointer. </param>
		///-------------------------------------------------------------------------------------------------
		virtual void Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState) = 0;

	public:

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Entity update function. You need to override this function to implement the logic of this
		/// 	entity.
		/// </summary>
		///
		/// <param name="timer">	StepTimer object used to get the elapsed time since the last frame. </param>
		///-------------------------------------------------------------------------------------------------
		virtual void DoUpdate(DX::StepTimer const &timer) = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Entity input notification function. You need to override this function to receive inputs.
		/// </summary>
		///
		/// <param name="pointerState">	State of the pointer. </param>
		///-------------------------------------------------------------------------------------------------
		virtual void OnInputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState) = 0;

	public:

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Return the entity name. </summary>
		///
		/// <returns>	The label. </returns>
		///-------------------------------------------------------------------------------------------------
		virtual std::string const GetLabel() const = 0;

		/// <summary>	Initialize the mesh. </summary>
		virtual void InitializeMesh() = 0;
		/// <summary>	Release the resources of the mesh. </summary>
		virtual void ReleaseMesh() = 0;
		/// <summary>	Render the mesh. </summary>
		virtual void Render() = 0;
		/// <summary>	Mark this entity for death. </summary>
		virtual void kill() = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Check if this entity is marked for death. </summary>
		///
		/// <returns>	True if dead, false if not. </returns>
		///-------------------------------------------------------------------------------------------------
		virtual bool isDead() const = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Translate of the offset value. </summary>
		///
		/// <param name="offset">	The offset. </param>
		///-------------------------------------------------------------------------------------------------
		virtual void Move(Windows::Foundation::Numerics::float3 offset) = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Set the relative position. </summary>
		///
		/// <param name="position">	The position. </param>
		///-------------------------------------------------------------------------------------------------
		virtual void SetRelativePosition(Windows::Foundation::Numerics::float3 position) = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Set the relative rotation. </summary>
		///
		/// <param name="rotation">	The rotation. </param>
		///-------------------------------------------------------------------------------------------------
		virtual void SetRelativeRotation(Windows::Foundation::Numerics::float3 rotation) = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Set the real position. </summary>
		///
		/// <param name="position">	The position. </param>
		///-------------------------------------------------------------------------------------------------
		virtual void SetRealPosition(Windows::Foundation::Numerics::float3 position) = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Set the real rotation. </summary>
		///
		/// <param name="rotation">	The rotation. </param>
		///-------------------------------------------------------------------------------------------------
		virtual void SetRealRotation(Windows::Foundation::Numerics::float3 rotation) = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Set the real position matrix. </summary>
		///
		/// <param name="positionMatrix">	[in,out] The position matrix. </param>
		///-------------------------------------------------------------------------------------------------
		virtual void SetRealPosition(DirectX::XMMATRIX &positionMatrix) = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Set the real rotation matrix. </summary>
		///
		/// <param name="rotationMatrix">	[in,out] The rotation matrix. </param>
		///-------------------------------------------------------------------------------------------------
		virtual void SetRealRotation(DirectX::XMMATRIX &rotationMatrix) = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Return the real position. </summary>
		///
		/// <returns>	The position. </returns>
		///-------------------------------------------------------------------------------------------------
		virtual Windows::Foundation::Numerics::float3 const &GetPosition() const = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Return the real rotation. </summary>
		///
		/// <returns>	The rotation. </returns>
		///-------------------------------------------------------------------------------------------------
		virtual Windows::Foundation::Numerics::float3 const &GetRotation() const = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Return the relative position to its parent. </summary>
		///
		/// <returns>	The relative position. </returns>
		///-------------------------------------------------------------------------------------------------
		virtual Windows::Foundation::Numerics::float3 const &GetRelativePosition() const = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Return the relative rotation to its parent. </summary>
		///
		/// <returns>	The relative rotation. </returns>
		///-------------------------------------------------------------------------------------------------
		virtual Windows::Foundation::Numerics::float3 const &GetRelativeRotation() const = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Return the real position matrix. </summary>
		///
		/// <returns>	The position matrix. </returns>
		///-------------------------------------------------------------------------------------------------
		virtual DirectX::XMMATRIX const &GetPositionMatrix() const = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Return the real rotation matrix. </summary>
		///
		/// <returns>	The rotation matrix. </returns>
		///-------------------------------------------------------------------------------------------------
		virtual DirectX::XMMATRIX const &GetRotationMatrix() const = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets transform matrix. </summary>
		///
		/// <returns>	The transform matrix. </returns>
		///-------------------------------------------------------------------------------------------------
		virtual DirectX::XMMATRIX const GetTransformMatrix() const = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Add a child entity. </summary>
		///
		/// <param name="child">	[in,out] If non-null, the child. </param>
		///-------------------------------------------------------------------------------------------------
		virtual void AddChild(IEntity::IEntityPtr child) = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Remove a child entity. </summary>
		///
		/// <param name="child">	[in,out] If non-null, the child. </param>
		///-------------------------------------------------------------------------------------------------
		virtual void RemoveChild(IEntity *child) = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Set the parent of this entity. </summary>
		///
		/// <param name="parent">	[in,out] If non-null, the parent. </param>
		///-------------------------------------------------------------------------------------------------
		virtual void SetParent(IEntity *parent) = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Get the parent of this entity. </summary>
		///
		/// <returns>	Null if it fails, else the parent. </returns>
		///-------------------------------------------------------------------------------------------------
		virtual IEntity *getParent() const = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Define if the entity must follow the user gaze at each frame. </summary>
		///
		/// <param name="followGaze">	  	True to follow gaze. </param>
		/// <param name="followOrient">   	True to follow orient. </param>
		/// <param name="positionOffsets">	The position offsets. </param>
		/// <param name="rotationOffsets">	The rotation offsets. </param>
		///-------------------------------------------------------------------------------------------------
		virtual void setFollowGaze(bool followGaze, bool followOrient,
								   Windows::Foundation::Numerics::float3 positionOffsets,
								   Windows::Foundation::Numerics::float3 rotationOffsets) = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Place the entity in front of the user gaze. </summary>
		///
		/// <param name="offsets">	The offsets. </param>
		///-------------------------------------------------------------------------------------------------
		virtual void positionInFrontOfGaze(Windows::Foundation::Numerics::float3 offsets) = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Place the entity in front of the user gaze No use of offsets for now (vector Matrix)
		/// </summary>
		///
		/// <param name="offsets">	The offsets. </param>
		///-------------------------------------------------------------------------------------------------
		virtual void rotateTowardGaze(Windows::Foundation::Numerics::float3 offsets) = 0;
	};
}
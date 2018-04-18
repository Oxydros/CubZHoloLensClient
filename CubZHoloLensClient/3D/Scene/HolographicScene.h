///-------------------------------------------------------------------------------------------------
// file:	3D\Scene\HolographicScene.h
//
// summary:	Declares the holographic scene class
///-------------------------------------------------------------------------------------------------
#pragma once

#include <3D\Resources\DeviceResources.h>
#include "3D\Entities\Common\IEntity.h"


///-------------------------------------------------------------------------------------------------
// namespace: HoloLensClient
//
// summary:	.
///-------------------------------------------------------------------------------------------------
namespace HoloLensClient
{
	/// <summary>	A holographic scene. </summary>
	class HolographicScene : public std::enable_shared_from_this<HolographicScene>
	{
	public:
		/// <summary>	Defines an alias representing the shared pointer. </summary>
		typedef std::shared_ptr<HolographicScene>				SharedPtr;

	private:
		/// <summary>	The coordinate system. </summary>
		Windows::Perception::Spatial::SpatialCoordinateSystem^	_coordinateSystem;
		/// <summary>	The pointer pose. </summary>
		Windows::UI::Input::Spatial::SpatialPointerPose^		_pointerPose;
		/// <summary>	The device resources. </summary>
		std::shared_ptr<DX::DeviceResources>					_deviceResources;
		/// <summary>	The entities. </summary>
		std::vector<IEntity::IEntityPtr>						_entities;
		/// <summary>	The new entities. </summary>
		std::vector<IEntity::IEntityPtr>						_newEntities;

	public:

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Constructor. </summary>
		///
		/// <param name="deviceResources">	[in,out] The device resources. </param>
		///-------------------------------------------------------------------------------------------------
		HolographicScene(std::shared_ptr<DX::DeviceResources> &deviceResources);
		/// <summary>	Destructor. </summary>
		~HolographicScene();

	public:
		/// <summary>	Initializes this object. </summary>
		void Initialize();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Updates the given timer. </summary>
		///
		/// <param name="timer">	The timer. </param>
		///-------------------------------------------------------------------------------------------------
		void Update(DX::StepTimer const& timer);
		/// <summary>	Renders this object. </summary>
		void Render();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Updates the coordinate system described by coordinateSystem. </summary>
		///
		/// <param name="coordinateSystem">	The coordinate system. </param>
		///-------------------------------------------------------------------------------------------------
		void UpdateCoordinateSystem(Windows::Perception::Spatial::SpatialCoordinateSystem^ coordinateSystem);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Updates the pointer pose described by pointerPose. </summary>
		///
		/// <param name="pointerPose">	The pointer pose. </param>
		///-------------------------------------------------------------------------------------------------
		void UpdatePointerPose(Windows::UI::Input::Spatial::SpatialPointerPose ^pointerPose);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Inputs the given pointer state. </summary>
		///
		/// <param name="pointerState">	State of the pointer. Null if no input is detected </param>
		///-------------------------------------------------------------------------------------------------
		void Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState);
		/// <summary>	Executes the device lost action. </summary>
		void OnDeviceLost();
		/// <summary>	Executes the device restored action. </summary>
		void OnDeviceRestored();

	public:

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets device resources. </summary>
		///
		/// <returns>	The device resources. </returns>
		///-------------------------------------------------------------------------------------------------
		std::shared_ptr<DX::DeviceResources> getDeviceResources() const { return (_deviceResources); };

	public:

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds an entity. </summary>
		///
		/// <param name="e">	An IEntity::IEntityPtr to process. </param>
		///-------------------------------------------------------------------------------------------------
		void addEntity(IEntity::IEntityPtr e);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds an entity in front to 'dist'. </summary>
		///
		/// <param name="e">   	An IEntity::IEntityPtr to process. </param>
		/// <param name="dist">	(Optional) The distance. </param>
		///-------------------------------------------------------------------------------------------------
		void addEntityInFront(IEntity::IEntityPtr e, float dist = 2.0f);

	public:

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets coordinate system. </summary>
		///
		/// <returns>	Nullptr if it fails, else the coordinate system. </returns>
		///-------------------------------------------------------------------------------------------------
		Windows::Perception::Spatial::SpatialCoordinateSystem^ getCoordinateSystem() const;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets pointer pose. </summary>
		///
		/// <returns>	Nullptr if it fails, else the pointer pose. </returns>
		///-------------------------------------------------------------------------------------------------
		Windows::UI::Input::Spatial::SpatialPointerPose^ getPointerPose() const;
	};
}


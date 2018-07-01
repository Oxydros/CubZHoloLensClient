///-------------------------------------------------------------------------------------------------
// file:	3D\Scene\HolographicScene.h
//
// summary:	Declares the holographic scene class
///-------------------------------------------------------------------------------------------------
#pragma once

#include <3D\Resources\DeviceResources.h>
#include "3D\Entities\Common\EmptyEntity.h"
#include "3D\Entities\GUI\Menus\MainMenu.h"
#include "3D\Entities\GUI\Menus\ModificationMenu.h"
#include <3D\Input\InteractionListener.h>
#include <Objects\HoloLensContext.h>


///-------------------------------------------------------------------------------------------------
// namespace: HoloLensClient
//
// summary:	.
///-------------------------------------------------------------------------------------------------
namespace HoloLensClient
{
	/// <summary>	A holographic scene. </summary>
	class HolographicScene : public std::enable_shared_from_this<HolographicScene>, public InteractionListener
	{
	public:
		/// <summary>	Defines an alias representing the shared pointer. </summary>
		typedef std::shared_ptr<HolographicScene>				SharedPtr;

	private:
		//Scene status
		bool													_alive{ true };
		/// <summary>	The coordinate system. </summary>
		Windows::Perception::Spatial::SpatialCoordinateSystem^	_coordinateSystem;
		/// <summary>	The pointer pose. </summary>
		Windows::UI::Input::Spatial::SpatialPointerPose^		_pointerPose;
		/// <summary>	The device resources. </summary>
		std::shared_ptr<DX::DeviceResources>					_deviceResources;
		/// <summary>	Root node. </summary>
		std::unique_ptr<EmptyEntity>							_root;

		/// <summary>	The modif menu. </summary>
		ModificationMenu										*_modifMenu{ nullptr };
		/// <summary>	The main menu. </summary>
		MainMenu												*_mainMenu{ nullptr };
		/// <summary>	User cursor (head direction). </summary>
		IEntity													*_cursor{ nullptr };
		/// <summary>	Entity currently focused (nearest) </summary>
		IEntity													*_focusedEntity{ nullptr };

		Windows::Foundation::EventRegistrationToken				_updateEntityEventToken;
		Windows::Foundation::EventRegistrationToken				_entityEventToken;

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

		/// <summary>	Executes the device lost action. </summary>
		void OnDeviceLost();
		/// <summary>	Executes the device restored action. </summary>
		void OnDeviceRestored();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Query if this object is alive. </summary>
		///
		/// <returns>	True if alive, false if not. </returns>
		///-------------------------------------------------------------------------------------------------
		bool isAlive() const { return _alive; };
		/// <summary>	Kills this object. </summary>
		void kill() { _alive = false; }

		void UpdateEntity(CubZHoloLensClient::WinNetwork::EntityDescription entity, CubZHoloLensClient::WinNetwork::SpaceDescription space);
		void CreateDeleteEntity(CubZHoloLensClient::WinNetwork::EntityAction action, CubZHoloLensClient::WinNetwork::EntityDescription entity);

	public:

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets device resources. </summary>
		///
		/// <returns>	The device resources. </returns>
		///-------------------------------------------------------------------------------------------------
		std::shared_ptr<DX::DeviceResources> getDeviceResources() const { return (_deviceResources); };

	public:
		void InteractionDetectedEvent(
			Windows::UI::Input::Spatial::SpatialInteractionManager^ sender,
			Windows::UI::Input::Spatial::SpatialInteractionDetectedEventArgs^ args) override;

	public:

		ModificationMenu * getModificationMenu() const { return _modifMenu; }
		MainMenu * getMainMenu() const { return _mainMenu; }
		IEntity *getCursor() const { return _cursor; }

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


///-------------------------------------------------------------------------------------------------
// file:	3D\Scene\SceneRenderer.h
//
// summary:	Declares the scene renderer class
///-------------------------------------------------------------------------------------------------
#pragma once

#include "3D\Resources\DeviceResources.h"
#include "3D\Utility\StepTimer.h"
#include "3D\Input\SpatialInputHandler.h"
#include "3D\Scene\HolographicScene.h"


///-------------------------------------------------------------------------------------------------
// namespace: HoloLensClient
//
// summary:	.
///-------------------------------------------------------------------------------------------------
namespace HoloLensClient
{
    /// <summary>	Updates, renders, and presents holographic scenes using Direct3D. </summary>
    class SceneRenderer : public DX::IDeviceNotify
    {
	private:

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Clears event registration state. Used when changing to a new HolographicSpace and when
		/// 	tearing down AppMain.
		/// </summary>
		///-------------------------------------------------------------------------------------------------
		void UnregisterHolographicEventHandlers();

		bool															_alive{ true };

		/// <summary>	Listens for the Pressed spatial input event. </summary>
		std::shared_ptr<SpatialInputHandler>                            m_spatialInputHandler;

		/// <summary>	Cached pointer to device resources. </summary>
		std::shared_ptr<DX::DeviceResources>                            m_deviceResources;

		/// <summary>	Render loop timer. </summary>
		DX::StepTimer                                                   m_timer;

		/// <summary>	Represents the holographic space around the user. </summary>
		Windows::Graphics::Holographic::HolographicSpace^               m_holographicSpace;

		/// <summary>	SpatialLocator that is attached to the primary camera. </summary>
		Windows::Perception::Spatial::SpatialLocator^                   m_locator;

		/// <summary>	A reference frame attached to the holographic camera. </summary>
		Windows::Perception::Spatial::SpatialStationaryFrameOfReference^ m_referenceFrame;

		/// <summary>	Event registration tokens. </summary>
		Windows::Foundation::EventRegistrationToken                     m_cameraAddedToken;
		/// <summary>	The camera removed token. </summary>
		Windows::Foundation::EventRegistrationToken                     m_cameraRemovedToken;
		/// <summary>	The locatability changed token. </summary>
		Windows::Foundation::EventRegistrationToken                     m_locatabilityChangedToken;

		/// <summary>	The holo scene. </summary>
		std::shared_ptr<HolographicScene>								_holoScene;

    public:

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Constructor. </summary>
		///
		/// <param name="deviceResources">	The device resources. </param>
		///-------------------------------------------------------------------------------------------------
		SceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
        /// <summary>	Destructor. </summary>
        ~SceneRenderer();

        ///-------------------------------------------------------------------------------------------------
        /// <summary>
        /// 	Sets the holographic space. This is our closest analogue to setting a new window for the
        /// 	app.
        /// </summary>
        ///
        /// <param name="holographicSpace">	The holographic space. </param>
        ///-------------------------------------------------------------------------------------------------
        void SetHolographicSpace(Windows::Graphics::Holographic::HolographicSpace^ holographicSpace);

        ///-------------------------------------------------------------------------------------------------
        /// <summary>	Starts the holographic frame and updates the content. </summary>
        ///
        /// <returns>
        /// 	Nullptr if it fails, else a Windows::Graphics::Holographic::HolographicFrame^.
        /// </returns>
        ///-------------------------------------------------------------------------------------------------
        Windows::Graphics::Holographic::HolographicFrame^ Update();

        ///-------------------------------------------------------------------------------------------------
        /// <summary>	Renders holograms, including world-locked content. </summary>
        ///
        /// <param name="holographicFrame">	The holographic frame. </param>
        ///
        /// <returns>	True if it succeeds, false if it fails. </returns>
        ///-------------------------------------------------------------------------------------------------
        bool Render(Windows::Graphics::Holographic::HolographicFrame^ holographicFrame);

        /// <summary>	Handle saving and loading of app state owned by AppMain. </summary>
        void SaveAppState();
        /// <summary>	Loads application state. </summary>
        void LoadAppState();

        /// <summary>	IDeviceNotify. </summary>
        virtual void OnDeviceLost();
        /// <summary>	Executes the device restored action. </summary>
        virtual void OnDeviceRestored();

		bool	isAlive() const { return _alive; }

		void OnListFile(Windows::Foundation::Collections::IVector<Platform::String^>^fileList);

    private:

        ///-------------------------------------------------------------------------------------------------
        /// <summary>	Asynchronously creates resources for new holographic cameras. </summary>
        ///
        /// <param name="sender">	The sender. </param>
        /// <param name="args">  	The arguments. </param>
        ///-------------------------------------------------------------------------------------------------
        void OnCameraAdded(
            Windows::Graphics::Holographic::HolographicSpace^ sender,
            Windows::Graphics::Holographic::HolographicSpaceCameraAddedEventArgs^ args);

        ///-------------------------------------------------------------------------------------------------
        /// <summary>
        /// 	Synchronously releases resources for holographic cameras that are no longer attached to
        /// 	the system.
        /// </summary>
        ///
        /// <param name="sender">	The sender. </param>
        /// <param name="args">  	The arguments. </param>
        ///-------------------------------------------------------------------------------------------------
        void OnCameraRemoved(
            Windows::Graphics::Holographic::HolographicSpace^ sender,
            Windows::Graphics::Holographic::HolographicSpaceCameraRemovedEventArgs^ args);

        ///-------------------------------------------------------------------------------------------------
        /// <summary>	Used to notify the app when the positional tracking state changes. </summary>
        ///
        /// <param name="sender">	The sender. </param>
        /// <param name="args">  	The arguments. </param>
        ///-------------------------------------------------------------------------------------------------
        void OnLocatabilityChanged(
            Windows::Perception::Spatial::SpatialLocator^ sender,
            Platform::Object^ args);
    };
}

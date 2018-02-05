#pragma once

#include "3D\Resources\DeviceResources.h"
#include "3D\Utility\StepTimer.h"
#include "3D\SpatialInputHandler.h"
#include "3D\Scene\HolographicScene.h"

namespace HoloLensClient
{
	// Updates, renders, and presents holographic scenes using Direct3D.
    class SceneRenderer : public DX::IDeviceNotify
    {
	private:
		// Clears event registration state. Used when changing to a new HolographicSpace
		// and when tearing down AppMain.
		void UnregisterHolographicEventHandlers();

		// Listens for the Pressed spatial input event.
		std::shared_ptr<SpatialInputHandler>                            m_spatialInputHandler;

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources>                            m_deviceResources;

		// Render loop timer.
		DX::StepTimer                                                   m_timer;

		// Represents the holographic space around the user.
		Windows::Graphics::Holographic::HolographicSpace^               m_holographicSpace;

		// SpatialLocator that is attached to the primary camera.
		Windows::Perception::Spatial::SpatialLocator^                   m_locator;

		// A reference frame attached to the holographic camera.
		Windows::Perception::Spatial::SpatialStationaryFrameOfReference^ m_referenceFrame;

		// Event registration tokens.
		Windows::Foundation::EventRegistrationToken                     m_cameraAddedToken;
		Windows::Foundation::EventRegistrationToken                     m_cameraRemovedToken;
		Windows::Foundation::EventRegistrationToken                     m_locatabilityChangedToken;

		std::unique_ptr<HolographicScene>								_holoScene;

    public:
		SceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
        ~SceneRenderer();

        // Sets the holographic space. This is our closest analogue to setting a new window
        // for the app.
        void SetHolographicSpace(Windows::Graphics::Holographic::HolographicSpace^ holographicSpace);

        // Starts the holographic frame and updates the content.
        Windows::Graphics::Holographic::HolographicFrame^ Update();

        // Renders holograms, including world-locked content.
        bool Render(Windows::Graphics::Holographic::HolographicFrame^ holographicFrame);

        // Handle saving and loading of app state owned by AppMain.
        void SaveAppState();
        void LoadAppState();

        // IDeviceNotify
        virtual void OnDeviceLost();
        virtual void OnDeviceRestored();

    private:
        // Asynchronously creates resources for new holographic cameras.
        void OnCameraAdded(
            Windows::Graphics::Holographic::HolographicSpace^ sender,
            Windows::Graphics::Holographic::HolographicSpaceCameraAddedEventArgs^ args);

        // Synchronously releases resources for holographic cameras that are no longer
        // attached to the system.
        void OnCameraRemoved(
            Windows::Graphics::Holographic::HolographicSpace^ sender,
            Windows::Graphics::Holographic::HolographicSpaceCameraRemovedEventArgs^ args);

        // Used to notify the app when the positional tracking state changes.
        void OnLocatabilityChanged(
            Windows::Perception::Spatial::SpatialLocator^ sender,
            Platform::Object^ args);
    };
}

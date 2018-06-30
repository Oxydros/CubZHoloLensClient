#pragma once

namespace HoloLensClient
{
	class InteractionListener
	{
	public:
		InteractionListener() = default;
		virtual ~InteractionListener() = default;

	public:
		virtual void InteractionDetectedEvent(
			Windows::UI::Input::Spatial::SpatialInteractionManager^ sender,
			Windows::UI::Input::Spatial::SpatialInteractionDetectedEventArgs^ args) = 0;
	};
}
#include "pch.h"
#include "Panel.h"

HoloLensClient::Panel::Panel(std::shared_ptr<HolographicScene> &scene)
	: Entity(scene)
{
}

void HoloLensClient::Panel::DoUpdate(DX::StepTimer const & timer)
{
}

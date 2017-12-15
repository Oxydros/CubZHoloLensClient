#include "pch.h"
#include "Objects\UserDevice.h"

using namespace CubZHoloLensClient;

UserDevice::UserDevice(Platform::String ^name, DeviceType type)
	: _name(name), _type(type)
{
}

Platform::String ^UserDevice::Name::get()
{
	return (_name);
}

DeviceType UserDevice::Type::get()
{
	return (_type);
}
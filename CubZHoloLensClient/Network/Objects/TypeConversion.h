#pragma once

#include <TypeDescription.pb.h>
#include <Utility\Utility.h>
#include <Network\Objects\TypeDescription.h>

namespace CubZHoloLensClient
{
	namespace WinNetwork
	{
		//https://stackoverflow.com/questions/33960999/protobuf-will-set-allocated-delete-the-allocated-object
		CubZPacket::FileDescription *UWPFileDescriptionToNative(WinNetwork::FileDescription const &fileDesc);
		WinNetwork::FileDescription NativeFileDescriptionToUWP(CubZPacket::FileDescription const &fileDesc);
		CubZPacket::EntityDescription *UWPEntityDescriptionToNative(WinNetwork::EntityDescription const &entityDesc);
		WinNetwork::EntityDescription NativeEntityDescriptionToUWP(CubZPacket::EntityDescription const &entityDesc);
		CubZPacket::SpaceDescription *UWPSpaceDescriptionToNative(WinNetwork::SpaceDescription const &spaceDesc);
		WinNetwork::SpaceDescription NativeSpaceDescriptionToUWP(CubZPacket::SpaceDescription const &spaceDesc);
	}
}
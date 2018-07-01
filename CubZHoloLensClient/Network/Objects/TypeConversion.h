#pragma once

#include <TypeDescription.pb.h>
#include <Utility\Utility.h>
#include <Network\Objects\TypeDescription.h>

namespace CubZHoloLensClient
{
	namespace WinNetwork
	{
		//https://stackoverflow.com/questions/33960999/protobuf-will-set-allocated-delete-the-allocated-object
		CubZPacket::FileDescription *UWPFileDescriptionToNative(WinNetwork::FileDescription const &fileDesc)
		{
			CubZPacket::FileDescription *result = new CubZPacket::FileDescription();

			result->set_type(CubZPacket::FileDescription_Type(fileDesc.type));
			result->set_name(Utility::platformStringToString(fileDesc.name));
			result->set_extension(Utility::platformStringToString(fileDesc.extension));
			result->set_path(Utility::platformStringToString(fileDesc.path));
			result->set_size(fileDesc.size);
			result->set_rights(fileDesc.rights);

			return (result);
		}

		WinNetwork::FileDescription NativeFileDescriptionToUWP(CubZPacket::FileDescription const &fileDesc)
		{
			WinNetwork::FileDescription result =
			{
				WinNetwork::FileType(fileDesc.type()),
					Utility::stringToPlatformString(fileDesc.name()),
					Utility::stringToPlatformString(fileDesc.extension()),
					Utility::stringToPlatformString(fileDesc.path()),
					fileDesc.size(),
					fileDesc.rights()
			};
			return (result);
		}

		CubZPacket::EntityDescription *UWPEntityDescriptionToNative(WinNetwork::EntityDescription const &entityDesc)
		{
			CubZPacket::EntityDescription *result = new CubZPacket::EntityDescription();

			result->set_type(CubZPacket::EntityDescription_Type(entityDesc.type));
			result->set_allocated_file(UWPFileDescriptionToNative(entityDesc.fileDescription));
			result->set_id(entityDesc.id);

			return (result);
		}

		WinNetwork::EntityDescription NativeEntityDescriptionToUWP(CubZPacket::EntityDescription const &entityDesc)
		{
			WinNetwork::EntityDescription result =
			{
				WinNetwork::EntityType(entityDesc.type()),
				NativeFileDescriptionToUWP(entityDesc.file()),
				entityDesc.id()
			};
			return (result);
		}

		CubZPacket::SpaceDescription *UWPSpaceDescriptionToNative(WinNetwork::SpaceDescription const &spaceDesc)
		{
			CubZPacket::SpaceDescription *result = new CubZPacket::SpaceDescription();

			result->set_x(spaceDesc.position.x);
			result->set_y(spaceDesc.position.y);
			result->set_z(spaceDesc.position.z);

			result->set_rx(spaceDesc.rotation.x);
			result->set_ry(spaceDesc.rotation.y);
			result->set_rz(spaceDesc.rotation.z);

			result->set_sx(spaceDesc.scale.x);
			result->set_sy(spaceDesc.scale.y);
			result->set_sz(spaceDesc.scale.z);

			return (result);
		}

		WinNetwork::SpaceDescription NativeSpaceDescriptionToUWP(CubZPacket::SpaceDescription const &spaceDesc)
		{
			WinNetwork::SpaceDescription result =
			{
				Windows::Foundation::Numerics::float3(spaceDesc.x(), spaceDesc.y(), spaceDesc.z()),
				Windows::Foundation::Numerics::float3(spaceDesc.rx(), spaceDesc.ry(), spaceDesc.rz()),
				Windows::Foundation::Numerics::float3(spaceDesc.sx(), spaceDesc.sy(), spaceDesc.sz())
			};
			return (result);
		}
	}
}
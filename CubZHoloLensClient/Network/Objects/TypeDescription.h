#pragma once

namespace CubZHoloLensClient
{
	namespace WinNetwork
	{
		
		public enum class FileType : int
		{
			DIRECTORY = 0,
			FILE = 1
		};

		public enum class EntityType : int
		{
			OBJ = 0,
			CUBE,
			SPHERE
		};

		public enum class ModelType : int
		{
			PHONE = 0,
			COMPUTER,
			HOLOLENS,
			UNKNOWNs
		};

		public value struct FileDescription
		{
			FileType				type;
			Platform::String^		name;
			Platform::String^		extension;
			Platform::String^		path;
			unsigned int			size;
			unsigned int			rights;
		};

		public value struct UserDescription
		{
			unsigned int			id;
			Platform::String		^userName;
			bool					gender;
			Platform::String		^careerPosition;
		};

		public value struct RuleDescription
		{
			UserDescription			user;
			unsigned int			rights;
		};

		public value struct SceneDescription
		{
			Platform::String		^pathScene;
			Platform::String		^sceneName;
		};

		public value struct DeviceDescription
		{
			ModelType				model;
			Platform::String		^name;
		};

		public value struct SpaceDescription
		{
			Windows::Foundation::Numerics::float3	position;
			Windows::Foundation::Numerics::float3	rotation;
			Windows::Foundation::Numerics::float3	scale;
		};

		public value struct EntityDescription
		{
			EntityType				type;
			FileDescription			fileDescription;
			int						id;
		};

		//function for operator == between two FileDescription
		struct equal_FileDescription
		{
			constexpr bool operator()(const WinNetwork::FileDescription& _Left, WinNetwork::FileDescription& _Right)
			{
				if (_Left.type == _Right.type && _Left.type == _Right.type && _Left.name == _Right.name && _Left.extension == _Right.extension && _Left.size == _Right.size && _Left.rights == _Right.rights) {
					return (true);
				}
				else {
					return (false);
				}

			}
		};
	}
}
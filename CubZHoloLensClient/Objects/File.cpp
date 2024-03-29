#include "pch.h"
#include "File.h"

using namespace CubZHoloLensClient;

File::File(Platform::String ^name, FileType type)
	: _name(name), _type(type)
{
}

Platform::String ^File::Name::get()
{
	return (_name);
}

FileType File::Type::get()
{
	return (_type);
}

Platform::String ^File::TypeName::get()
{
	return (_type == FileType::UNKNOWN ? "Unknown" : (_type == FileType::DIRECTORY ? "Directory" : "File"));
}
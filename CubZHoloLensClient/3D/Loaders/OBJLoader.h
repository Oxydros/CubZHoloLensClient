#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace Windows::Foundation::Numerics;

namespace HoloLensClient
{
	class OBJLoader
	{
	public:
		typedef std::pair<std::vector<DirectX::XMFLOAT3>, std::vector<unsigned short>> OBJReturnValue;
	public:
		static Concurrency::task<OBJReturnValue> LoadFromFile(std::wstring fileName,
															  bool RHCoordSystem = false);

	private:
		static void ParseFaces(std::string const &line);
	};
}
#include "pch.h"
#include <3D\Utility\DirectXHelper.h>
#include "OBJLoader.h"

using namespace HoloLensClient;

// http://www.rastertek.com/dx11tut08.html
// https://en.wikipedia.org/wiki/Wavefront_.obj_file
// https://www.gamedev.net/forums/topic/611078-need-help-with-obj-loader-in-directx11/
// 
Concurrency::task<OBJLoader::OBJReturnValue> OBJLoader::LoadFromFile(std::wstring fileName,
																		bool RHCoordSystem)
{
	auto readTask = DX::ReadTextFileAsync(fileName);

	auto r = readTask.then(
		[&](std::string text) -> OBJLoader::OBJReturnValue
	{
		std::vector<DirectX::XMFLOAT3> vertices;
		std::vector<unsigned short> indices;

		std::istringstream stream(text);
		std::string line;

		while (std::getline(stream, line))
		{
			std::stringstream lineStream(line);
			std::string lineType;

			lineStream >> lineType;

			TRACE("Got line " << line << " type is " << lineType << std::endl);

			if (lineType == "v")
			{
				float3 vertex;
				lineStream >> vertex.x >> vertex.y >> vertex.z;
				if (RHCoordSystem) {
					vertex.z *= -1.0f;
				}
				/*TRACE("Pushing " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl);*/
				vertices.push_back(DirectX::XMFLOAT3(vertex.x, vertex.y, vertex.z));
			}
			else if (lineType == "f")
			{
				ParseFaces(line);
			}
		}
		return std::make_pair(vertices, indices);
	});
	return (r);
}

void HoloLensClient::OBJLoader::ParseFaces(std::string const &_line)
{
	//char skip;
	//int x, y, z;

	////Format is "f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3"
	//lineStream
	//	>> x >> skip >> skip >> skip >> skip
	//	>> y >> skip >> skip >> skip >> skip
	//	>> z;
	//TRACE("Pushing " << x << " " << y << " " << z << std::endl);
	//x -= 1;
	//y -= 1;
	//z -= 1;
	//indices.push_back(x);
	//indices.push_back(y);
	//indices.push_back(z);

	std::string line = _line;
	std::string::iterator new_end = std::unique(line.begin(), line.end(),
		[](char lhs, char rhs) -> bool
	{
		return (lhs == rhs) && (lhs == ' ');
	});
	line.erase(new_end, line.end());

	std::stringstream stream(line), countVerticesStream(line);
	std::string val;
	stream >> val;
	if (val != "f")
	{
		throw std::logic_error("Error loading face at line ");
	}

	// Count the number of vertices per face by counting the /
	int nVertices = 0;
	while (countVerticesStream.good())
	{
		if (countVerticesStream.get() == ' ' && countVerticesStream.good())
			nVertices++;
	}

	int indexPosition = 0, indexTexture = 0, indexNormal = 0;

	for (unsigned int iFace = 0; iFace < nVertices; iFace++)
	{
		stream >> indexPosition;
		if ('/' == stream.peek())
		{
			stream.ignore();
			if ('/' != stream.peek())
			{
				stream >> indexTexture;
			}
			if ('/' == stream.peek())
			{
				stream.ignore();
				// Optional vertex normal
				stream >> indexNormal;
			}
		}
	}
}

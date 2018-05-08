#pragma once

#include <3D\Loaders\tiny_obj_loader.h>
#include "3D\Shaders\ShaderStructures.h"
#include "3D\Objects\Mesh\Common\ColoredObject.h"

namespace HoloLensClient
{
	class OBJMesh : public ColoredObject
	{
	private:
		tinyobj::attrib_t					_attrib;
		std::vector<tinyobj::shape_t>		_shapes;
		std::vector<tinyobj::material_t>	_materials;

		std::string							_meshFile;

	public:
		OBJMesh(std::shared_ptr<DX::DeviceResources> devicesResources,
				std::string const &meshFile,
				Windows::Foundation::Numerics::float4 color = { 1.0f, 1.0f, 1.0f, 1.0f });
		~OBJMesh();

	private:
		void CreateMesh() override;
	};
}


#pragma once

#include <DirectXMath.h>
#include "3D\Shaders\ShaderStructures.h"
#include "3D\Objects\SceneObject.h"

namespace HoloLensClient
{
	class ColoredCircle : public SceneObject
	{
	private:
		Windows::Foundation::Numerics::float3           _color;
		float											_radius;
		int												_precision;
		float											_radians;

	public:
		ColoredCircle(std::shared_ptr<DX::DeviceResources> &devicesResources,
			float radius = 0.25f,
			float radians = DirectX::XM_2PI,
			Windows::Foundation::Numerics::float3 color = { 1.0f, 1.0f, 1.0f });
		~ColoredCircle();

	private:
		void CreateMesh() override;
		std::vector<float3> CreateCircle(float3 center, float radius, int divisions, float radians);
	};
}


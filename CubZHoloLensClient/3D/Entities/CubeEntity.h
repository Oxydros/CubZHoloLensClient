#pragma once

#include "3D\Entities\Entity.h"
#include "3D\ObjectsRenderer\CubeRenderer.h"

namespace HoloLensClient
{
	class CubeEntity : public Entity
	{
	private:
		std::shared_ptr<HolographicScene>	_scene;
		CubeRenderer						_mesh;

	public:
		CubeEntity(std::shared_ptr<DX::DeviceResources> devicesResources, std::shared_ptr<HolographicScene> scene);
		~CubeEntity();

	public:
		void InitializeMesh() override;
		void ReleaseMesh() override;
		void Render() override;
		void Update(DX::StepTimer const &timer) override;
		void Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState) override;
		ObjectRenderer const &getMesh() const override;

		virtual void setPosition(Windows::Foundation::Numerics::float3 pos) override;
		virtual Windows::Foundation::Numerics::float3 getPosition() const override;
	};
}
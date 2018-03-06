#pragma once

#include "3D\Entities\Entity.h"
#include "3D\ObjectsRenderer\GazeRenderer.h"

namespace HoloLensClient
{
	class GazeEntity : public Entity
	{
	private:
		std::shared_ptr<HolographicScene>	_scene;
		GazeRenderer						_mesh;

	public:
		GazeEntity(std::shared_ptr<DX::DeviceResources> devicesResources, std::shared_ptr<HolographicScene> scene);
		~GazeEntity();

	public:
		void InitializeMesh() override;
		void ReleaseMesh() override;
		void Render() override;
		void Update(DX::StepTimer const &timer) override;
		void Inputs(Windows::UI::Input::Spatial::SpatialInteractionSourceState^ pointerState) override;
		void setPosition(Windows::Foundation::Numerics::float3 pos) override {};
		Windows::Foundation::Numerics::float3 getPosition() const override { return (_mesh.getPosition()); };
		ObjectRenderer const &getMesh() const override;
	};
}
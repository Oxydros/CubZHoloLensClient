#pragma once

#include <3D\Objects\GUI\Interfaces\ITextObject.h>
#include <3D\Objects\Mesh\2DFormes\ColoredRectangle.h>
#include <3D\Objects\GUI\TextObject.h>

namespace HoloLensClient
{
	class ButtonObject : public ITextObject
	{
	protected:
		std::unique_ptr<TextObject>					_textMesh;
		std::unique_ptr<ColoredRectangle>			_backgroundMesh;

		std::shared_ptr<DX::DeviceResources>		_deviceResources;

		Windows::Foundation::Numerics::float2		_size;
		Windows::Foundation::Numerics::float3		_position = { 0.0f, 0.0f, 0.0f };
		Windows::Foundation::Numerics::float3		_rotation = { 0.0f, 0.0f, 0.0f };
		Windows::Foundation::Numerics::float3		_scale = { 1.0f, 1.0f, 1.0f };
		Windows::Foundation::Numerics::float4		_color = { 1.0f, 1.0f, 1.0f, 1.0f };

	public:
		ButtonObject(std::shared_ptr<DX::DeviceResources> &devicesResources,
					 Windows::Foundation::Numerics::float2 size,
					 Windows::Foundation::Numerics::float4 backgroundColor = { 0.243f, 0.815f, 0.854f, 1.0f });
		~ButtonObject() = default;

		// Inherited via ITextObject
		void CreateDeviceDependentResources() override;
		void ReleaseDeviceDependentResources() override;
		void Update() override;
		void Render() override;
		void ApplyMatrix(DirectX::XMMATRIX const & modelTransform) override;
		void Translate(Windows::Foundation::Numerics::float3 translation) override;
		void SetPosition(Windows::Foundation::Numerics::float3 pos) override;
		void SetRotation(Windows::Foundation::Numerics::float3 rot) override;
		void SetScale(Windows::Foundation::Numerics::float3 scale) override;
		Windows::Foundation::Numerics::float3 GetPosition() const override;
		Windows::Foundation::Numerics::float3 GetRotation() const override;
		Windows::Foundation::Numerics::float3 GetScale() const override;
		void SetPosition(DirectX::XMMATRIX const & posMatrix) override;
		void SetRotation(DirectX::XMMATRIX const & rotMatrix) override;
		DirectX::XMMATRIX const & getPositionMatrix() const override;
		DirectX::XMMATRIX const & getRotationMatrix() const override;
		std::shared_ptr<DX::DeviceResources> getDeviceResources() const override;
		void GetBoundingBox(DirectX::BoundingOrientedBox & boundingBox) override;

	public:
		std::wstring const & getText() const override;
		void setText(std::wstring const & text) override;
	};
}
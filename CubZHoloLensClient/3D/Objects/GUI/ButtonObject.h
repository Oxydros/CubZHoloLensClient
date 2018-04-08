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
		Windows::Foundation::Numerics::float4		_color = { 1.0f, 1.0f, 1.0f, 1.0f };

	public:
		ButtonObject(std::shared_ptr<DX::DeviceResources> &devicesResources,
					 Windows::Foundation::Numerics::float2 size,
					 Windows::Foundation::Numerics::float4 backgroundColor = { 0.243f, 0.815f, 0.854f, 1.0f });
		~ButtonObject() = default;

		// Inherited via ITextObject
		void CreateDeviceDependentResources() override;
		void ReleaseDeviceDependentResources() override;
		void Render() override;
		void SetModelTransform(DirectX::XMMATRIX const & modelTransform) override;
		std::shared_ptr<DX::DeviceResources> getDeviceResources() const override;
		void GetBoundingBox(DirectX::BoundingOrientedBox & boundingBox) override;

	public:
		std::wstring const & getText() const override;
		void setText(std::wstring const & text) override;

		float const getFontSize() const override;
		void setFontSize(float font) override;

		void setColor(Windows::Foundation::Numerics::float4) override;
		Windows::Foundation::Numerics::float4 getColor() const override;
	};
}
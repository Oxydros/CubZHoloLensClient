#include <pch.h>
#include <3D\Resources\Texture2D.h>

using namespace HoloLensClient;

Texture2D::Texture2D(std::shared_ptr<DX::DeviceResources> &deviceResources,
					 std::string const &fileName)
	: _deviceResources(deviceResources), _fileName(fileName)
{
}

Texture2D::~Texture2D()
{

}

Microsoft::WRL::ComPtr<ID3D11Texture2D> HoloLensClient::Texture2D::getTexture() const
{
	return _texture;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> HoloLensClient::Texture2D::getTextureView() const
{
	return _textureView;
}

Concurrency::task<void> HoloLensClient::Texture2D::Load()
{
	Concurrency::task<std::vector<byte>> loadTextureTask = DX::ReadDataAsync(std::wstring(_fileName.begin(), _fileName.end()));

	return loadTextureTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			DX::CreateWICTextureFromMemory(
				_deviceResources->GetD3DDevice(),
				&fileData[0],
				fileData.size(),
				_textureResource.ReleaseAndGetAddressOf(),
				nullptr));
		DX::ThrowIfFailed(_textureResource.As(&_texture));
		D3D11_TEXTURE2D_DESC texDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
		ZeroMemory(&viewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		_texture->GetDesc(&texDesc);
		viewDesc.Format = texDesc.Format;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MostDetailedMip = 0;
		viewDesc.Texture2D.MipLevels = 1;

		DX::ThrowIfFailed(_deviceResources->GetD3DDevice()->CreateShaderResourceView(_textureResource.Get(), &viewDesc, _textureView.ReleaseAndGetAddressOf()));
	});
}

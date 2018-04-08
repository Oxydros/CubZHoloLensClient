#pragma once

#include <d3d11.h>
#include <3D\Resources\DeviceResources.h>
#include <3D\Utility\DirectXHelper.h>

namespace HoloLensClient
{
	class Texture2D : public std::enable_shared_from_this<Texture2D>
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D>				_texture;
		Microsoft::WRL::ComPtr<ID3D11Resource>				_textureResource;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	_textureView;
		std::shared_ptr<DX::DeviceResources>				_deviceResources;
		std::string											_fileName;

	public:
		Texture2D(std::shared_ptr<DX::DeviceResources> &deviceResources, std::string const &fileName);
		~Texture2D();

	public:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> getTexture() const;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> getTextureView() const;

	public:
		Concurrency::task<void> Load();
	};
}
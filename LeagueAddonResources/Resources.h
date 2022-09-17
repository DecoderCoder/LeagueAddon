#pragma once
#include "pch.h"
#include "d3d11.h"
#include "d3dx11tex.h"
#include <dxgi.h>
#include <iostream>
#include "vector"
#include "Images.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")

#ifdef MAKEDLL
#  define EXPORT __declspec(dllexport)
#else
#  define EXPORT __declspec(dllexport)
#endif

extern "C" class EXPORT RImage {
public:
	std::string Name;
	ID3D11ShaderResourceView* Image = nullptr;
	RImage(std::string Name, char* ImageBytes, int Size, ID3D11Device* pDevice) {
		this->Name = Name;

		D3DX11_IMAGE_LOAD_INFO info;
		ID3DX11ThreadPump* pump{ nullptr };

		D3DX11CreateShaderResourceViewFromMemory(pDevice, ImageBytes, Size, &info,
			pump, &this->Image, 0);
	}
};

extern "C" class EXPORT Image_Manager {
public:
	Image_Manager() {

	}

	Image_Manager(ID3D11Device* pDevice) {
		this->pDevice = pDevice;

	}

	int ImagesCount() {
		return this->Images.size();
	}

	void AddImage(std::string Name, char* Bytes, int Size) {
		this->Images.push_back(RImage(Name, Bytes, Size, this->pDevice));
	}

	bool HasImage(std::string name) {
		for (auto obj : this->Images) {
			if (name.compare(obj.Name) == 0)
				return true;
		}
		return false;
	}

	ID3D11ShaderResourceView* GetImageByName(std::string name) {
		for (auto obj : this->Images) {
			if (name.compare(obj.Name) == 0)
			{
				return obj.Image;
			}
		}

		return nullptr;
	}

	std::vector<RImage>* GetVector() {
		return &Images;
	}

private:
	ID3D11Device* pDevice;
	std::vector<RImage> Images;
};

extern "C" EXPORT void InitImages(Image_Manager & Images_Manager);
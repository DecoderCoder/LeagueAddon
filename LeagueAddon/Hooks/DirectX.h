#pragma once
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_win32.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "d3d9.h"
#include "d3d11.h"
#include <dxgi.h>
//#include "Vector.h"
#include "d3dx11tex.h"

typedef HRESULT(WINAPI* PresentDX9) (LPDIRECT3DDEVICE9 Device, CONST RECT* pSrcRect, CONST RECT* pDestRect, HWND hDestWindow, CONST RGNDATA* pDirtyRegion);
typedef HRESULT(__stdcall* PresentDX11) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef HRESULT(__stdcall* ResizeBuffer)(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);

namespace DirectXHook {
	inline bool Inited = false;

	bool HookDX11();
	bool HookDX9();
	bool HookInput();
	bool unHook();
	bool SupportDX9();
	bool SupportDX11();
	

	HRESULT WINAPI Hooked_PresentDX9(LPDIRECT3DDEVICE9 Device, CONST RECT* pSrcRect, CONST RECT* pDestRect, HWND hDestWindow, CONST RGNDATA* pDirtyRegion);
	HRESULT __stdcall Hooked_PresentDX11(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	HRESULT  __stdcall Hooked_ResizeBufferDX11(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
}

#include "DirectX.h"
#include "../Utils.h"
#include <thread>
#include <chrono>
#include <psapi.h>
#include "../EventManager/EventManager.h"
#include <dinput.h>
#include "../Scripts/OrbWalker.h"
#include "../Debug.h"

#pragma comment(lib, "Dinput8.lib")
#pragma comment(lib, "Dxguid.lib")

PresentDX9 oPresentDX9 = NULL;
uint64_t oPresentDX11 = NULL;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;


std::unique_ptr<PLH::VTableSwapHook> h;
std::unique_ptr<PLH::x86Detour> detour;


HWND window = NULL;
ID3D11RenderTargetView* mainRenderTargetView = NULL;
WNDPROC oWndProc = NULL;

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam) {
	DWORD wndProcId;
	GetWindowThreadProcessId(handle, &wndProcId);
	if (GetCurrentProcessId() != wndProcId) {
		return true;
	}
	window = handle;
	return false;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT WINAPI DirectXHook::Hooked_PresentDX9(LPDIRECT3DDEVICE9 Device, CONST RECT* pSrcRect, CONST RECT* pDestRect, HWND hDestWindow, CONST RGNDATA* pDirtyRegion) {

	return ((PresentDX9)oPresentDX9)(Device, pSrcRect, pDestRect, hDestWindow, pDirtyRegion);
}

bool DirectXHook::HookDX9() {
	// TODO: Add DX9 Support
	return false;
}

typedef HRESULT(__stdcall* fGetDeviceState)(IDirectInputDevice8* pThis, DWORD cbData, LPVOID lpvData);
typedef HRESULT(__stdcall* fGetDeviceData)(IDirectInputDevice8*, DWORD, LPDIDEVICEOBJECTDATA, LPDWORD, DWORD);
//std::unique_ptr<PLH::VTableSwapHook> dih;
//
void* oGetDeviceState = NULL;
void* oGetDeviceData = NULL;
HRESULT __stdcall hookGetDeviceData(IDirectInputDevice8* pThis, DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags) {

	//if (result == DI_OK) {
		//for (int i = 0; i < *pdwInOut; ++i) {
		//	if (LOBYTE(rgdod[i].dwData) > 0) { //key down
		//		if (rgdod[i].dwOfs == DIK_Q) {
		//			std::cout << "[q] pressed" << std::endl;
		//		}
		//	}
		//	if (LOBYTE(rgdod[i].dwData) == 0) { //key up
		//		if (rgdod[i].dwOfs == DIK_Q) {
		//			std::cout << "[q] released" << std::endl;
		//		}
		//	}
		//}
		//if (blockKeyboard) {
		//	*pdwInOut = 0; //set array size 0
		//}
	//}
	//MessageBoxA(0, to_string(0).c_str(), "hookGetDeviceData", 0);
	return ((fGetDeviceData)oGetDeviceData)(pThis, cbObjectData, rgdod, pdwInOut, dwFlags);
}

HRESULT __stdcall hookGetDeviceState(IDirectInputDevice8* pThis, DWORD cbData, LPVOID lpvData) {
	//HRESULT result = dih->origFunc<PLH::VFunc<13, fGetDeviceState>>(pThis, cbData, lpvData);

	//if (result == DI_OK) {
		//if (cbData == sizeof(DIMOUSESTATE)) {//caller is a mouse
	/*if (((LPDIMOUSESTATE)lpvData)->rgbButtons[1] != 0) {
		Utils::Log("Right click " + to_string(((LPDIMOUSESTATE)lpvData)->rgbButtons[0]));
	}*/

	//}
	//if (cbData == sizeof(DIMOUSESTATE2)) {//caller is also a mouse but different struct

	//}
	//if (blockMouse) { // I know that in our case the struct is LPDIMOUSESTATE2
	//	((LPDIMOUSESTATE2)lpvData)->rgbButtons[0] = 0;
	//	((LPDIMOUSESTATE2)lpvData)->rgbButtons[1] = 0;
	//}
//}
/*Utils::Log("lpvData->rgbButtons[0]: " + to_string(((LPDIMOUSESTATE)lpvData)->rgbButtons[0]));
Utils::Log("lpvData->rgbButtons[1]: " + to_string(((LPDIMOUSESTATE)lpvData)->rgbButtons[1]));
Utils::Log("lpvData2->rgbButtons[0]: " + to_string(((LPDIMOUSESTATE2)lpvData)->rgbButtons[0]));
Utils::Log("lpvData2->rgbButtons[1]: " + to_string(((LPDIMOUSESTATE2)lpvData)->rgbButtons[1]));*/
	//MessageBoxA(0, to_string(0).c_str(), "hookGetDeviceState", 0);
	return ((fGetDeviceState)oGetDeviceState)(pThis, cbData, lpvData);
	HRESULT result = ((fGetDeviceState)oGetDeviceState)(pThis, cbData, lpvData);
	if (OrbWalker::enabled && GetAsyncKeyState(VK_SPACE)) {
		if (cbData == sizeof(DIMOUSESTATE)) {
			Utils::Log(">>>>>  LPDIMOUSESTATE1:  " + to_string(((LPDIMOUSESTATE)lpvData)->rgbButtons[0]) + " | " + to_string(((LPDIMOUSESTATE)lpvData)->rgbButtons[1]) + " | " + to_string(((LPDIMOUSESTATE)lpvData)->rgbButtons[2]) + " | " + to_string(((LPDIMOUSESTATE)lpvData)->rgbButtons[3]) + " | ");
			((LPDIMOUSESTATE)lpvData)->rgbButtons[0] = 0;
			((LPDIMOUSESTATE)lpvData)->rgbButtons[1] = 0;
			((LPDIMOUSESTATE)lpvData)->rgbButtons[2] = 0;
			((LPDIMOUSESTATE)lpvData)->rgbButtons[3] = 0;
		}
		if (cbData == sizeof(DIMOUSESTATE2)) {
			Utils::Log(">>>>>  LPDIMOUSESTATE2:  " + to_string(((LPDIMOUSESTATE2)lpvData)->rgbButtons[0]) + " | " + to_string(((LPDIMOUSESTATE2)lpvData)->rgbButtons[1]) + " | " + to_string(((LPDIMOUSESTATE2)lpvData)->rgbButtons[2]) + " | " + to_string(((LPDIMOUSESTATE2)lpvData)->rgbButtons[3]) + " | ");
			((LPDIMOUSESTATE2)lpvData)->rgbButtons[0] = 0;
			((LPDIMOUSESTATE2)lpvData)->rgbButtons[1] = 0;
			((LPDIMOUSESTATE2)lpvData)->rgbButtons[2] = 0;
			((LPDIMOUSESTATE2)lpvData)->rgbButtons[3] = 0;
		}
		if (cbData == sizeof(DIJOYSTATE)) {
			Utils::Log(">>>>>  LPDIJOYSTATE1:  " + to_string(((LPDIJOYSTATE)lpvData)->rgbButtons[0]) + " | " + to_string(((LPDIJOYSTATE)lpvData)->rgbButtons[1]) + " | " + to_string(((LPDIJOYSTATE)lpvData)->rgbButtons[2]) + " | " + to_string(((LPDIJOYSTATE)lpvData)->rgbButtons[3]) + " | ");
			((LPDIJOYSTATE)lpvData)->rgbButtons[0] = 0;
			((LPDIJOYSTATE)lpvData)->rgbButtons[1] = 0;
			((LPDIJOYSTATE)lpvData)->rgbButtons[2] = 0;
			((LPDIJOYSTATE)lpvData)->rgbButtons[3] = 0;
		}
		if (cbData == sizeof(DIJOYSTATE2)) {
			Utils::Log(">>>>>  LPDIJOYSTATE2:  " + to_string(((LPDIJOYSTATE2)lpvData)->rgbButtons[0]) + " | " + to_string(((LPDIJOYSTATE2)lpvData)->rgbButtons[1]) + " | " + to_string(((LPDIJOYSTATE2)lpvData)->rgbButtons[2]) + " | " + to_string(((LPDIJOYSTATE2)lpvData)->rgbButtons[3]) + " | ");
			((LPDIJOYSTATE2)lpvData)->rgbButtons[0] = 0;
			((LPDIJOYSTATE2)lpvData)->rgbButtons[1] = 0;
			((LPDIJOYSTATE2)lpvData)->rgbButtons[2] = 0;
			((LPDIJOYSTATE2)lpvData)->rgbButtons[3] = 0;
		}


	}
	return result;

}

void* HookVTableFunction(void* pVTable, void* fnHookFunc, int nOffset) // https://guidedhacking.com/threads/how-to-hook-directinput-emulate-key-presses.14011/
{
	intptr_t ptrVtable = *((intptr_t*)pVTable);
	intptr_t ptrFunction = ptrVtable + sizeof(intptr_t) * nOffset;
	intptr_t ptrOriginal = *((intptr_t*)ptrFunction);
	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery((LPCVOID)ptrFunction, &mbi, sizeof(mbi));
	VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect);
	*((intptr_t*)ptrFunction) = (intptr_t)fnHookFunc;
	VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &mbi.Protect);
	return (void*)ptrOriginal;
}
LPDIRECTINPUT8 pDirectInput = NULL;
LPDIRECTINPUTDEVICE8 lpdiMouse;

//SHORT WINAPI hGetAsyncKeyState(int vKey) {
//	MessageBoxA(0, "GetAsyncKeyState", "", 0);
//	return true;
//}
//
//uint64_t oGetAsyncKeyState = NULL;

bool DirectXHook::HookInput()
{
	Utils::Log("Hook Input: DirectInput8Create Ptr: " + to_hex((int)&DirectInput8Create));


	if (auto error = DirectInput8Create(GetModuleHandle(NULL), 0x0800, IID_IDirectInput8, (VOID**)&pDirectInput, NULL) != DI_OK) {
		Utils::Log("Hook Input: Failed to create DirectInput: " + to_string(error) + " | " + to_string(GetLastError()));
		return false;
	}


	if (pDirectInput->CreateDevice(GUID_SysMouse, &lpdiMouse, NULL) != DI_OK) {
		pDirectInput->Release();
		pDirectInput = NULL;
		Utils::Log("Hook Input: Failed to CreateDevice");
		return false;
	}

	Utils::Log("Hook Input: lpdiMouse: " + to_hex((int)lpdiMouse));
	// TODO: Change to PLH hook SwapVTable or X86Detour	

	/*PLH::VFuncMap redirect = { {9, (uint64_t)&hookGetDeviceState}, {10, (uint64_t)&hookGetDeviceData} };
	std::unique_ptr<PLH::VTableSwapHook> directInputVTableHook;
	directInputVTableHook.reset(new PLH::VTableSwapHook((uint64_t)lpdiMouse, redirect));
	MessageBoxA(0, directInputVTableHook->hook() ? "ok" : "no", "vtablehook", 0);*/
	oGetDeviceState = HookVTableFunction(lpdiMouse, hookGetDeviceState, 9);
	oGetDeviceData = HookVTableFunction(lpdiMouse, hookGetDeviceData, 10);

	Utils::Log("Hook Input: Ok");

	return true;
}

bool DirectXHook::SupportDX9() {
	return (int)GetModuleHandleA("d3d9.dll") >= 0;
}

bool DirectXHook::SupportDX11() {
	return (int)GetModuleHandleA("d3d11.dll") >= 0;
}

bool DirectXHook::HookDX11() {
	Utils::Log(" > Renderer: Init");
	if (Inited)
		return true;
	WNDCLASSEX windowClass;
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = DefWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.hIcon = NULL;
	windowClass.hCursor = NULL;
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = TEXT("Decode");
	windowClass.hIconSm = NULL;
	::RegisterClassEx(&windowClass);
	window = ::CreateWindow(windowClass.lpszClassName, TEXT("Decode"), WS_OVERLAPPEDWINDOW, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, windowClass.hInstance, NULL);
	Utils::Log(" > Renderer: Window created");
	HMODULE libD3D11;
	if ((libD3D11 = ::GetModuleHandle(TEXT("d3d11.dll"))) == NULL)
	{
		::DestroyWindow(window);
		::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
		Utils::Log(" > Renderer: Module not found");
		return false;
	}

	void* D3D11CreateDeviceAndSwapChain;
	if ((D3D11CreateDeviceAndSwapChain = ::GetProcAddress(libD3D11, "D3D11CreateDeviceAndSwapChain")) == NULL)
	{
		::DestroyWindow(window);
		::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
		Utils::Log(" > Renderer: Unknown error D3D11CreateDeviceAndSwapChain");
		return false;
	}

	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };

	DXGI_RATIONAL refreshRate;
	refreshRate.Numerator = 60;
	refreshRate.Denominator = 1;

	DXGI_MODE_DESC bufferDesc;
	bufferDesc.Width = GetSystemMetrics(SM_CXSCREEN);
	bufferDesc.Height = GetSystemMetrics(SM_CYSCREEN);
	bufferDesc.RefreshRate = refreshRate;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SAMPLE_DESC sampleDesc;
	sampleDesc.Count = 1;
	sampleDesc.Quality = 0;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc = sampleDesc;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.Windowed = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* context;

	if (((long(__stdcall*)(
		IDXGIAdapter*,
		D3D_DRIVER_TYPE,
		HMODULE,
		UINT,
		const D3D_FEATURE_LEVEL*,
		UINT,
		UINT,
		const DXGI_SWAP_CHAIN_DESC*,
		IDXGISwapChain**,
		ID3D11Device**,
		D3D_FEATURE_LEVEL*,
		ID3D11DeviceContext**))(D3D11CreateDeviceAndSwapChain))(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, &featureLevel, &context) < 0)
	{
		Utils::Log(" > Renderer: Unknown error D3D11CreateDeviceAndSwapChain2");
		Beep(900, 100);
		::DestroyWindow(window);
		::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
		return false;
	}
	uint32_t* g_methodsTable = (uint32_t*)::calloc(205, sizeof(uint32_t));
	::memcpy(g_methodsTable, *(uint32_t**)swapChain, 18 * sizeof(uint32_t));
	::memcpy(g_methodsTable + 18, *(uint32_t**)device, 43 * sizeof(uint32_t));
	::memcpy(g_methodsTable + 18 + 43, *(uint32_t**)context, 144 * sizeof(uint32_t));
	void* target = (void*)g_methodsTable[8]; // [8]   Present
	Utils::Log(" > Renderer: Starting hooking");
	PLH::CapstoneDisassembler dis(PLH::Mode::x86);
	detour.reset(new PLH::x86Detour((char*)target, (char*)&DirectXHook::Hooked_PresentDX11, &oPresentDX11, dis));
	Utils::Log(" > Renderer: hooking...");
	if (detour->hook()) {
		Utils::Log(" > Renderer: Hook complete");
	}
	else {
		Utils::Log(" > Renderer: Smth went wrong");
	}


	Sleep(500);
	//std::this_thread::sleep_for(500ms);

	while (!Inited) {
		Beep(50, 10);
		Utils::Log(" > Renderer: Rehook");
		detour->reHook();
	}

	Utils::Log(" > Renderer: Hook OK");
	Utils::Log(" > Renderer: Ok");
	return true;
}

bool DirectXHook::unHook() {
	SetWindowLongPtr(GetHwndProc(), GWLP_WNDPROC, (LONG_PTR)oWndProc);
	if (oGetDeviceState)
		HookVTableFunction(lpdiMouse, oGetDeviceState, 9);

	if (oGetDeviceData)
		HookVTableFunction(lpdiMouse, oGetDeviceData, 10);

	if (pDirectInput)
		pDirectInput->Release();
	return detour->unHook() && h->unHook();
}

HRESULT __stdcall DirectXHook::Hooked_PresentDX11(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	//if (GetAsyncKeyState(VK_HOME))
	//	Inited = false;
	if (!Inited)
	{
		Utils::Log(" > DX11Present: Init: Begin");
		if (pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice) >= 0)
		{
			Render::Images_Manager = Image_Manager(pDevice);
			Resources::LoadImages(Render::Images_Manager);
			//Images_Manager = Renderer_Image_Manager(pDevice);
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow; 
			RECT rect;
			GetWindowRect(window, &rect);
			Render::RenderWidth = rect.right - rect.left;
			Render::RenderHeight = rect.bottom - rect.top;

			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			Inited = true;
			Utils::Log(" > DX11Present: Init: GetDevice: Ok");
		}
		else
			return ((PresentDX11)oPresentDX11)(pSwapChain, SyncInterval, Flags);
		Utils::Log(" > DX11Present: Init: ResizeBuffer");
		PLH::VFuncMap redirect = { {13, (uint64_t)&DirectXHook::Hooked_ResizeBufferDX11} };

		h.reset(new PLH::VTableSwapHook((uint64_t)pSwapChain, redirect));
		if (!h->hook()) {
			MessageBoxA(0, "Failed to hook", "", 0);
		}
		Utils::Log(" > DX11Present: Init: ResizeBuffer ok");
		//HookVTableFunction(pSwapChain, &DirectXHook::Hooked_ResizeBufferDX11, 13);

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(pDevice, pContext);
		Utils::Log(" > DX11Present: Init: Ok");
	}
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//	ImGuiStyle* style = &ImGui::GetStyle();
	//	style->WindowPadding = ImVec2(15, 15);
	//	style->WindowRounding = 5.0f;
	//	style->FramePadding = ImVec2(5, 5);
	//	style->FrameRounding = 4.0f;
	//	style->ItemSpacing = ImVec2(12, 8);
	//	style->ItemInnerSpacing = ImVec2(8, 6);
	//	style->IndentSpacing = 25.0f;
	//	style->ScrollbarSize = 15.0f;
	//	style->ScrollbarRounding = 9.0f;
	//	style->GrabMinSize = 5.0f;
	//	style->GrabRounding = 3.0f;
	//
	//	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	//	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	//	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	//	//style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	//	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	//	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	//	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	//	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	//	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	//	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	//	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	//	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	//	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	//	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	//	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	//	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	//	//style->Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	//	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	//	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	//	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	//	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	//	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	//	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	//	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	//	//style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//	//style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	//	//style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	//	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	//	//style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
	//	//style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
	////	style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
	//	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	//	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	//	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	//	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	//	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	//	//style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
	Debug::addonTime = GetTickCount();
	EventManager::Trigger(EventManager::EventType::OnDraw);

	ImGui::Render();
	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return ((PresentDX11)oPresentDX11)(pSwapChain, SyncInterval, Flags);
}

HRESULT  __stdcall DirectXHook::Hooked_ResizeBufferDX11(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	Utils::Log("DX11ResizeBuffer: Begin");
	if (mainRenderTargetView) {
		pContext->OMSetRenderTargets(0, 0, 0);
		mainRenderTargetView->Release();
		Utils::Log("DX11ResizeBuffer: Release");
	}
	HRESULT hr = h->origFunc<PLH::VFunc<13, ResizeBuffer>>(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
	ID3D11Texture2D* pBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
	// Perform error handling here!
	pDevice->CreateRenderTargetView(pBuffer, NULL, &mainRenderTargetView);
	// Perform error handling here!
	pBuffer->Release();
	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	D3D11_VIEWPORT vp;
	vp.Width = Width;
	vp.Height = Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1, &vp);
	Utils::Log("DX11ResizeBuffer: End");
	return hr;
}
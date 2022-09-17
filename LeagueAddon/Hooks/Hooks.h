#pragma once
#include <vector>
#include "../Utils.h"
#include "ultimate_hooks.h"
#include "../Offset.h"

#pragma comment(lib, "D3DX11.lib")
#pragma comment(lib, "D3DX9.lib")



namespace Hooks {
	inline bool Hooked = false;

	namespace DirectX {
		void HookDX();
		void HookInput();
		bool UnHookDX();
	}

	void ApplyHooks();
	void RemoveHooks();
}
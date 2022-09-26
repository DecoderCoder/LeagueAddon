#pragma once
#include <vector>
namespace EventManager {
	enum class EventType {
		OnLoad,
		OnUnload,
		OnThread,
		OnDraw,
		OnMenu,
		OnProcessSpell,
		OnKeyPressed,
		OnDeleteObject,
		Size
	};
	extern std::vector<void*> EventCallbacks[(unsigned int)EventType::Size];
	
	void AddEventHandler(EventType eventId, void* callback);
	void RemoveEventHandler(EventType eventId, void* callback);

	template <typename... Args>
	void Trigger(EventType eventId, Args... args) {
		for (auto callback : EventCallbacks[(unsigned int)eventId]) {
			static_cast<void(*)(Args...)>(callback)(args...);
		}
	}

	template <typename... Args>
	bool TriggerProcess(EventType eventId, Args... args) {
		auto process = true;
		for (auto callback : EventCallbacks[(unsigned int)eventId]) {
			if (!static_cast<bool(*)(Args...)>(callback)(args...)) {
				process = false;
			}
		}
		return process;
	}
}
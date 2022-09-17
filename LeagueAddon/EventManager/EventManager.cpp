#include "EventManager.h"
std::vector<void*> EventManager::EventCallbacks[(unsigned int)EventType::Size];

void EventManager::AddEventHandler(EventType eventId, void* callback) {
	EventCallbacks[(unsigned int)eventId].push_back(callback);
}

void EventManager::RemoveEventHandler(EventType eventId, void* callback) {
	auto& callbacks = EventCallbacks[(unsigned int)eventId];
	for (auto it = callbacks.begin(); it != callbacks.end(); it++) {
		if (*it == callback) {
			callbacks.erase(it);
			break;
		}
	}
}
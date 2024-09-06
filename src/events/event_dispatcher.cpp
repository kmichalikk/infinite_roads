#include "event_dispatcher.h"

void EventDispatcher::subscribe(EventType event, std::function<void(void *)> callback) {
    subscribers[event].push_back(callback);
}

void EventDispatcher::dispatch(EventType event, void *data) {
    for (auto sub : subscribers[event]) {
        sub(data);
    }
}

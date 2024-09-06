#pragma once

#include <functional>
#include <map>
#include <vector>

#include "events.h"


class EventDispatcher {
    std::map<EventType, std::vector<std::function<void(void *)>>> subscribers;
public:
    void subscribe(EventType event, std::function<void(void *)> callback);
    void dispatch(EventType event, void *data);
};

inline EventDispatcher eventDispatcher;

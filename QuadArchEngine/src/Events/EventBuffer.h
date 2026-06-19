#pragma once
#include "QuadArch/Core.h"

#include "EventTypes.h"
#include <vector>

#define EVENT_RESERVATION 256

namespace QuadArch
{
    class EventBuffer {
    public:
        std::vector<KeyEventData> KeyEvents;
        std::vector<MouseBtnEventData> MouseBtnEvents;
        std::vector<MouseMovedEventData> MouseMovedEvents;

        void Init() {
            KeyEvents.reserve(EVENT_RESERVATION);
            MouseBtnEvents.reserve(EVENT_RESERVATION);
            MouseMovedEvents.reserve(EVENT_RESERVATION);
        }

        void Clear() {
            KeyEvents.clear();
            MouseBtnEvents.clear();
            MouseMovedEvents.clear();
        }
    };
}
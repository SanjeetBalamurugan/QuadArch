#pragma once
#include "QuadArch/Core.h"
#include <cstdint>

namespace QuadArch
{
    struct KeyEventData {
        uint16_t Code;
        uint8_t Action; // 0 = Release, 1 = Press, 2 = Repeat
    };

    struct MouseBtnEventData {
        uint8_t Button;
        uint8_t Action;
    };

    struct MouseMovedEventData {
        float X;
        float Y;
    };
}

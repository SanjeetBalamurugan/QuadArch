#include "pch.h"
#include "Mouse.h"

namespace QuadArch {

    float Mouse::s_CurrentX = 0.0f;
    float Mouse::s_CurrentY = 0.0f;

    bool Mouse::s_CurrentButtonStates[8] = { false };
    bool Mouse::s_PreviousButtonStates[8] = { false };

    std::pair<float, float> Mouse::GetMousePos() { return { s_CurrentX, s_CurrentY }; }
    float Mouse::GetMouseX() { return s_CurrentX; }
    float Mouse::GetMouseY() { return s_CurrentY; }

    bool Mouse::IsButtonDown(MouseCode button) {
        return s_CurrentButtonStates[button] && !s_PreviousButtonStates[button];
    }

    bool Mouse::IsButtonHeld(MouseCode button) {
        return s_CurrentButtonStates[button] && s_PreviousButtonStates[button];
    }

    bool Mouse::IsButtonUp(MouseCode button) {
        return !s_CurrentButtonStates[button] && s_PreviousButtonStates[button];
    }

    void Mouse::ClearFrameStates() {
        for (int i = 0; i < 8; i++) {
            s_PreviousButtonStates[i] = s_CurrentButtonStates[i];
        }
    }

    void Mouse::RecordMousePosition(float x, float y) {
        s_CurrentX = x;
        s_CurrentY = y;
    }

    void Mouse::RecordButtonPress(MouseCode button) {
        if (button < 8) s_CurrentButtonStates[button] = true;
    }

    void Mouse::RecordButtonRelease(MouseCode button) {
        if (button < 8) s_CurrentButtonStates[button] = false;
    }
}

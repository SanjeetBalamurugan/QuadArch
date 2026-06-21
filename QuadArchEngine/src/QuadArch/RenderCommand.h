#pragma once
#include "Core.h"
#include <string>

namespace QuadArch
{
	class QuadAPI RenderCommand
	{
	public:
		inline static void SetClearColor(float r, float g, float b, float a);
		inline static void Clear();
		inline static void Blend(bool enable);
		
		inline static void SetWindowTitle(const std::string& title);
		inline static void SetCursorMode(bool hiddenAndLocked);
	};
}

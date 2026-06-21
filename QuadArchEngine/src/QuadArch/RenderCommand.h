#pragma once
#include "Core.h"

namespace QuadArch
{
	class QuadAPI RenderCommand
	{
	public:
		inline static void SetClearColor(float r, float g, float b, float a);
		inline static void Clear();
		inline static void Blend(bool enable);
	};
}

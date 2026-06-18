#pragma once
#include "Core.h"
#include "Window.h"

namespace QuadArch {
	class QuadAPI Application
	{
	public:
		~Application() = default;

		virtual void Init();
		virtual void Update();
		virtual void End();
	private:
		Window* m_Window = nullptr;
	};
}
#pragma once
#include "Core.h"

namespace QuadArch {
	class QuadAPI Application
	{
	public:
		~Application() = default;

		virtual void Init();
		virtual void Update();
		virtual void End();
	};
}
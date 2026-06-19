#pragma once
#include "QuadArch/Core.h"

namespace QuadArch {
	class QuadAPI Scene
	{
	public:
		~Scene() = default;

		virtual void Init() = 0;
		virtual void OnUpdate(float ts) = 0;
		virtual void Destroy() = 0;
	};

	class QuadAPI EmptyScene : public Scene
	{
	public:
		void Init() override {}
		void OnUpdate(float ts) override {}
		void Destroy() override {}
	};
}
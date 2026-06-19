#pragma once
#include "QuadArch/Core.h"

#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

#include <memory>

namespace QuadArch {
	class QuadAPI ExampleScene : public Scene
	{
	public:
		void Init() override {}
		void OnUpdate(float ts) override 
		{
			glClearColor(0.0f, 0.3f, 0.8f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		void Destroy() override {}
	};

	class QuadAPI SplashScreen : public Scene
	{
	public:
		void Init() override {}
		void OnUpdate(float ts) override 
		{
			glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_ElapsedTime += ts;
			std::unique_ptr<ExampleScene> next = std::make_unique<ExampleScene>();
			if (m_ElapsedTime >= 3.0f)
			{
				SceneManager::GetInstance().SetScene(std::move(next));
				SceneManager::GetInstance().ChangeScene();
			}
		}
		void Destroy() override {}

	private:
		float m_ElapsedTime = 0.0f;
	};
}
#pragma once
#include "QuadArch.h"

#include "GLFW/glfw3.h"
#include <memory>

class ExampleScene : public QuadArch::Scene
{
public:
	void Init() override {}
	void OnUpdate(float ts) override
	{
		QuadArch::RenderCommand::SetClearColor(0.0f, 0.3f, 0.8f, 1.0f);
		QuadArch::RenderCommand::Clear();
	}
	void Destroy() override {}
};

class SplashScreen : public QuadArch::Scene
{
public:
	void Init() override {}
	void OnUpdate(float ts) override
	{
		QuadArch::RenderCommand::SetClearColor(0.0f, 0.0f, 0.2f, 1.0f);
		QuadArch::RenderCommand::Clear();

		m_ElapsedTime += ts;
		std::unique_ptr<ExampleScene> next = std::make_unique<ExampleScene>();
		if (m_ElapsedTime >= 3.0f)
		{
			QuadArch::SceneManager::GetInstance().SetScene(std::move(next));
			QuadArch::SceneManager::GetInstance().ChangeScene();
		}
	}
	void Destroy() override {}

private:
	float m_ElapsedTime = 0.0f;
};

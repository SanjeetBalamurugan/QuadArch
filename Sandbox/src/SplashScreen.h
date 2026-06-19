#pragma once
#include "QuadArch.h"

#include <memory>
#include <iostream>

class ExampleScene : public QuadArch::Scene
{
public:
	void Init() override {}
	void OnUpdate(float ts) override
	{
		QuadArch::RenderCommand::SetClearColor(r, 0.3f, 0.8f, 1.0f);
		QuadArch::RenderCommand::Clear();

		float mouseX = QuadArch::Mouse::GetMouseX();
		float mouseY = QuadArch::Mouse::GetMouseY();

		if (QuadArch::Mouse::IsButtonDown(QuadArch::MouseButton::Left))
		{
			std::cout << "Left Click Intercepted at Screen Coordinates: X=" << mouseX << ", Y=" << mouseY << "\n";
		}

		if (QuadArch::Mouse::IsButtonHeld(QuadArch::MouseButton::Right))
		{
			std::cout << "Right Hold Intercepted at Screen Coordinates: X=" << mouseX << ", Y=" << mouseY << "\n";
		}

		if (QuadArch::Input::IsKeyDown(QuadArch::Key::Enter))
		{
			if (r >= 1.0f)
			{
				r -= 0.5f;
			}
			else {
				r += 0.5f;
			}
		}
	}
	void Destroy() override {}

private:
	float r = 0.0f;
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

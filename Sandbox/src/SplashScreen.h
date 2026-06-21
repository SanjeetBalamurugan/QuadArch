#pragma once
#include "QuadArch.h"

#include <memory>
#include <iostream>
#include <string>
#include <filesystem> 
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Cube
{
    glm::vec3 pos;
    glm::vec3 scale;
    glm::vec3 rot;
};

class ExampleScene : public QuadArch::Scene
{
public:
    void Init() override
    {
        m_cubes.reserve(500);
        m_Camera = std::make_shared<QuadArch::Camera3D>();
        QuadArch::Renderer::SetActiveCamera(m_Camera);

        std::cout << "Engine Active Working Directory: " << std::filesystem::current_path() << std::endl;
        m_defaultTex = std::make_shared<QuadArch::Texture>("assets/default_texture.png");

        auto mat = std::make_shared<QuadArch::LitMaterial>(0, m_defaultTex, glm::vec4(0.0f, 0.0f, 0.5f, 1.0f));
        m_lit = mat;

        QuadArch::RenderCommand::Blend(false);
    }

    void OnUpdate(float ts) override
    {
        QuadArch::RenderCommand::SetClearColor(r, 0.0f, 0.8f, 1.0f);
        QuadArch::RenderCommand::Clear();

        float mouseX = QuadArch::Mouse::GetMouseX();
        float mouseY = QuadArch::Mouse::GetMouseY();
        QuadArch::Renderer::GetActiveCamera()->OnUpdate(ts, mouseX, mouseY);

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
            if (r >= 1.0f) r -= 0.5f;
            else           r += 0.5f;
        }

        if (m_lit)
        {
            // Positioned at Z = -5.0f (in front of camera), size = 1.0f world meter
            QuadArch::Renderer::DrawCube({ 0.0f, 0.0f, -5.0f }, { 1, 1, 1 }, { 0.0f, 0.0f, 0.0f }, m_lit);
        }

        if (QuadArch::Input::IsKeyDown(QuadArch::Key::C))
        {
            Cube c;
            c.pos = { QuadArch::Random::RandDOUBLE(0, 100.0f), QuadArch::Random::RandDOUBLE(0, 10.0f), QuadArch::Random::RandDOUBLE(-20.0f, 20.0f) };
            c.scale = { QuadArch::Random::RandDOUBLE(0, 3.0f), QuadArch::Random::RandDOUBLE(0, 2.0f), QuadArch::Random::RandDOUBLE(0, 5.0f) };
            c.rot = { QuadArch::Random::RandDOUBLE(0, 3), QuadArch::Random::RandDOUBLE(0, 3), QuadArch::Random::RandDOUBLE(-3, 3) };

            m_cubes.push_back(c);
        }

        for (Cube c : m_cubes)
        {
            QuadArch::Renderer::DrawCube(c.pos, c.scale, c.rot, m_lit);
        }

        std::cout << QuadArch::Random::RandINT(0, 100) << std::endl;
    }

    void Destroy() override {}

private:
    float r = 0.0f;
    std::shared_ptr<QuadArch::LitMaterial> m_lit;
    std::shared_ptr<QuadArch::Texture> m_defaultTex;
    std::shared_ptr<QuadArch::Camera3D> m_Camera;

    std::vector<Cube> m_cubes = {};
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

        if (m_ElapsedTime >= 3.0f)
        {
            std::unique_ptr<ExampleScene> next = std::make_unique<ExampleScene>();
            QuadArch::SceneManager::GetInstance().SetScene(std::move(next));
            QuadArch::SceneManager::GetInstance().ChangeScene();
        }
    }
    void Destroy() override {}

private:
    float m_ElapsedTime = 0.0f;
};

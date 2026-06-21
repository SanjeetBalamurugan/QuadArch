#pragma once
#include "QuadArch.h"
#include <memory>
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <sstream>
#include <iomanip>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TEST_COUNT 500

struct Cube {
    glm::vec3 pos;
    glm::vec3 scale;
    glm::vec3 rot;
};

class ExampleScene : public QuadArch::Scene {
public:
    void Init() override {
        m_SkyboxTex = QuadArch::Texture::LoadCrossCubemap("assets/skybox/skybox01.png");
        m_SkyboxMat = std::make_shared<QuadArch::SkyBoxMaterial>(1, m_SkyboxTex);

        m_cubes.reserve(500000);
        for (size_t i = 0; i < TEST_COUNT; i++) {
            Cube c;
            c.pos = { QuadArch::Random::RandDOUBLE(-1000, 1000.0f),
                      QuadArch::Random::RandDOUBLE(-500, 1000.0f),
                      QuadArch::Random::RandDOUBLE(-200.0f, 200.0f) };
            c.scale = { QuadArch::Random::RandDOUBLE(0, 3.0f),
                        QuadArch::Random::RandDOUBLE(0, 2.0f),
                        QuadArch::Random::RandDOUBLE(0, 5.0f) };
            c.rot = { QuadArch::Random::RandDOUBLE(0, 3),
                      QuadArch::Random::RandDOUBLE(0, 3),
                      QuadArch::Random::RandDOUBLE(-3, 3) };
            m_cubes.push_back(c);
        }

        m_Camera = std::make_shared<QuadArch::Camera3D>();
        QuadArch::Renderer::SetActiveCamera(m_Camera);
        std::cout << "Engine Active Working Directory: " << std::filesystem::current_path() << std::endl;
        m_defaultTex = std::make_shared<QuadArch::Texture>("assets/default_texture.png");
        m_lit = std::make_shared<QuadArch::LitMaterial>(0, m_defaultTex, glm::vec4(0.0f, 0.0f, 0.5f, 1.0f));

        QuadArch::RenderCommand::Blend(false);
        QuadArch::RenderCommand::SetCursorMode(m_Focused);
    }

    void OnUpdate(float ts) override {
        QuadArch::RenderCommand::Clear();
        auto* camera = QuadArch::Renderer::GetActiveCamera();
        if (camera)
            m_SkyboxMat->DrawSkybox(*camera);

        m_ElapsedTime += ts;
        m_FrameCount++;

        if (QuadArch::Input::IsKeyDown(QuadArch::Key::Escape))
        {
            m_Focused = !m_Focused;
            QuadArch::RenderCommand::SetCursorMode(m_Focused);
        }

        if (m_ElapsedTime >= 0.5f) {
            float fps = (float)m_FrameCount / m_ElapsedTime;
            float frameTimeMS = (m_ElapsedTime / (float)m_FrameCount) * 1000.0f;

            std::stringstream ss;
            ss << "Sandbox | FPS: " << fps << " | Frame Time: " << std::fixed << std::setprecision(2) << frameTimeMS << " ms";
            QuadArch::RenderCommand::SetWindowTitle(ss.str());

            m_FrameCount = 0;
            m_ElapsedTime = 0.0f;
        }

        float mouseX = QuadArch::Mouse::GetMouseX();
        float mouseY = QuadArch::Mouse::GetMouseY();
        QuadArch::Renderer::GetActiveCamera()->OnUpdate(ts, mouseX, mouseY);

        if (QuadArch::Mouse::IsButtonDown(QuadArch::MouseButton::Left)) {
            std::cout << "Left Click Intercepted at Screen Coordinates: X=" << mouseX << ", Y=" << mouseY << "\n";
        }
        if (QuadArch::Mouse::IsButtonHeld(QuadArch::MouseButton::Right)) {
            std::cout << "Right Hold Intercepted at Screen Coordinates: X=" << mouseX << ", Y=" << mouseY << "\n";
        }

        if (QuadArch::Input::IsKeyDown(QuadArch::Key::Enter)) {
            r = (r >= 1.0f) ? r - 0.5f : r + 0.5f;
        }

        if (m_lit) {
            QuadArch::Renderer::DrawCube({ 0.0f, 0.0f, -5.0f }, { 1, 1, 1 }, { 0.0f, 0.0f, 0.0f }, m_lit);
        }
        for (const auto& c : m_cubes) {
            QuadArch::Renderer::DrawCube(c.pos, c.scale, c.rot, m_lit);
        }
    }

    void Destroy() override {}

private:
    float r = 0.0f;
    std::shared_ptr<QuadArch::LitMaterial> m_lit;
    std::shared_ptr<QuadArch::Texture> m_defaultTex;
    std::shared_ptr<QuadArch::Camera3D> m_Camera;
    std::vector<Cube> m_cubes = {};

    std::shared_ptr<QuadArch::Texture> m_SkyboxTex;
    std::shared_ptr<QuadArch::SkyBoxMaterial> m_SkyboxMat;

    float m_ElapsedTime = 0.0f;
    unsigned int m_FrameCount = 0;
    bool m_Focused = true;
};

class SplashScreen : public QuadArch::Scene {
public:
    void Init() override {}

    void OnUpdate(float ts) override {
        QuadArch::RenderCommand::SetClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        QuadArch::RenderCommand::Clear();

        m_ElapsedTime += ts;
        if (m_ElapsedTime >= 3.0f) {
            auto next = std::make_unique<ExampleScene>();
            QuadArch::SceneManager::GetInstance().SetScene(std::move(next));
            QuadArch::SceneManager::GetInstance().ChangeScene();
        }
    }

    void Destroy() override {}

private:
    float m_ElapsedTime = 0.0f;
};

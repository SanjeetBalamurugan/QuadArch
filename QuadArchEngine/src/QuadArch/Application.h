#pragma once
#include "Core.h"
#include "Window.h"
#include "Game.h"

#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

#include "Scene/ExampleScenes/ExampleScene.h"

#include "Inputs/Input.h"
#include "Inputs/KeyCodes.h"

#include "Inputs/Mouse.h"

#include <memory>

namespace QuadArch {
	class QuadAPI Application
	{
	public:
		Application(std::unique_ptr<Game> game)
			: m_Game(std::move(game)) {};
		~Application() = default;

		virtual void Init();
		virtual void Update();
		virtual void End();

		Window* GetWindow() const { return m_Window; }
	private:
		Window* m_Window = nullptr;
		std::unique_ptr<Game> m_Game = nullptr;

		std::unique_ptr<EventBuffer> m_EventBuffer;
	};
}
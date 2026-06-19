#pragma once
#include "QuadArch/Core.h"

#include "Scene.h"

namespace QuadArch
{
	class SceneManager
	{
	public:
		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;
		SceneManager(SceneManager&&) = delete;
		SceneManager& operator=(SceneManager&&) = delete;

		static SceneManager& GetInstance()
		{
			static SceneManager instance;
			return instance;
		}

		void Init() const;
		void Update(float ts) const;
		void Destroy() const;
		void SetScene(std::unique_ptr<Scene> scene);
		void ChangeScene();

	private:
		SceneManager() {};
		~SceneManager() = default;

		std::unique_ptr<Scene> m_CurrentScene = std::make_unique<QuadArch::EmptyScene>();
		std::unique_ptr<Scene> m_NextScene = nullptr;
	};
}


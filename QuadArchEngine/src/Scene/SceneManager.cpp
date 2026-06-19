#include "pch.h"
#include "SceneManager.h"

void QuadArch::SceneManager::Init() const
{
	m_CurrentScene->Init();
}

void QuadArch::SceneManager::Update(float ts) const
{
	m_CurrentScene->OnUpdate(ts);
}

void QuadArch::SceneManager::Destroy() const
{
	m_CurrentScene->Destroy();
	if (m_NextScene != nullptr)
		m_NextScene->Destroy();
}

void QuadArch::SceneManager::SetScene(std::unique_ptr<Scene> scene)
{
	m_NextScene = std::move(scene);
}

void QuadArch::SceneManager::ChangeScene()
{
	m_CurrentScene->Destroy();
	m_CurrentScene = std::move(m_NextScene);
	m_CurrentScene->Init();
}

#include "QuadArch.h"

#include "SplashScreen.h"

class SandboxGame : public QuadArch::Game
{
public:
	void Init() override 
	{
		// Splash Screen
		std::unique_ptr<SplashScreen> splash = std::make_unique<SplashScreen>();
		QuadArch::SceneManager::GetInstance().SetScene(std::move(splash));
		QuadArch::SceneManager::GetInstance().ChangeScene();
	};
	void Update(float ts) override {};
	void Destroy() override {};
private:
	
};

int main() {
	QuadArch::Application* SandboxApp = new QuadArch::Application(std::make_unique<SandboxGame>());
	SandboxApp->Init();
	SandboxApp->Update();
	SandboxApp->End();
	delete SandboxApp;
}
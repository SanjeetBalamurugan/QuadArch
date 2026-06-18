#include "QuadArch.h"

int main() {
	QuadArch::Application* SandboxApp = new QuadArch::Application();
	SandboxApp->Init();
	SandboxApp->Update();
	SandboxApp->End();
	delete SandboxApp;
}
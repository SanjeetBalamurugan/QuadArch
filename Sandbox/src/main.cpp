#include "QuadArch.h"

int main() {
	QuadArch::Application* SandboxApp = new QuadArch::Application();
	SandboxApp->Init();
	delete SandboxApp;
}
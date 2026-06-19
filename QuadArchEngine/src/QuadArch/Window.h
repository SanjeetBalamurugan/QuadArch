#pragma once
#include "Core.h"

#include <string>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace QuadArch {
	struct WindowSpecs {
		unsigned int width = 500;
		unsigned int height = 500;

		std::string title = "TestApp";
		GLFWwindow* window = nullptr;
	};

	class QuadAPI Window
	{
	public:
		void Init();
		void Update();
		void Destroy();

		const WindowSpecs& GetSpecs() const { return m_Specs; }

	private:
		WindowSpecs m_Specs;
	};

}

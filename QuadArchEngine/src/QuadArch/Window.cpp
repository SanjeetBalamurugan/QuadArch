#include "pch.h"
#include "Window.h"

void QuadArch::Window::Init()
{
	m_Specs.window = glfwCreateWindow(
		m_Specs.width,
		m_Specs.height,
		m_Specs.title.c_str(),
		NULL,
		NULL
	);

	if (m_Specs.window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}
}

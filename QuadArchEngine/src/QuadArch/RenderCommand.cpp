#include "pch.h"
#include "RenderCommand.h"

#include "Window.h"

inline void QuadArch::RenderCommand::SetClearColor(float r, float g, float b, float a)
{
	glClearColor(r,g,b,a);
}

inline void QuadArch::RenderCommand::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

inline void QuadArch::RenderCommand::Blend(bool enable)
{
	if (enable)
		glEnable(GL_BLEND);
	else
	{
		glDisable(GL_BLEND);
	}
}

inline void QuadArch::RenderCommand::SetWindowTitle(const std::string& title)
{
	GLFWwindow* window = glfwGetCurrentContext();
	glfwSetWindowTitle(window, title.c_str());
}

inline void QuadArch::RenderCommand::SetCursorMode(bool hiddenAndLocked)
{
    GLFWwindow* window = glfwGetCurrentContext();

    if (hiddenAndLocked)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

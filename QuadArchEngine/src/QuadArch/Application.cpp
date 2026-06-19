#include "pch.h"
#include "Application.h"

static void GLFWErrorCallback(int error_code, const char* description)
{
	std::cerr << "[GLFW Error] (" << error_code << "): " << description << std::endl;
}

static void GLFWFrameSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void QuadArch::Application::Init()
{
	glfwSetErrorCallback(GLFWErrorCallback);

	if (!glfwInit())
	{
		std::cout << "OpenGl Init Failed!!" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	std::cout << "GLFW Compile Version: "
		<< GLFW_VERSION_MAJOR << "."
		<< GLFW_VERSION_MINOR << "."
		<< GLFW_VERSION_REVISION << std::endl;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = new Window();
	m_Window->Init();
	glfwMakeContextCurrent(m_Window->GetSpecs().window);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	glfwSetFramebufferSizeCallback(m_Window->GetSpecs().window, GLFWFrameSizeCallback);
	glViewport(0, 0, m_Window->GetSpecs().width, m_Window->GetSpecs().height);

	void* proc = (void*)glfwGetProcAddress("glGetString");
	std::cout << "Direct driver test (glGetString address): " << proc << std::endl;
}

void QuadArch::Application::Update()
{
	std::unique_ptr<SplashScreen> splash = std::make_unique<SplashScreen>();
	SceneManager::GetInstance().SetScene(std::move(splash));
	SceneManager::GetInstance().ChangeScene();
	SceneManager::GetInstance().Init();

	auto lastTime = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(m_Window->GetSpecs().window))
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> elapsedTime = currentTime - lastTime;
		lastTime = currentTime;

		float ts = elapsedTime.count();
		SceneManager::GetInstance().Update(ts);

		glfwSwapBuffers(m_Window->GetSpecs().window);
		glfwPollEvents();
	}

	SceneManager::GetInstance().Destroy();
}

void QuadArch::Application::End()
{
	glfwTerminate();
	delete m_Window;
}

#include "atlpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Atlas {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		ATL_CORE_ASSERT(windowHandle, "window Handle is null");
	}

	void OpenGLContext::Init()
	{
		ATL_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
		ATL_CORE_ASSERT(status, "Failed to initializ Glad!");

		ATL_CORE_INFO("OpenGL Info:");
		ATL_CORE_INFO("	Vendor:	  {0}", glGetString(GL_VENDOR));
		ATL_CORE_INFO("	Renderer: {0}", glGetString(GL_RENDERER));
		ATL_CORE_INFO("	Version:  {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		ATL_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}
#include "Window.h"
#include "RHI/RHI.h"

Window::Window(int width, int height)
	:mWidth(width),mHeight(height)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	mWindow = glfwCreateWindow(mWidth, mHeight, "Gear", nullptr, nullptr);
	glfwSetWindowUserPointer(mWindow, this);
	glfwSetFramebufferSizeCallback(mWindow, resizeCallback);
	glfwSetKeyCallback(mWindow, keyCallback);
	glfwSetMouseButtonCallback(mWindow, mouseButtonCallback);
	glfwSetCursorPosCallback(mWindow, cursorPosCallback);
	glfwSetScrollCallback(mWindow, mouseScrollCallback);

	//��ʼ��������

	auto res = glfwCreateWindowSurface(RHI::instance().getInstance(), mWindow, nullptr, &mSurface);
	if (res != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}

	//ȱ�ټ������
	VkBool32 supportsPresent;
	uint32_t graphicsFamily = RHI::instance().getDevice()->getGraphicsFamily();
	vkGetPhysicalDeviceSurfaceSupportKHR(RHI::instance().getDevice()->getPhyDevice(), graphicsFamily, mSurface, &supportsPresent);

	if (!supportsPresent)
	{
		throw std::runtime_error("Cannot find a graphics queue that also supports present operations.");
	}

	mSwapChain = new RHISwapChain(RHI::instance().getDevice(), mSurface, width, height);
}

Window::~Window()
{
	glfwDestroyWindow(mWindow);
	if (mSwapChain)
		delete mSwapChain;
}

GLFWwindow * Window::getWindowPtr()
{
	return mWindow;
}

std::vector<const char*> Window::getRequiredExtensions(bool enableValidationLayers)
{
	std::vector<const char*> extensions;

	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (unsigned int i = 0; i < glfwExtensionCount; i++)
	{
		extensions.push_back(glfwExtensions[i]);
	}

	if (enableValidationLayers)
	{
		//extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

int Window::getWidth()
{
	return mWidth;
}

int Window::getHeight()
{
	return mHeight;
}

void Window::reset(uint32_t width, uint32_t height)
{
	mWidth = width;
	mHeight = height;
	if (mSwapChain)
		delete mSwapChain;
	mSwapChain = new RHISwapChain(RHI::instance().getDevice(), mSurface, width, height);
}

void resizeCallback(GLFWwindow * window, int width, int height)
{
	Window * win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	win->reset(width, height);
	
}

void cursorPosCallback(GLFWwindow * window, double xPos, double yPos)
{
	Input::instance().setMousePosition(glm::vec2(xPos, yPos));
}

void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	int curKey = 0;
	if (key >= 48 && key <= 57) 
	{
		curKey = key - 48;
	}
	else if (key >= 65 && key <= 90) 
	{
		curKey = key - 55;
	}
	else 
	{
		return;
	}

	switch (action) 
	{
	case GLFW_PRESS:
		Input::instance().setKeyDown(curKey);
		Input::instance().setKey(curKey, true);
		break;
	case GLFW_RELEASE:
		Input::instance().setKeyUp(curKey);
		Input::instance().setKey(curKey, false);
		break;
	default:
		break;
	}
}

void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	switch (action) 
	{
	case GLFW_PRESS:
		Input::instance().setMouseButtonDown(button);
		Input::instance().setMouseButton(button, true);
		break;
	case GLFW_RELEASE:
		Input::instance().setMouseButtonUp(button);
		Input::instance().setMouseButton(button, false);
		break;
	default:
		break;
	}
}

void mouseScrollCallback(GLFWwindow * window, double xOffset, double yOffset)
{
	Input::instance().setMouseScrollWheel(yOffset);
}



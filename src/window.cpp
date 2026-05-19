
#include <stdexcept>

#include "instance.h"
#include "window.h"
#include "frameDraw.h"

void Window::initWindow() {

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
   // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    glWindow = glfwCreateWindow(winWidth, winHeight, "Triangle Rasterizer", nullptr, nullptr);
    glfwSetWindowUserPointer(glWindow, this);
    glfwSetFramebufferSizeCallback(glWindow, framebufferResizeCallback);
}

void Window::framebufferResizeCallback(GLFWwindow* glWindow, int width, int height) {
    auto app = reinterpret_cast<FrameDraw*>(glfwGetWindowUserPointer(glWindow));
    app->framebufferResized = true;
}

void Window::createSurface() {
    if (glfwCreateWindowSurface(vulkanInstance, glWindow, nullptr, &vulkanSurface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to Create Window Surface");
    }
}

void Window::pauseWindowConditional() {
    int width = 0, height = 0;
    glfwGetFramebufferSize(glWindow, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(glWindow, &width, &height);
        glfwWaitEvents();
    }
}

Window::Window(VulkanContext& ctx) : context(ctx) {

    vulkanInstance = ctx.instance->vulkanInstance;

    initWindow();
    createSurface();
}

Window::~Window() {
    vkDestroySurfaceKHR(vulkanInstance, vulkanSurface, nullptr);
    glfwDestroyWindow(glWindow);
}
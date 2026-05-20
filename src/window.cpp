
#include <stdexcept>

#include "instance.h"
#include "window.h"
#include "frameDraw.h"

void Window::initWindow() {

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
   // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    glWindow = glfwCreateWindow(winWidth, winHeight, "Triangle Rasterizer", nullptr, nullptr);
    glfwSetFramebufferSizeCallback(glWindow, framebufferResizeCallback);
}

void Window::createSurface() {
    if (glfwCreateWindowSurface(VINSTANCE, GLWINDOW, nullptr, &vulkanSurface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to Create Window Surface");
    }
}

void Window::framebufferResizeCallback(GLFWwindow* glWindow, int width, int height) {
    auto frame = reinterpret_cast<FrameDraw*>(glfwGetWindowUserPointer(glWindow));
    frame->framebufferResized = true;
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
    ctx.window = this;

    initWindow();
    createSurface();
}

Window::~Window() {
    
    vkDestroySurfaceKHR(VINSTANCE, vulkanSurface, nullptr);
    glfwDestroyWindow(glWindow);
}
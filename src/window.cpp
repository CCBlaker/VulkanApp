#pragma once

#include <stdexcept>

#include "instance.h"
#include "window.h"

void Window::initWindow() {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    glWindow = glfwCreateWindow(WINWIDTH, WINHEIGHT, "Triangle Rasterizer", nullptr, nullptr);
}

void Window::createSurface() {
    if (glfwCreateWindowSurface(vulkanInstance, glWindow, nullptr, &vulkanSurface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to Create Window Surface");
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
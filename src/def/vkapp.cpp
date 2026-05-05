#pragma once

#include "vkapp.h"
#include <memory>

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

void VulkanApp::init() {
    context = std::make_unique<VulkanContext>(enableValidationLayers, validationLayers);

    glfwInit();

    // Create objects in order of dependencies
    instance = std::make_unique<Instance>(*context);
    context->instance = instance.get();

    debugMessenger = std::make_unique<DebugMessenger>(*context);
    context->debugMessenger = debugMessenger.get();

    window = std::make_unique<Window>(*context);
    context->window = window.get();

    physicalDevice = std::make_unique<PhysicalDevice>(*context);
    context->physicalDevice = physicalDevice.get();

    device = std::make_unique<Device>(*context);
    context->device = device.get();

}

void VulkanApp::mainLoop() {
    while (!glfwWindowShouldClose(window->glWindow)) {
        glfwPollEvents();
    }
}

void VulkanApp::cleanup() {
    // unique_ptrs will clean up in reverse order
    glfwTerminate();
}
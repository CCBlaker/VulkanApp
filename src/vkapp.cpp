
#include "vkapp.h"
#include <memory>
#include <cstring>

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

void VulkanApp::init() {
    context = std::make_unique<VulkanContext>(enableValidationLayers, validationLayers, deviceExtensions);

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

    swapChain = std::make_unique<SwapChain>(*context);
    context->swapChain = swapChain.get();

    renderpass = std::make_unique<Renderpass>(*context);
    context->renderpass = renderpass.get();

    pipeline = std::make_unique<Pipeline>(*context);
    context->pipeline = pipeline.get();

    framebuffer = std::make_unique<Framebuffer>(*context);
    context->framebuffer = framebuffer.get();

    commandbuffer = std::make_unique<Commandbuffer>(*context);
    context->commandbuffer = commandbuffer.get();

    frameDraw = std::make_unique<FrameDraw>(*context);
    context->frameDraw = frameDraw.get();
}

void VulkanApp::mainLoop() {
    while (!glfwWindowShouldClose(window->glWindow)) {
        glfwPollEvents();
        frameDraw.get()->drawFrame();
    }
        vkDeviceWaitIdle(device.get()->vulkanDevice);
}

void VulkanApp::cleanup() {
    // unique_ptrs will clean up in reverse order
    glfwTerminate();
}
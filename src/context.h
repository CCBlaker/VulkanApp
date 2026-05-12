#pragma once

#define WINDOW context.window
#define SWAPCHAIN context.swapChain
#define PDEVICE context.physicalDevice
#define PINDICES context.physicalDevice->vulkanPhysicalDeviceQueueFamilies

#define VINSTANCE context.instance->vulkanInstance
#define VDEBUG context.debugMessenger->vulkanDebugMessenger
#define GLWINDOW context.window->glWindow
#define VSURFACE context.window->vulkanSurface
#define VDEVICE context.device->vulkanDevice
#define VPIPELINE context.pipeline->vulkanPipeline

#define ISDEBUG context.enableValidationLayers
#define LAYERS context.validationLayers
#define EXTENSIONS context.deviceExtensions

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

// Forward declarations
class Instance;
class DebugMessenger;
class Window;
class PhysicalDevice;
class Device;
class SwapChain;
class Pipeline;

struct VulkanContext {
    bool enableValidationLayers;
    std::vector<const char*> validationLayers;
    std::vector<const char*> deviceExtensions;

    Instance* instance = nullptr;
    DebugMessenger* debugMessenger = nullptr;
    Window* window = nullptr;
    PhysicalDevice* physicalDevice = nullptr;
    Device* device = nullptr;
    SwapChain* swapChain = nullptr;
    Pipeline* pipeline = nullptr;

    VulkanContext(bool enable, const std::vector<const char*>& layers, const std::vector<const char*>& extensions)
        : enableValidationLayers(enable), validationLayers(layers), deviceExtensions(extensions){}
};
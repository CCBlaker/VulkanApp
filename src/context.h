#pragma once

#define WINDOW context.window
#define SWAPCHAIN context.swapChain
#define PDEVICE context.physicalDevice
#define PINDICES context.physicalDevice->vulkanPhysicalDeviceQueueFamilies
#define DEVICE context.device
#define PIPELINE context.pipeline
#define COMMANDBUFFER context.commandbuffer

#define VINSTANCE context.instance->vulkanInstance
#define VDEBUG context.debugMessenger->vulkanDebugMessenger
#define GLWINDOW context.window->glWindow
#define VSURFACE context.window->vulkanSurface
#define VDEVICE context.device->vulkanDevice
#define VPIPELINE context.pipeline->vulkanGraphicsPipeline
#define VRENDERPASS context.renderpass->vulkanRenderpass
#define VSWAPCHAIN context.swapChain->vulkanSwapChain
#define VFRAMEBUFFERS context.framebuffer->vulkanFramebuffers
#define VCOMMANDPOOL context.commandbuffer->vulkanCommandPool
#define VCOMMANDBUFFER context.commandbuffer->vulkanCommandbuffer

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
class Renderpass;
class Pipeline;
class Framebuffer;
class Commandbuffer;
class FrameDraw;

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
    Renderpass* renderpass = nullptr;
    Pipeline* pipeline = nullptr;
    Framebuffer* framebuffer = nullptr;
    Commandbuffer* commandbuffer = nullptr;
    FrameDraw* frameDraw = nullptr;


    VulkanContext(bool enable, const std::vector<const char*>& layers, const std::vector<const char*>& extensions)
        : enableValidationLayers(enable), validationLayers(layers), deviceExtensions(extensions){}
};
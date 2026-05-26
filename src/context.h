#pragma once

#define WINDOW context.window
#define SWAPCHAIN context.swapChain
#define PDEVICE context.physicalDevice
#define PINDICES context.physicalDevice->vulkanQueueFamilies
#define DEVICE context.device
#define RENDERPASS context.renderpass
#define PIPELINE context.pipeline
#define FRAMEBUFFER context.framebuffer
#define COMMANDBUFFER context.commandbuffer
#define FRAMEDRAW context.frameDraw

#define VINSTANCE context.instance->vulkanInstance
#define VDEBUG context.debugMessenger->vulkanDebugMessenger
#define GLWINDOW context.window->glWindow
#define VSURFACE context.window->vulkanSurface
#define VDEVICE context.device->vulkanDevice
#define VPIPELINE context.pipeline->vulkanGraphicsPipeline
#define VVIEWPORT context.pipeline->viewport
#define VSCISSOR context.pipeline->scissor
#define VRENDERPASS context.renderpass->vulkanRenderpass
#define VSWAPCHAIN context.swapChain->vulkanSwapChain
#define VFRAMEBUFFERS context.swapChain->vulkanFramebuffers
#define VCOMMANDPOOL context.commandbuffer->vulkanCommandPool
#define VCOMMANDBUFFERS context.commandbuffer->vulkanCommandbuffers

#define ISDEBUG context.enableValidationLayers
#define LAYERS context.validationLayers
#define EXTENSIONS context.deviceExtensions
#define MAXFRAMESINFLIGHT context.maxFramesInFlight 

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

// Forward declarations
class Instance;
class DebugMessenger;
class Window;
class PhysicalDevice;
class Device;
class Renderpass;
class SwapChain;
class Pipeline;
class Commandbuffer;
class FrameDraw;

struct VulkanContext {
    bool enableValidationLayers;
    std::vector<const char*> validationLayers;
    std::vector<const char*> deviceExtensions;
    const int maxFramesInFlight;

    Instance* instance = nullptr;
    DebugMessenger* debugMessenger = nullptr;
    Window* window = nullptr;
    PhysicalDevice* physicalDevice = nullptr;
    Device* device = nullptr;
    Renderpass* renderpass = nullptr;
    SwapChain* swapChain = nullptr;
    Pipeline* pipeline = nullptr;
    Commandbuffer* commandbuffer = nullptr;
    FrameDraw* frameDraw = nullptr;


    VulkanContext(bool enable, const std::vector<const char*>& layers, const std::vector<const char*>& extensions, const int& flightFrames)
        : enableValidationLayers(enable), validationLayers(layers), deviceExtensions(extensions), maxFramesInFlight(flightFrames) {}
};
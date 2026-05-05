#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

// Forward declarations
class Instance;
class DebugMessenger;
class Window;
class PhysicalDevice;
class Device;

struct VulkanContext {
    bool enableValidationLayers;
    std::vector<const char*> validationLayers;

    Instance* instance = nullptr;
    DebugMessenger* debugMessenger = nullptr;
    Window* window = nullptr;
    PhysicalDevice* physicalDevice = nullptr;
    Device* device = nullptr;

    VulkanContext(bool enable, const std::vector<const char*>& layers)
        : enableValidationLayers(enable), validationLayers(layers) {}
};
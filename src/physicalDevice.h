#pragma once

#include "context.h"

#include <optional>

struct QueueFamilyIndicies {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

class PhysicalDevice {

    public:

        VkPhysicalDevice vulkanPhysicalDevice = VK_NULL_HANDLE;
        QueueFamilyIndicies vulkanPhysicalDeviceQueueFamilies;

        PhysicalDevice(VulkanContext& context);

        ~PhysicalDevice();

    private:

        VulkanContext& context;
        VkInstance vulkanInstance;
        VkSurfaceKHR vulkanSurface;

        QueueFamilyIndicies findQueueFamilies(VkPhysicalDevice device);
        bool isDeviceSuitable(VkPhysicalDevice device);
        void pickPhysicalDevice();

};
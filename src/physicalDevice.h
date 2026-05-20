#pragma once

#include "context.h"

#include <set>
#include <optional>

struct QueueFamilyIndicies {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    
     std::set<uint32_t> uniqueFamilies() {
        return {graphicsFamily.value(), presentFamily.value()};
    }

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapSurfaceSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};


class PhysicalDevice {

    public:

        VkPhysicalDevice vulkanPhysicalDevice = VK_NULL_HANDLE;
        QueueFamilyIndicies vulkanQueueFamilies;
        SwapSurfaceSupportDetails supportDetails;
        
        PhysicalDevice(VulkanContext& context);

        ~PhysicalDevice();

    private:

        VulkanContext& context;

        QueueFamilyIndicies findQueueFamilies(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        void querySwapSurfaceSupport(VkPhysicalDevice device);

        bool isDeviceSuitable(VkPhysicalDevice device);
        void pickPhysicalDevice();

};
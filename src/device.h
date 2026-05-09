#pragma once

#include "context.h"

#include <vector>
#include <set>

class Device {
    public:

        VkDevice vulkanDevice;
        VkQueue graphicsQueue;
        VkQueue presentQueue;

        Device(VulkanContext& context);
        
        ~Device();

    private:

        VulkanContext& context;
        
        void createLogicalDevice(PhysicalDevice& physicalDevice);
};
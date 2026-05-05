#pragma once

#include <vector>
#include <stdexcept>
#include <optional>

#include "physicalDevice.h"
#include "instance.h"
#include "window.h"

//Find Graphics Queue Family
QueueFamilyIndicies PhysicalDevice::findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndicies indices;
    uint32_t queueFamilyCount = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {

        //Check Graphics Support
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }
        
        //Check Presentation Support
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, vulkanSurface, &presentSupport);
        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }
    return indices;
}

//Check if Physical Device is Suitable for Vulkan
bool PhysicalDevice::isDeviceSuitable(VkPhysicalDevice device) {
    QueueFamilyIndicies indices = findQueueFamilies(device);

    return indices.isComplete();
}

//Picks Physical Device for Vulkan
void PhysicalDevice::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, nullptr);
    
    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find Vulkan Supported GPU");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        QueueFamilyIndicies indices = findQueueFamilies(device);

        if (indices.isComplete()) {
            vulkanPhysicalDevice = device;
            vulkanPhysicalDeviceQueueFamilies = indices;
            break;
        }
    }
    if (vulkanPhysicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("Failed to find Suitable GPU");
    }
    
}

PhysicalDevice::PhysicalDevice(VulkanContext& ctx) : context(ctx)
{
    if (ctx.instance == nullptr) {
        throw std::runtime_error("PhysicalDevice Created Before Instance");
    }
    vulkanInstance = ctx.instance->vulkanInstance;
    vulkanSurface = ctx.window->vulkanSurface;

    ctx.physicalDevice = this;
    pickPhysicalDevice();
}

PhysicalDevice::~PhysicalDevice() {
}
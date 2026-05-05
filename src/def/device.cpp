#pragma once

#include <stdexcept>
#include <set>

#include "device.h"
#include "physicalDevice.h"

//Creates the Logical Device that interfaces with the GPU
void Device::createLogicalDevice(PhysicalDevice& physicalDevice) {
    QueueFamilyIndicies indices = physicalDevice.vulkanPhysicalDeviceQueueFamilies;

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};
    
    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    //Settings for the Device Features
    VkPhysicalDeviceFeatures deviceFeatures{};

    //Settings for the Device itself
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physicalDevice.vulkanPhysicalDevice, &createInfo, nullptr, &vulkanDevice) != VK_SUCCESS) {
        throw std::runtime_error("Failed to Create Logical Device");
    }

    vkGetDeviceQueue(vulkanDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(vulkanDevice, indices.presentFamily.value(), 0, &presentQueue);
}

Device::Device(VulkanContext& ctx) : context(ctx), enableValidationLayers(ctx.enableValidationLayers), validationLayers(ctx.validationLayers)
{
    if (ctx.physicalDevice == nullptr) {
        throw std::runtime_error("Device Created before PhysicalDevice");
    }
    physicalDevice = ctx.physicalDevice;
    vulkanPhysicalDevice = physicalDevice->vulkanPhysicalDevice;
    ctx.device = this;
    createLogicalDevice(*physicalDevice);
}

Device::~Device() {

    vkDestroyDevice(vulkanDevice, nullptr);
}
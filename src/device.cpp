
#include <stdexcept>
#include <set>

#include "device.h"
#include "physicalDevice.h"

//Creates the Logical Device that interfaces with the GPU
void Device::createLogicalDevice(PhysicalDevice& physicalDevice) {
    QueueFamilyIndicies indices = physicalDevice.vulkanQueueFamilies;

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    
    float queuePriority = 1.0f;
    for (uint32_t queueFamily : physicalDevice.vulkanQueueFamilies.uniqueFamilies()) {
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
    createInfo.enabledExtensionCount = static_cast<uint32_t>(context.deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = context.deviceExtensions.data();

    if (ISDEBUG) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(LAYERS.size());
        createInfo.ppEnabledLayerNames = LAYERS.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physicalDevice.vulkanPhysicalDevice, &createInfo, nullptr, &vulkanDevice) != VK_SUCCESS) {
        throw std::runtime_error("Failed to Create Logical Device");
    }

    vkGetDeviceQueue(vulkanDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(vulkanDevice, indices.presentFamily.value(), 0, &presentQueue);
}

Device::Device(VulkanContext& ctx) : context(ctx)
{
    ctx.device = this;
    createLogicalDevice(*PDEVICE);
}

Device::~Device() {

    vkDestroyDevice(vulkanDevice, nullptr);
}
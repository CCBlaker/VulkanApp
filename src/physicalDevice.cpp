
#include <vector>
#include <stdexcept>
#include <optional>
#include <set>

#include "context.h"
#include "swapChain.h"
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
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, VSURFACE, &presentSupport);
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

bool PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;

    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensionsSet(EXTENSIONS.begin(), EXTENSIONS.end());

    for (const VkExtensionProperties& extension : availableExtensions) {
        requiredExtensionsSet.erase(extension.extensionName);
    }

    return requiredExtensionsSet.empty();
}

//Check if Physical Device is Suitable for Vulkan
bool PhysicalDevice::isDeviceSuitable(VkPhysicalDevice device) {
    QueueFamilyIndicies indices = findQueueFamilies(device);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    swapChainAdequate = !SWAPCHAIN->supportDetails.formats.empty() && !SWAPCHAIN->supportDetails.presentModes.empty();

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

//Picks Physical Device for Vulkan
void PhysicalDevice::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(VINSTANCE, &deviceCount, nullptr);
    
    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find Vulkan Supported GPU");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(VINSTANCE, &deviceCount, devices.data());

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
    ctx.physicalDevice = this;
    pickPhysicalDevice();
}

PhysicalDevice::~PhysicalDevice() {
}
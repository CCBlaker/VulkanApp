#pragma once

#include <iostream>

#include <format>
#include <vector>
#include <cstring>

#include "instance.h"

//Gets Required Extensions for GLFW and Validation Layer Debugging
std::vector<const char*> Instance::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    
    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    return extensions;
}

//Checks for Availability of the Validation Layers
bool Instance::checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;
        for (const auto& availableLayer : availableLayers) {
            if (strcmp(layerName, availableLayer.layerName) == 0) {
                layerFound = true;
                break;
            }
        }
        if (!layerFound) {
            return false;
        }
    }
    return true;
}

void Instance::createInstance() {

    std::cout << "Validation Layers: " << std::format("{}", enableValidationLayers) << std::endl;
    
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("Validation Layers not Available");
    }

    //Init Vulkan Instance Info
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "TriangleRasterizer";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "idk";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensionNames = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<int32_t>(extensionNames.size());
    createInfo.ppEnabledExtensionNames = extensionNames.data();

    //Check for Alternate Extension Support
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    //Pass Validation Layer info
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    //Create Vulkan Instance
    if (vkCreateInstance(&createInfo, nullptr, &vulkanInstance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

Instance::Instance(VulkanContext& ctx) : context(ctx), enableValidationLayers(ctx.enableValidationLayers), validationLayers(ctx.validationLayers)
{
    ctx.instance = this;
    createInstance();
}

Instance::~Instance() {
    vkDestroyInstance(vulkanInstance, nullptr);
}
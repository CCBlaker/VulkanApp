#pragma once

#include <iostream>
#include <stdexcept>

#include <cstring>

#include "debugMessenger.h"
#include "instance.h"

//Creates Debug Messenger Instance at runtime
VkResult DebugMessenger::CreateDebugUtilsMessengerEXT(VkInstance instance, 
                                const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
                                const VkAllocationCallbacks* pAllocator, 
                                      VkDebugUtilsMessengerEXT* pDebugMessenger) {

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger); 
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

//Destroys Debug Messenger Instance at runtime
void DebugMessenger::DestroyDebugUtilsMessengerEXT(VkInstance instance, 
                                  VkDebugUtilsMessengerEXT debugMessenger, 
                            const VkAllocationCallbacks* pAllocator) {

    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

//Debug Callback Function
VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessenger::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeveriy,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
        std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;
        
return VK_FALSE;
}

//Populates Debug Messenger with Error Types and Severities
void DebugMessenger::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

    createInfo.messageSeverity = 
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    createInfo.messageType = 
    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr;
}

void DebugMessenger::createDebugMessenger() {
    if (!ISDEBUG) return;
    
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    populateDebugMessengerCreateInfo(createInfo);

    //Create Debug Messenger Instance 
    if (CreateDebugUtilsMessengerEXT(VINSTANCE, &createInfo, nullptr, &vulkanDebugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("Failed to Initialize Debug Messenger");
    }
}

void DebugMessenger::destroyDebugMessenger() {
    if (ISDEBUG && vulkanDebugMessenger != VK_NULL_HANDLE) {
        DestroyDebugUtilsMessengerEXT(VINSTANCE, vulkanDebugMessenger, nullptr);
    }
}

DebugMessenger::DebugMessenger(VulkanContext& ctx) : context(ctx)
{
    ctx.debugMessenger = this;
    createDebugMessenger();
}

DebugMessenger::~DebugMessenger() {
    destroyDebugMessenger();
}
#pragma once

#include "context.h"
#include <vector>

class DebugMessenger {
    public:

        VkDebugUtilsMessengerEXT vulkanDebugMessenger;

        DebugMessenger(VulkanContext& context);

        ~DebugMessenger();
        
    private:

        VulkanContext& context;
        VkInstance vulkanInstance;
        bool enableValidationLayers;
        std::vector<const char*> validationLayers;

        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, 
                        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
                        const VkAllocationCallbacks* pAllocator, 
                                VkDebugUtilsMessengerEXT* pDebugMessenger);

        void DestroyDebugUtilsMessengerEXT(VkInstance instance, 
                                    VkDebugUtilsMessengerEXT debugMessenger, 
                            const VkAllocationCallbacks* pAllocator);

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
                                                VkDebugUtilsMessageSeverityFlagBitsEXT messageSeveriy,
                                                VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                void* pUserData);

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        void createDebugMessenger();

        void destroyDebugMessenger();
};
#pragma once

#include "context.h"
#include <vector>

class Instance {
    public:
    
        VkInstance vulkanInstance;

        Instance(VulkanContext& context);
        
        ~Instance();


    private:
        
        VulkanContext& context;
        bool enableValidationLayers;
        std::vector<const char*> validationLayers;

        std::vector<const char*> getRequiredExtensions();

        bool checkValidationLayerSupport();

        void createInstance();

};
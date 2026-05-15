#pragma once

#include "context.h"

class Instance {
    public:
    
        VkInstance vulkanInstance;

        Instance(VulkanContext& context);
        
        ~Instance();

    private:
        
        VulkanContext& context;
        
        std::vector<const char*> getRequiredExtensions();
        bool checkValidationLayerSupport();

        void createInstance();

};
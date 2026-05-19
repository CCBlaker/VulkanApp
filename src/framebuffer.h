
#pragma once

#include "context.h"

class Framebuffer {
    public:

        Framebuffer(VulkanContext& context);
        
        void createFramebuffer();
        
        ~Framebuffer();

        std::vector<VkFramebuffer> vulkanFramebuffers; 
        
    private:

        VulkanContext& context;

};
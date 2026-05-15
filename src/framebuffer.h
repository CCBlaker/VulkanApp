
#pragma once

#include "context.h"

class Framebuffer {
    public:

        Framebuffer(VulkanContext& context);
        
        ~Framebuffer();

        std::vector<VkFramebuffer> vulkanFramebuffers; 
        
    private:

        void createFramebuffer();

        VulkanContext& context;

};
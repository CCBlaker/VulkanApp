#pragma once

#include "context.h"

class Renderpass {
    public:

        Renderpass(VulkanContext& context);
        
        ~Renderpass();

        
        VkRenderPass vulkanRenderpass;
        VkSurfaceFormatKHR surfaceFormat;

    private:

        void createRenderpass();
        void pickSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

        VulkanContext& context;

};
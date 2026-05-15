#pragma once

#include "context.h"

class Renderpass {
    public:

        Renderpass(VulkanContext& context);
        
        ~Renderpass();

        VkRenderPass vulkanRenderpass;

    private:

        void createRenderpass();

        VulkanContext& context;

};
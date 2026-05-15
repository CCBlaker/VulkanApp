
#pragma once

#include "context.h"

class Commandbuffer {
    public:

        Commandbuffer(VulkanContext& context);
        
        ~Commandbuffer();

        VkCommandPool vulkanCommandPool;
        VkCommandBuffer vulkanCommandbuffer;

    private:

        void createCommandPool();
        void allocateCommandbuffer();
        void recordCommandbuffer(VkCommandBuffer commandbuffer, uint32_t imageIndex);

        VulkanContext& context;

};
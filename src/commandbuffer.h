
#pragma once

#include "context.h"

class Commandbuffer {
    public:

        Commandbuffer(VulkanContext& context);

        void recordCommandbuffer(VkCommandBuffer commandbuffer, uint32_t imageIndex);
        
        ~Commandbuffer();

        VkCommandPool vulkanCommandPool;
        VkCommandBuffer vulkanCommandbuffer;

    private:

        void createCommandPool();
        void allocateCommandbuffer();

        VulkanContext& context;

};
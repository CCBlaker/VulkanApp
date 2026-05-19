
#pragma once

#include "context.h"

class Commandbuffer {
    public:

        Commandbuffer(VulkanContext& context);

        void recordCommandbuffer(VkCommandBuffer commandbuffer, uint32_t imageIndex);
        
        ~Commandbuffer();

        VkCommandPool vulkanCommandPool;
        std::vector<VkCommandBuffer> vulkanCommandbuffers;

    private:

        void createCommandPool();
        void allocateCommandbuffers();

        VulkanContext& context;

};
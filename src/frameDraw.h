#pragma once

#include "context.h"

class FrameDraw {
    public:
    
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> imageFinishedSemaphores;

        std::vector<VkFence> frameFlightFences;
        
        FrameDraw(VulkanContext& context);

        uint32_t currentFrame;
        bool swapChainAdequate;
        bool framebufferResized;

        void drawFrame();
        
        ~FrameDraw();

    private:
        
        VulkanContext& context;

        void createSyncObjects();
};
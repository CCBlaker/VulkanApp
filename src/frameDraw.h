#pragma once

#include "context.h"

class FrameDraw {
    public:
    
        VkSemaphore imageAvailableSemaphore;
        VkSemaphore imageFinishedSemaphore;

        VkFence frameFlightFence;

        FrameDraw(VulkanContext& context);
        
        void drawFrame();
        
        ~FrameDraw();

    private:
        
        VulkanContext& context;

        void createSyncObjects();


};
#pragma once

#include "context.h"

class Window {
    public:
    
        const uint32_t WINWIDTH = 1080;
        const uint32_t WINHEIGHT = 1080;

        GLFWwindow* glWindow;
        VkSurfaceKHR vulkanSurface;

        Window(VulkanContext& context);
        
        ~Window();

    private:

        VulkanContext& context;
        VkInstance vulkanInstance;

        void initWindow();

        void createSurface();

};
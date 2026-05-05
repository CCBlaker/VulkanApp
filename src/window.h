#pragma once

#include "context.h"

class Window {
    public:
    
        const uint32_t WINWIDTH = 800;
        const uint32_t WINHEIGHT = 800;

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
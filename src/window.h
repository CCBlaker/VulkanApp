#pragma once

#include "context.h"

class Window {
    public:
    
        const uint32_t winWidth = 1080;
        const uint32_t winHeight = 1080;

        GLFWwindow* glWindow;
        VkSurfaceKHR vulkanSurface;

        Window(VulkanContext& context);

        void pauseWindowConditional();
        
        ~Window();

    private:

        VulkanContext& context;
        VkInstance vulkanInstance;

        void initWindow();
        void createSurface();
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

};
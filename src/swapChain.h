#pragma once

#include "context.h"
#include "physicalDevice.h" 
#include <vector>

class SwapChain {
    public:
    
        VkSwapchainKHR vulkanSwapChain;
        
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> vulkanFramebuffers; 

        VkFormat swapChainImageFormat;
        VkPresentModeKHR presentMode;
        VkExtent2D swapChainExtent;
        uint32_t imageCount;
        
        size_t swapChainImageNum;

        SwapChain(VulkanContext& context);

        void recreateSwapChain();

        ~SwapChain();

    private:

        void cleanupSwapChain();

        void createSwapChain();
        void createImageViews();
        void createFramebuffer();

        void pickSupportDetails(const VkPhysicalDevice device, SwapSurfaceSupportDetails& supportDetails);
        VkPresentModeKHR pickPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D pickExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        VulkanContext& context;
};
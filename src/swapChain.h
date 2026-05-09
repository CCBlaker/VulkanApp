#pragma once

#include "context.h"
#include <vector>

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class SwapChain {
    public:
    
        VkSwapchainKHR vulkanSwapChain;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;
        size_t swapChainImageNum;

        SwapChainSupportDetails supportDetails;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;

        SwapChain(VulkanContext& context);
        
        ~SwapChain();


    private:

        VulkanContext& context;

        void querySwapChainSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        void createSwapChain();
        void createImageViews();


};
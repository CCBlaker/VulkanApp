
#include <cstdint>
#include <limits>
#include <algorithm>
#include <set>
#include <stdexcept>

#include "swapChain.h"
#include "window.h"
#include "renderpass.h"
#include "device.h"

VkPresentModeKHR SwapChain::pickPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::pickExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(GLWINDOW, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

void SwapChain::pickSupportDetails(const VkPhysicalDevice device, SwapSurfaceSupportDetails& supportDetails) {
    
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, VSURFACE, &supportDetails.capabilities);

    presentMode = pickPresentMode(supportDetails.presentModes);
    swapChainExtent = pickExtent(supportDetails.capabilities);
    
    imageCount = supportDetails.capabilities.minImageCount + 1;

    if (supportDetails.capabilities.maxImageCount > 0 && imageCount > supportDetails.capabilities.maxImageCount) {
        imageCount = supportDetails.capabilities.maxImageCount;
    }
}

void SwapChain::createSwapChain() {

    pickSupportDetails(PDEVICE->vulkanPhysicalDevice, PDEVICE->supportDetails);

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = VSURFACE;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = RENDERPASS->surfaceFormat.format;
    createInfo.imageColorSpace = RENDERPASS->surfaceFormat.colorSpace;
    createInfo.imageExtent = swapChainExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilyIndices[] = {PINDICES.graphicsFamily.value(), PINDICES.presentFamily.value()};;

    if (PINDICES.graphicsFamily != PINDICES.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }
    createInfo.preTransform = PDEVICE->supportDetails.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    if (vkCreateSwapchainKHR(VDEVICE, &createInfo, nullptr, &vulkanSwapChain) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Swap Chain");
    }

    vkGetSwapchainImagesKHR(VDEVICE, vulkanSwapChain, &imageCount, nullptr);
    swapChainImageNum = imageCount;
    swapChainImages.resize(swapChainImageNum);
    vkGetSwapchainImagesKHR(VDEVICE, vulkanSwapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = RENDERPASS->surfaceFormat.format;
}

void SwapChain::createImageViews() {
    swapChainImageViews.resize(swapChainImageNum);
    for (size_t i = 0; i < swapChainImageNum; i++) {

        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(VDEVICE, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to Create Image Views");
        } 

    }
}

void SwapChain::createFramebuffer() {
    vulkanFramebuffers.resize(swapChainImageViews.size());

    for (size_t i = 0; i < swapChainImageNum; i++) {

        VkImageView attachments[] = {
            swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = VRENDERPASS;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(VDEVICE, &framebufferInfo, nullptr, &vulkanFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to Create Framebuffer");
        }
    }
}

void SwapChain::cleanupSwapChain() {
    vkDestroySwapchainKHR(VDEVICE, vulkanSwapChain, nullptr);
    for (auto imageView : swapChainImageViews) {
        vkDestroyImageView(VDEVICE, imageView, nullptr);
    }
    for (auto framebuffer : vulkanFramebuffers) {
        vkDestroyFramebuffer(VDEVICE, framebuffer, nullptr);
    }
}

void SwapChain::recreateSwapChain() {    
    vkDeviceWaitIdle(VDEVICE);

    cleanupSwapChain();

    createSwapChain();
    createImageViews();
    createFramebuffer();
}


SwapChain::SwapChain(VulkanContext& ctx) : context(ctx) {

    ctx.swapChain = this;
    
    createSwapChain();
    createImageViews();
    createFramebuffer();
}

SwapChain::~SwapChain() {

    cleanupSwapChain();
}
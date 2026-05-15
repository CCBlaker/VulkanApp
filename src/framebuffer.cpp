
#include <stdexcept>

#include "framebuffer.h"
#include "renderpass.h"
#include "swapChain.h"
#include "device.h"

void Framebuffer::createFramebuffer() {
    vulkanFramebuffers.resize(SWAPCHAIN->swapChainImageViews.size());

    for (size_t i = 0; i < SWAPCHAIN->swapChainImageNum; i++) {

        VkImageView attachments[] = {
            SWAPCHAIN->swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = VRENDERPASS;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = SWAPCHAIN->swapChainExtent.width;
        framebufferInfo.height = SWAPCHAIN->swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(VDEVICE, &framebufferInfo, nullptr, &vulkanFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to Create Framebuffer");
        }
    }
}

Framebuffer::Framebuffer(VulkanContext& ctx) : context(ctx)
{
    ctx.framebuffer = this;
    createFramebuffer();
}

Framebuffer::~Framebuffer() {
    for (auto framebuffer : vulkanFramebuffers) {
        vkDestroyFramebuffer(VDEVICE, framebuffer, nullptr);
    }
}
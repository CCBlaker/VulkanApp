
#include <stdexcept>

#include "commandbuffer.h"
#include "device.h"
#include "physicalDevice.h"
#include "renderpass.h"
#include "swapChain.h"
#include "pipeline.h"

void Commandbuffer::createCommandPool() {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = PINDICES.graphicsFamily.value();

    if (vkCreateCommandPool(VDEVICE, &poolInfo, nullptr, &vulkanCommandPool) != VK_SUCCESS) {
        throw std::runtime_error("Could not Create Command Pool");
    }
}

void Commandbuffer::allocateCommandbuffers() {
    vulkanCommandbuffers.resize(context.maxFramesInFlight);

    VkCommandBufferAllocateInfo allocationInfo{};
    allocationInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocationInfo.commandPool = vulkanCommandPool;
    allocationInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocationInfo.commandBufferCount = (uint32_t) vulkanCommandbuffers.size();

    if (vkAllocateCommandBuffers(VDEVICE, &allocationInfo, vulkanCommandbuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("Failed to Allocate Command Buffers");
    }
}

void Commandbuffer::recordCommandbuffer(VkCommandBuffer commandbuffer, uint32_t imageIndex) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    if (vkBeginCommandBuffer(commandbuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to Begin Recording Command Buffer");
    }

    VkRenderPassBeginInfo renderpassInfo{};
    renderpassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderpassInfo.renderPass = VRENDERPASS;
    renderpassInfo.framebuffer = VFRAMEBUFFERS[imageIndex];
    renderpassInfo.renderArea.offset = {0, 0};
    renderpassInfo.renderArea.extent = SWAPCHAIN->swapChainExtent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderpassInfo.clearValueCount = 1;
    renderpassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandbuffer, &renderpassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VPIPELINE);

    VVIEWPORT.width = static_cast<float>(SWAPCHAIN->swapChainExtent.width);
    VVIEWPORT.height = static_cast<float>(SWAPCHAIN->swapChainExtent.height);
    VSCISSOR.extent = SWAPCHAIN->swapChainExtent;

    vkCmdSetViewport(commandbuffer, 0, 1, &PIPELINE->viewport);
    vkCmdSetScissor(commandbuffer, 0, 1, &PIPELINE->scissor);

    vkCmdDraw(commandbuffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(commandbuffer);

    if (vkEndCommandBuffer(commandbuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to Record Command Buffer");
    }
}

Commandbuffer::Commandbuffer(VulkanContext& ctx) : context(ctx)
{
    ctx.commandbuffer = this;
    createCommandPool();
    allocateCommandbuffers();
}

Commandbuffer::~Commandbuffer() {
    vkDestroyCommandPool(VDEVICE, vulkanCommandPool, nullptr);
}
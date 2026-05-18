
#include <stdexcept>
#include <cstdint>

#include "frameDraw.h"
#include "device.h"
#include "swapChain.h"
#include "commandbuffer.h"

void FrameDraw::createSyncObjects() {
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (vkCreateSemaphore(VDEVICE, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(VDEVICE, &semaphoreInfo, nullptr, &imageFinishedSemaphore) != VK_SUCCESS ||
        vkCreateFence(VDEVICE, &fenceInfo, nullptr, &frameFlightFence) != VK_SUCCESS) {
            throw std::runtime_error("Failed to Create Semaphore");
        }
}

void FrameDraw::drawFrame() {
    vkWaitForFences(VDEVICE, 1, &frameFlightFence, VK_TRUE, UINT_MAX);
    vkResetFences(VDEVICE, 1, &frameFlightFence);
   
    uint32_t imageIndex;
    vkAcquireNextImageKHR(VDEVICE, VSWAPCHAIN, UINT_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

    vkResetCommandBuffer(VCOMMANDBUFFER, 0);
    
    COMMANDBUFFER->recordCommandbuffer(VCOMMANDBUFFER, imageIndex);
    
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &VCOMMANDBUFFER;

    VkSemaphore signalSemaphores[] = {imageFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(DEVICE->graphicsQueue, 1, &submitInfo, frameFlightFence) != VK_SUCCESS) {
        throw std::runtime_error("Failed to Submit Draw Command Buffer");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {VSWAPCHAIN};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    presentInfo.pResults = nullptr;

    //FINAL PRESENT COMMAND!!!!!!!!!
    vkQueuePresentKHR(DEVICE->presentQueue, &presentInfo);
}

FrameDraw::FrameDraw(VulkanContext& ctx) : context(ctx)
{
    ctx.frameDraw = this;

    createSyncObjects();
    
}

FrameDraw::~FrameDraw() {

    vkDestroySemaphore(VDEVICE, imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(VDEVICE, imageFinishedSemaphore, nullptr);
    vkDestroyFence(VDEVICE, frameFlightFence, nullptr);

}
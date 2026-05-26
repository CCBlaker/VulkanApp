
#include <stdexcept>
#include <cstdint>

#include "frameDraw.h"
#include "device.h"
#include "swapChain.h"
#include "commandbuffer.h"
#include "window.h"

void FrameDraw::createSyncObjects() {
    imageAvailableSemaphores.resize(context.maxFramesInFlight);
    imageFinishedSemaphores.resize(context.maxFramesInFlight);
    frameFlightFences.resize(context.maxFramesInFlight);
    
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < context.maxFramesInFlight; i++) {
        if (vkCreateSemaphore(VDEVICE, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(VDEVICE, &semaphoreInfo, nullptr, &imageFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(VDEVICE, &fenceInfo, nullptr, &frameFlightFences[i]) != VK_SUCCESS) {
                throw std::runtime_error("Failed to Create Synchronization Objects for Frame");
        }
    }
}

void FrameDraw::drawFrame() {

    swapChainAdequate = true;

    vkWaitForFences(VDEVICE, 1, &frameFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(VDEVICE, VSWAPCHAIN, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        swapChainAdequate = false;
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("Failed to Acquire Swapchain Image");
    }

    vkResetFences(VDEVICE, 1, &frameFlightFences[currentFrame]);

    vkResetCommandBuffer(VCOMMANDBUFFERS[currentFrame], 0);
    COMMANDBUFFER->recordCommandbuffer(VCOMMANDBUFFERS[currentFrame], imageIndex);
    
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &VCOMMANDBUFFERS[currentFrame];

    VkSemaphore signalSemaphores[] = {imageFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(DEVICE->graphicsQueue, 1, &submitInfo, frameFlightFences[currentFrame]) != VK_SUCCESS) {
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
    result = vkQueuePresentKHR(DEVICE->presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        swapChainAdequate = false;
        
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to Present Swapchain Image");
    }

    currentFrame = (currentFrame + 1) % MAXFRAMESINFLIGHT;
}

FrameDraw::FrameDraw(VulkanContext& ctx) : context(ctx)
{
    ctx.frameDraw = this;
    currentFrame = 0;
    framebufferResized = false;

    createSyncObjects();
    
    glfwSetWindowUserPointer(GLWINDOW, this);
}

FrameDraw::~FrameDraw() {

    for (size_t i = 0; i < context.maxFramesInFlight; i++) {
        vkDestroySemaphore(VDEVICE, imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(VDEVICE, imageFinishedSemaphores[i], nullptr);
        vkDestroyFence(VDEVICE, frameFlightFences[i], nullptr);
    }
}
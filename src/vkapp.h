#pragma once

#include <memory>
#include "window.h"
#include "instance.h"
#include "debugMessenger.h"
#include "physicalDevice.h"
#include "device.h"
#include "swapChain.h"
#include "renderpass.h"
#include "pipeline.h"
#include "framebuffer.h"
#include "commandbuffer.h"
#include "syncObjects.h"

#include "context.h"

class VulkanApp {
public:
    void run() {
        init();
        mainLoop();
        cleanup();
    }

private:
    std::unique_ptr<VulkanContext> context;

    std::unique_ptr<Instance> instance;
    std::unique_ptr<DebugMessenger> debugMessenger;
    std::unique_ptr<Window> window;
    std::unique_ptr<PhysicalDevice> physicalDevice;
    std::unique_ptr<Device> device;
    std::unique_ptr<SwapChain> swapChain;
    std::unique_ptr<Renderpass> renderpass;
    std::unique_ptr<Pipeline> pipeline;
    std::unique_ptr<Framebuffer> framebuffer;
    std::unique_ptr<Commandbuffer> commandbuffer;
    // std::unique_ptr<SyncObjects> syncObjects;


    void init();
    void mainLoop();
    void cleanup();
};
#pragma once

#include "context.h"
#include "shader.h"

class Pipeline {
    public:

        Pipeline(VulkanContext& context);
        
        ~Pipeline();

        VkPipeline vulkanGraphicsPipeline; 
        VkPipelineLayout vulkanPipelineLayout;

        VkViewport viewport{};
        VkRect2D scissor{};


    private:

        Shader vertShader;
        Shader fragShader;

        void createGraphicsPipeline();
        void createPipelineLayout();


        VulkanContext& context;

};
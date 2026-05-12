#pragma once

#include "context.h"
#include "shader.h"

class Pipeline {
    public:

        Pipeline(VulkanContext& context);
        
        ~Pipeline();

        VkPipeline vulkanPipeline; 

    private:

        Shader vertShader;
        Shader fragShader;

        void createGraphicsPipeline();

        VulkanContext& context;

};
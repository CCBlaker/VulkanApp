#include "pipeline.h"

void Pipeline::createGraphicsPipeline() {
    VkPipelineShaderStageCreateInfo vertShaderInfo{};
    vertShaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderInfo.module = vertShader.vulkanShader;
    vertShaderInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderInfo{};
    fragShaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderInfo.module = fragShader.vulkanShader;
    fragShaderInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderInfo, fragShaderInfo};
}

Pipeline::Pipeline(VulkanContext& ctx) :
  context(ctx),
  vertShader(ctx, "shaders/vert.spv"),
  fragShader(ctx, "shaders/frag.spv")
{
    ctx.pipeline = this;

    createGraphicsPipeline();
}

Pipeline::~Pipeline() {

}

#include <stdexcept>

#include "pipeline.h"
#include "device.h"
#include "swapChain.h"
#include "renderpass.h"

void Pipeline::createPipelineLayout() {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(VDEVICE, &pipelineLayoutInfo, nullptr, &vulkanPipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to Create Pipeline Layout");
    }

}

void Pipeline::createGraphicsPipeline() {

    //Shaders
    
    //Vertex Shader
    VkPipelineShaderStageCreateInfo vertShaderInfo{};
    vertShaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderInfo.module = vertShader.vulkanShader;
    vertShaderInfo.pName = "main";


    //Fragment Shader
    VkPipelineShaderStageCreateInfo fragShaderInfo{};
    fragShaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderInfo.module = fragShader.vulkanShader;
    fragShaderInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderInfo, fragShaderInfo};

    //Fixed Function Pipeline States----------

    //Vertex Input State
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    //Viewport Display State

    //Viewport Rect
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = SWAPCHAIN->swapChainExtent.width;
    viewport.height = SWAPCHAIN->swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport. maxDepth = 1.0f;

    //Scissor Rect
    scissor.offset = {0, 0};
    scissor.extent = SWAPCHAIN->swapChainExtent;

    //Viewport State
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    //Dynamic State
    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();


    //Rasterization State

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;

    //Multisampling State

    VkPipelineMultisampleStateCreateInfo multisampler{};
    multisampler.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampler.sampleShadingEnable = VK_FALSE;
    multisampler.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampler.minSampleShading = 1.0f;
    multisampler.pSampleMask = nullptr;
    multisampler.alphaToCoverageEnable = VK_FALSE;
    multisampler.alphaToOneEnable = VK_FALSE;

    //Color Blending State

    //Per-Framebuffer
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
                                          VK_COLOR_COMPONENT_G_BIT |
                                          VK_COLOR_COMPONENT_B_BIT |
                                          VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    //Global
    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    //Final Pipeline Creation--------------

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampler;
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = vulkanPipelineLayout;
    pipelineInfo.renderPass = VRENDERPASS;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    if (vkCreateGraphicsPipelines(VDEVICE, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &vulkanGraphicsPipeline) != VK_SUCCESS) {
        throw std::runtime_error("Failed to Create Graphics Pipeline");
    }
}

Pipeline::Pipeline(VulkanContext& ctx) :
  context(ctx),
  vertShader(ctx, "shaders/vert.spv"),
  fragShader(ctx, "shaders/frag.spv")
{
    ctx.pipeline = this;

    createPipelineLayout();
    createGraphicsPipeline();
}

Pipeline::~Pipeline() {
    vkDestroyPipelineLayout(VDEVICE, vulkanPipelineLayout, nullptr);
    vkDestroyPipeline(VDEVICE, vulkanGraphicsPipeline, nullptr);
}
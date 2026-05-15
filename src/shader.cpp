
#include <fstream>

#include "shader.h"
#include "device.h"

std::vector<char> Shader::readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to Open File");
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer;
}

void Shader::createShaderModule(const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(VDEVICE, &createInfo, nullptr, &vulkanShader) != VK_SUCCESS) {
        throw std::runtime_error("Failed to Create Shader Module");
    }
}


Shader::Shader(VulkanContext& ctx, const std::string& name) : context(ctx), filename(name)
{
    std::vector<char> code = readFile(name);

    createShaderModule(code);
}

Shader::~Shader() {
    vkDestroyShaderModule(VDEVICE, vulkanShader, nullptr);
}
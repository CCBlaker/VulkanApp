#pragma once

#include <string>
#include <vector>

#include "context.h"

class Shader {
    public:

        Shader(VulkanContext& context, const std::string& filename);

        VkShaderModule vulkanShader;

        static std::vector<char> readFile(const std::string& filename);

        void createShaderModule(const std::vector<char>& code);

        ~Shader();

    private:

        VulkanContext& context;

        std::string filename;
};
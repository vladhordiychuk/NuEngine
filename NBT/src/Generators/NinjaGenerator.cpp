#include "NBT/Generators/NinjaGenerator.hpp"
#include <iostream>

namespace NBT
{
    NinjaGenerator::NinjaGenerator(std::filesystem::path outputDir, std::filesystem::path projectRoot)
        : m_outputDir(std::move(outputDir)), m_projectRoot(std::move(projectRoot))
    {
    }

    bool NinjaGenerator::Generate(const std::string& projectName, const std::vector<Module>& modules)
    {
        std::cerr << "Ninja generator is not implemented yet!\n";
        return false;
    }
}
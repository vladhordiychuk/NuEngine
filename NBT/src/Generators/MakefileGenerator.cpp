#include "NBT/Generators/MakefileGenerator.hpp"
#include <iostream>

namespace NBT
{
    MakefileGenerator::MakefileGenerator(std::filesystem::path outputDir, std::filesystem::path projectRoot)
        : m_outputDir(std::move(outputDir)), m_projectRoot(std::move(projectRoot))
    {
    }

    bool MakefileGenerator::Generate(const std::string& projectName, const std::vector<Module>& modules)
    {
        std::cerr << "Makefile generator is not implemented yet!\n";
        return false;
    }
}
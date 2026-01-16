#pragma once
#include "IProjectGenerator.hpp"
#include <filesystem>

namespace NBT
{
    class VSGenerator : public IProjectGenerator
    {
    public:
        VSGenerator(std::filesystem::path outputDir, std::filesystem::path projectRoot, std::filesystem::path projectFileDir);
        bool Generate(const std::string& projectName, const std::vector<Module>& modules) override;

    private:
        void GenerateSolution(const std::string& solutionName, const std::vector<Module>& modules);
        void GenerateVCXProj(const Module& mod);

        std::filesystem::path m_outputDir;       // ../build
        std::filesystem::path m_projectRoot;     // ../
        std::filesystem::path m_projectFileDir;  // Game/
    };
}
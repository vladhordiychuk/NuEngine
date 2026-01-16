#pragma once
#include "NBT/Core/Config.hpp"
#include "NBT/Core/Module.hpp"
#include "NBT/Generators/IProjectGenerator.hpp"
#include <unordered_map>
#include <filesystem>
#include <vector>
#include <string>

namespace NBT
{
    class BuildSystem
    {
    public:
        bool LoadProject(const std::string& projectPath);
        bool Generate(IProjectGenerator* generator);
        std::filesystem::path GetOutputDir() const { return m_outputDir; }
        std::filesystem::path GetProjectRoot() const { return m_projectRoot; }
        std::filesystem::path GetProjectFileDir() const { return m_projectFileDir; } // Новий геттер

    private:
        void LoadTargets();
        void LoadDependencies(Module& target);
        std::vector<std::string> ExpandGlob(const std::string& pattern);

        Config m_projectConfig;
        std::string m_projectName;
        std::unordered_map<std::string, Module> modules;

        std::filesystem::path m_projectRoot;     // ../ (батьківська папка)
        std::filesystem::path m_projectFileDir;  // Game/ (де .nuproject)
        std::filesystem::path m_outputDir;       // ../build
    };
}
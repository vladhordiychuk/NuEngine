#include "NBT/Core/BuildSystem.hpp"
#include <fstream>
#include <iostream>
#include <regex>
#include <filesystem>

namespace NBT
{
    std::vector<std::string> BuildSystem::ExpandGlob(const std::string& pattern)
    {
        std::vector<std::string> result;
        std::filesystem::path base_path = m_projectFileDir / std::filesystem::path(pattern).parent_path();
        if (base_path.empty()) base_path = m_projectFileDir;

        std::string regex_str = std::regex_replace(pattern, std::regex("\\*\\*"), ".*");
        regex_str = std::regex_replace(regex_str, std::regex("\\*"), "[^/]*");
        std::regex re(regex_str);

        try
        {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(base_path))
            {
                if (entry.is_regular_file())
                {
                    std::string rel = std::filesystem::relative(entry.path(), m_projectFileDir).string();
                    std::replace(rel.begin(), rel.end(), '\\', '/');
                    if (std::regex_match(rel, re))
                    {
                        result.push_back(rel);
                    }
                }
            }
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            std::cerr << "Warning: Could not expand glob pattern '" << pattern << "'. Error: " << e.what() << "\n";
        }
        return result;
    }

    bool BuildSystem::LoadProject(const std::string& project_path)
    {
        std::filesystem::path projectFilePath = std::filesystem::absolute(project_path);
        m_projectFileDir = projectFilePath.parent_path();                    // Game/
        m_projectRoot = m_projectFileDir.parent_path();                      // ../
        m_outputDir = m_projectRoot / "build";                               // ../build
        std::filesystem::create_directories(m_outputDir);

        try
        {
            m_projectConfig = Config(toml::parse(project_path));
            std::cout << "Loaded project: " << m_projectFileDir.string() << "\n";
        }
        catch (const std::exception& e)
        {
            std::cerr << "Parse error: " << e.what() << "\n";
            return false;
        }

        m_projectName = m_projectConfig.GetString("project.name", "NuEngine");
        LoadTargets();
        return true;
    }

    bool BuildSystem::Generate(IProjectGenerator* generator)
    {
        if (!generator)
        {
            std::cerr << "Error: No generator provided!\n";
            return false;
        }

        std::cout << "Starting generation with " << m_projectName << "...\n";
        std::vector<Module> module_list;
        for (const auto& pair : modules)
        {
            module_list.push_back(pair.second);
        }
        return generator->Generate(m_projectName, module_list);
    }

    void BuildSystem::LoadTargets()
    {
        if (!m_projectConfig.Has("targets"))
        {
            std::cout << "Warning: No [targets] found in project file.\n";
            return;
        }

        try
        {
            const auto targets_value = m_projectConfig.GetValue("targets");
            if (!targets_value.is_table())
            {
                std::cerr << "Error: [targets] is not a table.\n";
                return;
            }

            const auto& targets = targets_value.as_table();
            for (const auto& pair : targets)
            {
                const std::string& targetName = pair.first;
                const toml::value& targetValue = pair.second;
                Module mod;
                mod.name = targetName;
                mod.type = toml::find_or(targetValue, "type", "executable");

                auto patterns = toml::find_or(targetValue, "sources", std::vector<std::string>{});
                for (const auto& pattern : patterns)
                {
                    auto files = ExpandGlob(pattern);
                    mod.sources.insert(mod.sources.end(), files.begin(), files.end());
                }

                mod.dependencies = toml::find_or(targetValue, "dependencies", std::vector<std::string>{});
                modules[mod.name] = mod;
                std::cout << "Found target: " << mod.name << "\n";
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error parsing [targets]: " << e.what() << "\n";
        }
    }

    void BuildSystem::LoadDependencies(Module& target)
    {
        // ...
    }
}
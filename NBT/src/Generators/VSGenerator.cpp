#include "NBT/Generators/VSGenerator.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <filesystem>

namespace NBT
{
    VSGenerator::VSGenerator(std::filesystem::path outputDir, std::filesystem::path projectRoot, std::filesystem::path projectFileDir)
        : m_outputDir(std::move(outputDir))
        , m_projectRoot(std::move(projectRoot))
        , m_projectFileDir(std::move(projectFileDir))
    {
    }

    bool VSGenerator::Generate(const std::string& projectName, const std::vector<Module>& modules)
    {
        try
        {
            GenerateSolution(projectName, modules);
            return true;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error during VS generation: " << e.what() << "\n";
            return false;
        }
    }

    void VSGenerator::GenerateSolution(const std::string& solutionName, const std::vector<Module>& modules)
    {
        std::ofstream sln(m_outputDir / (solutionName + ".sln"));
        sln << "Microsoft Visual Studio Solution File, Format Version 12.00\n";
        sln << "# Visual Studio 17\n";

        for (const auto& mod : modules)
        {
            GenerateVCXProj(mod);
            std::string projFile = mod.name + ".vcxproj";
            sln << "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"" << mod.name << "\", \""
                << projFile << "\", \"{11111111-1111-1111-1111-" << mod.name << "}\"\n";
            sln << "EndProject\n";
        }

        sln << "Global\n"
            << "    GlobalSection(SolutionConfigurationPlatforms) = preSolution\n"
            << "        Debug|x64 = Debug|x64\n"
            << "    EndGlobalSection\n"
            << "    GlobalSection(ProjectConfigurationPlatforms) = postSolution\n";

        for (const auto& mod : modules)
        {
            std::string guid = "{11111111-1111-1111-1111-" + mod.name + "}";
            sln << "        " << guid << ".Debug|x64.ActiveCfg = Debug|x64\n"
                << "        " << guid << ".Debug|x64.Build.0 = Debug|x64\n";
        }

        sln << "    EndGlobalSection\n"
            << "EndGlobal\n";

        std::cout << "Generated Solution: " << (m_outputDir / (solutionName + ".sln")).string() << "\n";
    }

    void VSGenerator::GenerateVCXProj(const Module& mod)
    {
        auto proj_path = m_outputDir / (mod.name + ".vcxproj");
        std::ofstream proj(proj_path);

        proj << R"(<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup Label="ProjectConfigurations">
    <ProjectConfiguration Include="Debug|x64"><Configuration>Debug</Configuration><Platform>x64</Platform></ProjectConfiguration>
  </ItemGroup>
  <PropertyGroup Label="Globals">
    <ProjectGuid>{11111111-1111-1111-1111-)" << mod.name << R"(}</ProjectGuid>
    <Keyword>Win32Proj</Keyword>
    <WindowsTargetPlatformVersion>10.0</WindowsTargetPlatformVersion>
    <RootNamespace>)" << mod.name << R"(</RootNamespace>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.Default.props" />
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
    <ConfigurationType>)" << (mod.type == "executable" ? "Application" : "StaticLibrary") << R"(</ConfigurationType>
    <PlatformToolset>v143</PlatformToolset>
    <CharacterSet>Unicode</CharacterSet>
    <LanguageStandard>stdcpp20</LanguageStandard>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.props" />

  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <SDLCheck>true</SDLCheck>
      <PreprocessorDefinitions>_DEBUG;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <ConformanceMode>true</ConformanceMode>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
      <GenerateDebugInformation>true</GenerateDebugInformation>
    </Link>
  </ItemDefinitionGroup>

  <ItemGroup>
)";

        for (const auto& file : mod.sources)
        {
            std::filesystem::path fullSrc = m_projectFileDir / file;
            std::filesystem::path relPath = std::filesystem::relative(fullSrc, m_outputDir);
            std::string relStr = relPath.string();
            std::replace(relStr.begin(), relStr.end(), '/', '\\');
            proj << "    <ClCompile Include=\"" << relStr << "\" />\n";
        }

        proj << R"(  </ItemGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
</Project>)";

        std::cout << "Generated: " << proj_path.string() << "\n";
    }
}
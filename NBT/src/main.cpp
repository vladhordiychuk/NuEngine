#include "NBT/Core/BuildSystem.hpp"
#include "NBT/Generators/VSGenerator.hpp"
#include "NBT/Generators/MakefileGenerator.hpp"
#include "NBT/Generators/NinjaGenerator.hpp"
#include <iostream>
#include <memory>
#include <string>

namespace NBT
{
    struct Args
    {
        std::string projectFile;
        std::string generatorName = "vs2022";
    };

    bool ParseArgs(int argc, char** argv, Args& outArgs)
    {
        if (argc < 3 || std::string(argv[1]) != "build")
        {
            std::cerr << "Usage: nbt build <Project.nuproject> [--gen=GENERATOR]\n";
            std::cerr << "Available generators: vs2022, make, ninja\n";
            return false;
        }
        outArgs.projectFile = argv[2];
        if (argc > 3 && std::string(argv[3]).rfind("--gen=", 0) == 0)
        {
            outArgs.generatorName = std::string(argv[3]).substr(6);
        }
        return true;
    }

    std::unique_ptr<IProjectGenerator> CreateGenerator(const Args& args, const BuildSystem& bs)
    {
        if (args.generatorName == "vs2022")
        {
            return std::make_unique<VSGenerator>(bs.GetOutputDir(), bs.GetProjectRoot(), bs.GetProjectFileDir());
        }
        if (args.generatorName == "make")
        {
            return std::make_unique<MakefileGenerator>(bs.GetOutputDir(), bs.GetProjectRoot());
        }
        if (args.generatorName == "ninja")
        {
            return std::make_unique<NinjaGenerator>(bs.GetOutputDir(), bs.GetProjectRoot());
        }
        std::cerr << "Error: Unknown generator '" << args.generatorName << "'\n";
        return nullptr;
    }
}

int main(int argc, char** argv)
{
    NBT::Args args;
    if (!NBT::ParseArgs(argc, argv, args)) {
        return 1;
    }

    NBT::BuildSystem bs;
    if (!bs.LoadProject(args.projectFile))
    {
        std::cerr << "Build failed: Could not load project.\n";
        return 1;
    }

    std::unique_ptr<NBT::IProjectGenerator> generator = NBT::CreateGenerator(args, bs);
    if (!generator)
    {
        return 1;
    }

    if (!bs.Generate(generator.get()))
    {
        std::cerr << "Build failed: Generation step failed!\n";
        return 1;
    }

    std::cout << "Build succeeded! Open " << bs.GetOutputDir().string() << "\n";
    return 0;
}
#pragma once 

#include "Config.hpp"
#include "Module.hpp"

namespace NBT
{
    struct BuildContext
    {
        Config projectConfig;
        Module* module = nullptr;
        std:string platform = "windows";
        std::string config = "debug";

        void AddDefine(const std::string& define)
        {
            if (module)
            {
                module->defines.push_back(define);
            }
        }

        void AddInclude(const std::string& include)
        {
            if (module)
            {
                module->includes.push_back(include);
            }
        }

        void AddSource(const std::string& source)
        {
            if (module)
            {
                module->sources.push_back(source);
            }
        }

        void AddLibrary(const std::string& library)
        {
            if (module)
            {
                module->linkLibraries.push_back(library);
            }
        }
    }
}
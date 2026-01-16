#pragma once

#include "Config.hpp"
#include <vector>
#include <string>

namespace NBT
{
	struct Module
	{
		std::string name;
		std::string type = "static";
		std::vector<std::string> sources;
		std::vector<std::string> includes;
		std::vector<std::string> defines;
		std::vector<std::string> dependencies;
		std::vector<std::string> linkLibraries;

		bool Load(const std::string& path);
	};
}
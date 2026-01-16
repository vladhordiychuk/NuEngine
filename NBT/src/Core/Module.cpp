#include "NBT/Core/Module.hpp"

#include "toml.hpp"
#include <iostream>

namespace NBT
{
	bool Module::Load(const std::string& path)
	{
		try {
			auto table = toml::parse(path);
			if (!table.contains("module")) {
				std::cerr << "No [module] in " << path << "\n";
				return false;
			}

			const auto& mod = table.at("module");
			name = toml::get<std::string>(mod.at("name"));
			type = toml::find_or(mod, "type", "static");
			sources = toml::find_or(mod, "sources", std::vector<std::string>{});
			includes = toml::find_or(mod, "includes", std::vector<std::string>{});
			defines = toml::find_or(mod, "defines", std::vector<std::string>{});
			dependencies = toml::find_or(mod, "dependencies", std::vector<std::string>{});
			linkLibraries = toml::find_or(mod, "linkLibraries", std::vector<std::string>{});
			return true;
		}
		catch (const std::exception& e) {
			std::cerr << "Failed to load module " << path << ": " << e.what() << "\n";
			return false;
		}
	}
}
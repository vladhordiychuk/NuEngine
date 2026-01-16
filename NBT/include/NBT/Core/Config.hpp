#pragma once

#include "toml.hpp"
#include <string>
#include <vector>

namespace NBT
{
	class Config
	{
	public:
		Config() = default;

		explicit Config(const toml::value& value) : data(value)
		{
		}

		std::string GetString(const std::string& key, const std::string& defaultValue = "") const
		{
			return toml::find_or(data, key, defaultValue);
		}

		bool GetBool(const std::string& key, bool defaultValue = false) const
		{
			return toml::find_or(data, key, defaultValue);
		}

		std::vector<std::string> GetArray(const std::string& key) const
		{
			try
			{
				auto array = toml::find<toml::array>(data, key);
				std::vector<std::string> result;
				for (const auto& value : array)
				{
					result.push_back(value.as_string());
				}
				return result;
			}
			catch (...)
			{
				return {};
			}
		}

		bool Has(const std::string& key) const
		{
			return data.contains(key);
		}

		toml::value GetValue(const std::string& key) const
		{
			return toml::find(data, key);
		}

	private:
		toml::value data;
	};
}

#pragma once

#include <string>
#include <vector>
#include <mutex>

#include <Core/Types/Types.hpp>
#include <Core/Types/Result.hpp>
#include <Core/Errors/FileSystemError.hpp>

namespace NuEngine::Core
{
    using NuChar = NuEngine::Core::Types::NuChar;
    using NuBool = NuEngine::Core::Types::NuBool;
    using NuSize = NuEngine::Core::Types::NuSize;

    /*
    * @class FileSystem
    *
    * @brief Manages file operations relative to a base path.
    *
    * The FileSystem class provides utilities for reading, writing, and checking
    * the existence of files, as well as resolving absolute paths.
    */
    class FileSystem
    {
    public:
        /*
        * @brief Constructs a FileSystem with a base path.
        *
        * @param basePath The root directory for file operations.
        */
        FileSystem(const std::string& basePath);

        /*
        * @brief Reads the contents of a file into a vector of NuChar.
        *
        * @param path The relative or absolute path to the file.
        *
        * @return Result with the file's contents or FileSystemError.
        */
        [[nodiscard]] NuEngine::Core::Result<std::vector<NuChar>, FileSystemError> ReadFile(const std::string& path) const noexcept;

        /*
        * @brief Writes data to a file.
        *
        * @param path The relative or absolute path to the file.
        * @param data The data to write.
        *
        * @return Result with void (success) or FileSystemError.
        */
        [[nodiscard]] NuEngine::Core::Result<void, FileSystemError> WriteFile(const std::string& path, const std::vector<NuChar>& data) noexcept;

        /*
        * @brief Checks if a file exists.
        *
        * @param path The relative or absolute path to the file.
        *
        * @return True if the file exists, false otherwise.
        */
        [[nodiscard]] NuBool FileExists(const std::string& path) const noexcept;

        /*
        * @brief Resolves a relative path to an absolute path based on the base path.
        *
        * @param path The relative path to resolve.
        *
        * @return Result with the absolute path or FileSystemError.
        */
        [[nodiscard]] NuEngine::Core::Result<std::string, FileSystemError> GetAbsolutePath(const std::string& path) const noexcept;

    private:
        std::string m_basePath; // Base path for file operations
        mutable std::mutex m_mutex;

        /*
        * @brief Converts a relative path to an absolute one, considering the base path.
        *
        * @param path The relative path (may start with "res://").
        *
        * @return The absolute path as a string.
        */
        [[nodiscard]] std::string ResolvePath(const std::string& path) const noexcept;
    };
}
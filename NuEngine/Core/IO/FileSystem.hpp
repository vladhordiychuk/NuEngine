// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <string>
#include <vector>
#include <mutex>

#include <Core/Types/Result.hpp>
#include <Core/Errors/FileSystemError.hpp>

namespace NuEngine::Core
{
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
        * @brief Reads the contents of a file into a vector of char.
        *
        * @param path The relative or absolute path to the file.
        *
        * @return Result with the file's contents or FileSystemError.
        */
        [[nodiscard]] Result<std::vector<char>, FileSystemError> ReadFile(const std::string& path) const;

        /*
        * @brief Writes data to a file.
        *
        * @param path The relative or absolute path to the file.
        * @param data The data to write.
        *
        * @return Result with void (success) or FileSystemError.
        */
        [[nodiscard]] Result<void, FileSystemError> WriteFile(const std::string& path, const std::vector<char>& data);

        /*
        * @brief Checks if a file exists.
        *
        * @param path The relative or absolute path to the file.
        *
        * @return True if the file exists, false otherwise.
        */
        [[nodiscard]] bool FileExists(const std::string& path) const;

        /**
         * @brief Resolves a relative path to an absolute path based on the base path.
         *
         * @param path The relative path to resolve.
         *
         * @return Result with the absolute path or FileSystemError.
         */
        [[nodiscard]] Result<std::string, FileSystemError> GetAbsolutePath(const std::string& path) const;

        [[nodiscard]] std::string GetBasePath() const noexcept;

        

    private:
        std::string m_basePath;         // Base path for file operations

        mutable std::mutex m_mutex;

        /**
         * @brief Converts a relative path to an absolute one, considering the base path.
         *
         * @param path The relative path (may start with "res://").
         *
         * @return The absolute path as a string.h
         */
        [[nodiscard]] std::string ResolvePath(const std::string& path) const noexcept;
    };
}
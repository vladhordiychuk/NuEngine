// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <string>

#include <Core/Types/Types.hpp>

namespace NuEngine::Core
{
    /*
    * @brief List of errors for file operations.
    */
    enum class FileSystemError
    {
        Success,                    // The operation was a success.
        FileNotFound,               // File not found.
        PermissionDenied,           // No access rights.
        InvalidPath,                // Incorrect path.
        DirectoryCreationFailed,    // Failed to create the directory.
        ReadFailed,                 // File read error.
        WriteFailed,                // File write error.
        OutOfMemory,                // Not enough memory.
        InvalidBasePath,            // Incorrect base path.
        PlatformFailure,            // Platform-specific API error.
    };

    [[nodiscard]] NU_FORCEINLINE std::string ToString(FileSystemError error) {
        switch (error)
        {
            case FileSystemError::Success: return "Success";
            case FileSystemError::FileNotFound: return "File not found";
            case FileSystemError::PermissionDenied: return "Permission denied";
            case FileSystemError::InvalidPath: return "Invalid path";
            case FileSystemError::DirectoryCreationFailed: return "Directory creation failed";
            case FileSystemError::ReadFailed: return "Read failed";
            case FileSystemError::WriteFailed: return "Write failed";
            case FileSystemError::OutOfMemory: return "Out of memory";
            case FileSystemError::InvalidBasePath: return "Invalid base path";
            case FileSystemError::PlatformFailure: return "Platform-specific API error";
        default: 
            return "Unknown error";
        }
    }
}
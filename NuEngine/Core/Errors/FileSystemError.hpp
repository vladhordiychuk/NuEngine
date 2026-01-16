// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <string>
#include <iostream>
#include <vector>

#include <Core/Types/Types.hpp>
#include <Core/Types/ErrorContext.hpp>

namespace NuEngine::Core
{
    enum class FileSystemErrorCode
    {
        Success,
        FileNotFound,
        PermissionDenied,
        InvalidPath,
        DirectoryCreationFailed,
        ReadFailed,
        WriteFailed,
        OutOfMemory,
        InvalidBasePath,
        PlatformFailure,
    };

    [[nodiscard]] NU_FORCEINLINE std::string_view ToErrorString(FileSystemErrorCode code) noexcept
    {
        switch (code)
        {
        [[likely]] case FileSystemErrorCode::Success:                 
            return "Success";
        case FileSystemErrorCode::FileNotFound:           
            return "File not found";
        case FileSystemErrorCode::PermissionDenied:        
            return "Permission denied";
        case FileSystemErrorCode::InvalidPath:             
            return "Invalid path";
        case FileSystemErrorCode::DirectoryCreationFailed: 
            return "Directory creation failed";
        case FileSystemErrorCode::ReadFailed:              
            return "Read failed";
        case FileSystemErrorCode::WriteFailed:             
            return "Write failed";
        case FileSystemErrorCode::OutOfMemory:             
            return "Out of memory";
        case FileSystemErrorCode::InvalidBasePath:        
            return "Invalid base path";
        case FileSystemErrorCode::PlatformFailure:         
            return "Platform-specific API error";
        default:                                           
            return "Unknown error";
        }
    }

    struct FileSystemError
    {
        ErrorSeverity severity;
        FileSystemErrorCode code;
        std::string path;
        std::string details;
        std::vector<Core::ErrorContext> context;

        FileSystemError(FileSystemErrorCode c, std::string p = "", std::string d = "", Core::ErrorContext ctx = {}) noexcept
            : code(c), path(std::move(p)), details(std::move(d))
        {
            context.push_back(ctx);
        }

        [[nodiscard]] std::string ToString() const noexcept
        {
            std::string result(ToErrorString(code));

            if (!path.empty())
            {
                result += ": '" + path + "'";
            }
            if (!details.empty())
            {
                result += " (" + details + ")";
            }
            return result;
        }
    };

    NU_FORCEINLINE std::ostream& operator<<(std::ostream& os, const FileSystemError& e)
    {
        return os << e.ToString();
    }

    [[nodiscard]] NU_FORCEINLINE std::string ToString(const FileSystemError& e)
    {
        return e.ToString();
    }
}
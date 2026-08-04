// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Core/Types/ErrorTraits.hpp>
#include <Core/Types/BasicError.hpp>

namespace NuEngine::Core
{
	/**
	 * @brief Error codes for file system operations.
	 */
	enum class FileSystemErrorCode : uint8_t
	{
		Success,                    ///> Operation completed successfully.
		FileNotFound,               ///> Requested file does not exist.
		PermissionDenied,           ///> Access to the file or directory was denied.
		InvalidPath,                ///> Path format is invalid.
		DirectoryCreationFailed,    ///> Failed to create the target directory.
		ReadFailed,                 ///> Failed to read from a file.
		WriteFailed,                ///> Failed to write to a file.
		OutOfMemory,                ///> Memory allocation failed.
		InvalidBasePath,            ///> Base path is invalid or not initialized.
		PlatformFailure,            ///> Platform-specific API returned an error.
	};

	/**
	 * @brief Error trait specialization for file system errors.
	 *
	 * Provides the error category name and conversion of
	 * error codes to human-readable strings.
	 */
	template <>
	struct ErrorTraits<FileSystemErrorCode>
	{
		static constexpr std::string_view Category = "FileSystemError";

		/**
		 * @brief Converts a file system error code to text.
		 *
		 * @param code Error code.
		 * @return Human-readable description.
		 */
		[[nodiscard]] static constexpr std::string_view ToString(FileSystemErrorCode code) noexcept
		{
			switch (code)
			{
				case FileSystemErrorCode::Success:
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
			}

			return "Unknown FileSystem Error";
		}
	};

	static_assert(ErrorTraitsConcept<ErrorTraits<FileSystemErrorCode>, FileSystemErrorCode>);

	using FileSystemError = BasicError<FileSystemErrorCode>;
} // namespace NuEngine::Core
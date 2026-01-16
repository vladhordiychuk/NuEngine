#include <Core/IO/FileSystem.hpp>
#include <fstream>
#include <filesystem>

namespace NuEngine::Core
{
    FileSystem::FileSystem(const std::string& basePath)
        : m_basePath(basePath.empty() ? "." : basePath)
    {
    }

    std::string FileSystem::ResolvePath(const std::string& path) const noexcept
    {
        std::filesystem::path base(m_basePath);
        if (path.rfind("res://", 0) == 0)
        {
            return (base / path.substr(6)).string();
        }
        return path;
    }

    Result<std::vector<char>, FileSystemError> FileSystem::ReadFile(const std::string& path) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        std::string fullPath = ResolvePath(path);
        std::ifstream file(fullPath, std::ios::binary);
        if (!file)
        {
            return Err(FileSystemError(FileSystemErrorCode::FileNotFound));
        }

        file.seekg(0, std::ios::end);
        std::size_t size = static_cast<std::size_t>(file.tellg());
        if (file.fail())
        {
            return Err(FileSystemError(FileSystemErrorCode::ReadFailed));
        }
        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(size);
        file.read(buffer.data(), size);
        if (!file.good())
        {
            return Err(FileSystemError(FileSystemErrorCode::ReadFailed));
        }

        return Ok(std::move(buffer));
    }

    Result<void, FileSystemError> FileSystem::WriteFile(const std::string& path, const std::vector<char>& data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        std::string fullPath = ResolvePath(path);
        std::filesystem::path fsPath(fullPath);

        std::error_code ec;
        if (fsPath.has_parent_path())
        {
            std::filesystem::create_directories(fsPath.parent_path(), ec);
            if (ec) {
                return Err(FileSystemError(FileSystemErrorCode::DirectoryCreationFailed));
            }
        }

        std::ofstream file(fullPath, std::ios::binary);
        if (!file)
        {
            return Err(FileSystemError(FileSystemErrorCode::WriteFailed));
        }

        file.write(data.data(), data.size());
        if (!file.good())
        {
            return Err(FileSystemError(FileSystemErrorCode::WriteFailed));
        }

        return Ok();
    }

    bool FileSystem::FileExists(const std::string& path) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::string fullPath = ResolvePath(path);
        std::error_code ec;
        return std::filesystem::exists(fullPath, ec);
    }

    Result<std::string, FileSystemError> FileSystem::GetAbsolutePath(const std::string& path) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::string fullPath = ResolvePath(path);
        std::error_code ec;
        auto absolutePath = std::filesystem::absolute(fullPath, ec);
        if (ec)
        {
            return Err(FileSystemError(FileSystemErrorCode::InvalidPath));
        }
        return Ok(absolutePath.string());
    }

    std::string FileSystem::GetBasePath() const noexcept
    {
        return m_basePath;
    }
}
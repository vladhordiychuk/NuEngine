#include <Core/IO/FileSystem.hpp>
#include <fstream>
#include <filesystem>

#ifndef NU_ROOT_DIR
    #define NU_ROOT_DIR ""
#endif

namespace NuEngine::Core
{
    FileSystem::FileSystem(const std::string& basePath)
        : m_basePath(basePath.empty() ? "." : basePath)
    {
    }

    std::filesystem::path FileSystem::GetPath(const std::string& path)
    {
        std::string cleanPath = path;
        if (cleanPath.rfind("res://", 0) == 0)
        {
            cleanPath = path.substr(6);
        }

        std::filesystem::path pathObj(cleanPath);

        if (pathObj.is_absolute() && std::filesystem::exists(pathObj))
        {
            return pathObj;
        }

        std::filesystem::path localPath = std::filesystem::current_path() / pathObj;
        if (std::filesystem::exists(localPath))
        {
            return localPath;
        }

        std::string rootDir = NU_ROOT_DIR;
        if (!rootDir.empty())
        {
            std::filesystem::path devPath = std::filesystem::path(rootDir) / pathObj;
            if (std::filesystem::exists(devPath))
            {
                return devPath;
            }
        }
        return localPath;
    }

    std::string FileSystem::ResolvePath(const std::string& path) const noexcept
    {
        return GetPath(path).string();
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
            if (ec) 
            {
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

    Result<std::string, FileSystemError> FileSystem::ReadTextFile(const std::string& path) const
    {
        auto result = ReadFile(path);
        if (result.IsError())
        {
            return Err(result.UnwrapError());
        }

        auto& data = result.Unwrap();
        return Ok(std::string(data.begin(), data.end()));
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
# Logging

The `Logger` is thread-safe and shares the same diagnostic vocabulary as the rest of the module: it reports its own initialization failures through `Result<void, FileSystemError>` rather than treating logging as a special case that can't fail.

```cpp
Result<void, FileSystemError> Logger::InitInternal(LoggerState& state, const std::string& path) noexcept
{
    if (state.initialized)
    {
        return Ok();
    }

    state.logPath = path;

    auto dirRes = CreateLogDirectory(state.logPath);
    if (dirRes.IsError())
    {
        return dirRes;
    }

    state.logFile.open(state.logPath, std::ios::out | std::ios::app);
    if (!state.logFile.is_open())
    {
        return Err(FileSystemError{
            FileSystemErrorCode::WriteFailed,
            "Failed to open log file",
            state.logPath
            });
    }

    state.initialized = true;
    return Ok();
}
```

If the Logger itself can't start — say, it can't open its own log file — the same `BasicError` type used throughout [the error system](ErrorSystem.md) explains why, printed straight to `stderr` since there's nowhere else left to put it at that point. `LOG_FATAL` uses this same pipeline to escalate straight into [`FatalReporter`](Diagnostics.md#fatal-reporter-tier-3) when needed.

> 🚧 Full API documentation coming soon.

---

[← Back to README](../../Core/README.md)
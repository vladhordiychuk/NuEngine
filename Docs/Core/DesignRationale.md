# Design Rationale: How Other Engines Handle This

`Result<T, E>` isn't invented from scratch, and it isn't the only reasonable answer to "how should a game engine report failure." For context:

| Approach | Summary |
|---|---|
| **Exceptions** | Expressive, and can't be silently ignored like a `bool` — but many engines disable them entirely, since stack-unwinding cost is unpredictable and interacts poorly with tight frame budgets and some platform/scripting boundaries. |
| **Unreal Engine** | `check()` / `ensure()` / `verify()` for contract violations, `UE_LOG` for diagnostics, and its own Crash Reporter Client for fatal errors — conceptually close to NuEngine's three-tier split, but built on macros and logging rather than a return-value type. A `Result`-like type exists in parts of the codebase, but most functions still communicate failure through booleans, output parameters, or logging. |
| **Unity (C#)** | Defaults to exceptions almost everywhere, since the language makes that the path of least resistance. There's no standard `Result` type in the base library (a few community packages add one). Garbage collection also changes the cost model for exceptions relative to native C++, so the usual "exceptions are expensive" argument doesn't transfer over directly. |
| **Rust** | Made `Result<T, E>` the default way to handle recoverable errors across its standard library, years before C++ got `std::expected`. NuEngine borrows this shape and adapts it to what a real-time engine needs: a fixed memory budget, compile-time control over diagnostic weight, and integration with the rest of the engine's own diagnostic tools. |

```mermaid
quadrantChart
    title Where NuEngine::Result sits
    x-axis "Ad-hoc / scattered" --> "Systematic"
    y-axis "Opt-in" --> "Language / engine default"
    "Exceptions (native)": [0.35, 0.55]
    "Unreal macros": [0.55, 0.6]
    "Unity exceptions": [0.3, 0.85]
    "Rust Result": [0.85, 0.9]
    "NuEngine Result": [0.8, 0.75]
```

`bool` tells you *that* something failed. `std::expected` tells you *that* something failed, with a value attached. `Result<T, E>` tells you exactly *what*, *where*, and *why*, integrated with the same `Logger` and crash reporter as everything else in the engine. It isn't a replacement for the standard library, and it isn't `std::expected` with extra steps — it's what was needed once `std::expected` wasn't available yet, and once a single wrapper type wasn't enough to describe how errors actually move through a real engine.

---

[← Back to README](../../NuEngine/Core/README.md)
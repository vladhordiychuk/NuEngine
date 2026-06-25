#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <cstdint>
#include <unordered_map>

#include <Weave/WeaveGraphScene.hpp>
#include <NuEngine/Weave/WeaveTypes.hpp>

namespace NuEditor::Weave
{
    using NodeKind = NuEngine::Weave::NodeKind;

    enum class DiagSeverity : uint8_t 
    { 
        Info, 
        Warning, 
        Error 
    };

    enum class CompileStage : uint8_t
    {
        Validate,
        TopoSort,
        ConstFold,
        RegAlloc,
        Emit,
        WriteFile
    };

    using NodeId = int;
    static constexpr NodeId k_NoNode = -1;

    struct CompileDiag
    {
        DiagSeverity Severity = DiagSeverity::Info;
        CompileStage Stage = CompileStage::Validate;
        NodeId Node = k_NoNode;
        std::string Message;
    };

    struct CompileResult
    {
        bool Success = false;
        std::vector<uint8_t> Bytecode;
        std::vector<CompileDiag> Diagnostics;

        [[nodiscard]] bool HasErrors() const noexcept;
        [[nodiscard]] bool HasWarnings() const noexcept;

        void AddError(CompileStage stage, NodeId node, std::string message);
        void AddWarning(CompileStage stage, NodeId node, std::string message);
        void AddInfo(CompileStage stage, NodeId node, std::string message);
    };

    static constexpr uint32_t k_MaxRegisters = NuEngine::Weave::k_RegisterCount;

    struct CompileContext
    {
        std::unordered_map<uint32_t, uint8_t> RegMap;
        uint8_t NextReg = 0;
        std::unordered_map<int, float> FoldedConsts;

        [[nodiscard]] uint32_t CurrentOffset(const CompileResult& r) const noexcept
        {
            return static_cast<uint32_t>(r.Bytecode.size());
        }

        void Reset() noexcept
        {
            RegMap.clear();
            FoldedConsts.clear();
            NextReg = 0;
        }
    };

    class WeaveCompiler
    {
    public:
        [[nodiscard]] CompileResult Compile(const WeaveGraphScene& scene, std::string_view outPath = "");

    private:
        [[nodiscard]] bool Validate(const WeaveGraphScene& scene, CompileResult& result);
        [[nodiscard]] bool TopoSort(const WeaveGraphScene& scene, CompileResult& result, std::vector<int>& outOrder);
        void ConstFold(const std::vector<int>& order, const WeaveGraphScene& scene, CompileContext& ctx);
        void RegAlloc(const std::vector<int>& order, const WeaveGraphScene& scene, CompileContext& ctx);
        void Emit(const std::vector<int>& order, const WeaveGraphScene& scene, CompileContext& ctx, CompileResult& result);
        [[nodiscard]] bool WriteFile(const CompileResult& result, std::string_view path);

        void EmitByte(CompileResult& result, uint8_t  val) noexcept;
        void EmitFloat(CompileResult& result, float    val) noexcept;
        void EmitUInt16(CompileResult& result, uint16_t val) noexcept;
        void EmitUInt32(CompileResult& result, uint32_t val) noexcept;
        void PatchUInt16(CompileResult& result, uint32_t pos, uint16_t val) noexcept;

        [[nodiscard]] uint8_t AllocReg(CompileContext& ctx) noexcept;
        [[nodiscard]] uint8_t GetReg(const CompileContext& ctx, int nodeId, int pinIdx) const noexcept;
        void AssignReg(CompileContext& ctx, int nodeId, int pinIdx, uint8_t reg) noexcept;

        [[nodiscard]] const WeaveConnection* FindInputConnection(const WeaveGraphScene& scene, int nodeId, int pinIdx) const noexcept;
    };
} // namespace NuEditor::Weave
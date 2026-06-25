#include <Weave/WeaveCompiler.hpp>
#include <NuEngine/Weave/WeaveTypes.hpp>

#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <fstream>
#include <cstring>
#include <cassert>
#include <cmath>

#include <cstdint>

static uint32_t Crc32(const uint8_t* data, size_t len) noexcept
{
	uint32_t crc = 0xFFFFFFFF;
	for (size_t i = 0; i < len; ++i)
	{
		crc ^= data[i];
		for (int b = 0; b < 8; ++b)
		{
			crc = (crc >> 1) ^ (0xEDB88320u & -(crc & 1u));
		}
	}
	return ~crc;
}

namespace NuEditor::Weave
{
	bool CompileResult::HasErrors() const noexcept
	{
		for (const auto& d : Diagnostics)
		{
			if (d.Severity == DiagSeverity::Error)
			{
				return true;
			}
		}
		return false;
	}

	bool CompileResult::HasWarnings() const noexcept
	{
		for (const auto& d : Diagnostics)
		{
			if (d.Severity == DiagSeverity::Warning)
			{
				return true;
			}
		}
		return false;
	}

	void CompileResult::AddError(CompileStage stage, NodeId node, std::string message)
	{
		Diagnostics.push_back({ DiagSeverity::Error, stage, node, std::move(message) });
	}

	void CompileResult::AddWarning(CompileStage stage, NodeId node, std::string message)
	{
		Diagnostics.push_back({ DiagSeverity::Warning, stage, node, std::move(message) });
	}

	void CompileResult::AddInfo(CompileStage stage, NodeId node, std::string message)
	{
		Diagnostics.push_back({ DiagSeverity::Info, stage, node, std::move(message) });
	}

	void WeaveCompiler::EmitByte(CompileResult& result, uint8_t val) noexcept
	{
		result.Bytecode.push_back(val);
	}

	void WeaveCompiler::EmitFloat(CompileResult& result, float val) noexcept
	{
		uint8_t buff[4];
		std::memcpy(buff, &val, 4);
		result.Bytecode.insert(result.Bytecode.end(), buff, buff + 4);
	}

	void WeaveCompiler::EmitUInt16(CompileResult& result, uint16_t val) noexcept
	{
		result.Bytecode.push_back(static_cast<uint8_t>(val & 0xFF));
		result.Bytecode.push_back(static_cast<uint8_t>((val >> 8) & 0xFF));
	}

	void WeaveCompiler::EmitUInt32(CompileResult& result, uint32_t val) noexcept
	{
		for (int i = 0; i < 4; ++i)
		{
			result.Bytecode.push_back(static_cast<uint8_t>((val >> (i * 8)) & 0xFF));
		}
	}

	void WeaveCompiler::PatchUInt16(CompileResult& result, uint32_t pos, uint16_t val) noexcept
	{
		assert(pos + 1 < result.Bytecode.size());
		result.Bytecode[pos] = static_cast<uint8_t>(val & 0xFF);
		result.Bytecode[pos + 1] = static_cast<uint8_t>((val >> 8) & 0xFF);
	}

	uint8_t WeaveCompiler::AllocReg(CompileContext& ctx) noexcept
	{
		assert(ctx.NextReg < k_MaxRegisters && "");
		return ctx.NextReg++;
	}

	uint8_t WeaveCompiler::GetReg(const CompileContext& ctx, int nodeId, int pinIdx) const noexcept
	{
		uint32_t key = (static_cast<uint32_t>(nodeId) << 8 | static_cast<uint32_t>(pinIdx));
		auto it = ctx.RegMap.find(key);
		assert(it != ctx.RegMap.end() && "");
		return it->second;
	}

	void WeaveCompiler::AssignReg(CompileContext& ctx, int nodeId, int nodeIdx, uint8_t reg) noexcept
	{
		uint32_t key = (static_cast<uint32_t>(nodeId) << 8 | static_cast<uint32_t>(nodeIdx));
		ctx.RegMap[key] = reg;
	}

	const WeaveConnection* WeaveCompiler::FindInputConnection(const WeaveGraphScene& scene, int nodeId, int pinIdx) const noexcept
	{
		for (const auto& conn : scene.GetConnections())
		{
			if (conn.ToNodeId == nodeId && conn.ToPinIdx == pinIdx)
			{
				return &conn;
			}
		}
		return nullptr;
	}

	bool WeaveCompiler::Validate(const WeaveGraphScene& scene, CompileResult& result)
	{
		const auto& nodes = scene.GetNodes();

		if (nodes.isEmpty())
		{
			result.AddError(CompileStage::Validate, k_NoNode, "Graph is empty - nothing to compile.");
			return false;
		}

		bool hasEntry = false;
		for (const auto& node : nodes)
		{
			if (node.Kind == NodeKind::Event_OnUpdate || node.Kind == NodeKind::Event_OnCollide)
			{
				hasEntry = true;
				break;
			}
		}

		if (!hasEntry)
		{
			result.AddError(CompileStage::Validate, k_NoNode,
				"No entry-point node found. "
				"Add an 'Event OnUpdate' or 'Event OnCollide' node.");
			return false;
		}

		for (const auto& node : nodes)
		{
			if (node.Kind == NodeKind::Unknown)
			{
				result.AddWarning(CompileStage::Validate, node.Id,
					std::string("Unknown node kind for '") +
					node.Title.toStdString() + "' - will be skipped");
			}
		}

		return true;
	}

	bool WeaveCompiler::TopoSort(const WeaveGraphScene& scene, CompileResult& result, std::vector<int>& outOrder)
	{
		const auto& nodes = scene.GetNodes();
		const auto& connections = scene.GetConnections();

		std::unordered_map<int, int> inDegree;
		std::unordered_map<int, std::vector<int>> adjList;

		for (const auto& node : nodes)
		{
			inDegree[node.Id] = 0;
			adjList[node.Id] = {};
		}

		for (const auto& conn : connections)
		{
			adjList[conn.FromNodeId].push_back(conn.ToNodeId);
			inDegree[conn.ToNodeId]++;
		}

		std::queue<int> queue;

		for (const auto& node : nodes)
		{
			if (inDegree[node.Id] == 0)
			{
				queue.push(node.Id);
			}
		}

		int processed = 0;

		while (!queue.empty())
		{
			int id = queue.front();
			queue.pop();
			outOrder.push_back(id);
			++processed;

			for (int next : adjList[id])
			{
				if (--inDegree[next] == 0)
				{
					queue.push(next);
				}
			}
		}

		if (processed != nodes.size())
		{
			result.AddError(CompileStage::TopoSort, k_NoNode, "Graph contains a cycle — cyclic graphs are not supported.");
			return false;
		}

		return true;
	}

	void WeaveCompiler::ConstFold(const std::vector<int>& order, const WeaveGraphScene& scene, CompileContext& ctx)
	{
		std::unordered_map<int, float> knownConsts;

		auto nodeById = [&](int id) -> const WeaveNode* {
			for (const auto& node : scene.GetNodes())
			{
				if (node.Id == id)
				{
					return &node;
				}
			}
			return nullptr;
			};

		for (int id : order)
		{
			const WeaveNode* node = nodeById(id);

			if (!node)
			{
				continue;
			}

			const NodeKind kind = node->Kind;

			// ── ПАТЧ: читаємо PinDefaultValues[0] замість Subtitle ──────────
			if (kind == NodeKind::Const_Float)
			{
				float val = node->GetDefaultFloat(0);
				knownConsts[id] = val;
				ctx.FoldedConsts[id] = val;
				continue;
			}

			if (kind == NodeKind::Math_Add || kind == NodeKind::Math_Sub ||
				kind == NodeKind::Math_Mul || kind == NodeKind::Math_Div)
			{
				const auto* connA = FindInputConnection(scene, id, 0);
				const auto* connB = FindInputConnection(scene, id, 1);

				if (!connA || !connB)
				{
					continue;
				}

				auto itA = knownConsts.find(connA->FromNodeId);
				auto itB = knownConsts.find(connB->FromNodeId);

				if (itA == knownConsts.end() || itB == knownConsts.end())
				{
					continue;
				}

				float a = itA->second;
				float b = itB->second;
				float res = 0.0f;

				switch (kind)
				{
				case NodeKind::Math_Add:
					res = a + b;
					break;
				case NodeKind::Math_Sub:
					res = a - b;
					break;
				case NodeKind::Math_Mul:
					res = a * b;
					break;
				case NodeKind::Math_Div:
					res = (b == 0.0f) ? 0.0f : a / b;
					break;
				default:
					break;
				}

				knownConsts[id] = res;
				ctx.FoldedConsts[id] = res;
			}
		}
	}

	void WeaveCompiler::RegAlloc(const std::vector<int>& order, const WeaveGraphScene& scene, CompileContext& ctx)
	{
		auto nodeById = [&](int id) -> const WeaveNode* {
			for (const auto& node : scene.GetNodes())
			{
				if (node.Id == id)
				{
					return &node;
				}
			}
			return nullptr;
			};

		for (auto id : order)
		{
			const WeaveNode* node = nodeById(id);

			if (!node)
			{
				continue;
			}

			for (int i = 0; i < node->Pins.size(); ++i)
			{
				if (node->Pins[i].IsOutput)
				{
					AssignReg(ctx, id, i, AllocReg(ctx));
				}
			}
		}
	}

	void WeaveCompiler::Emit(const std::vector<int>& order, const WeaveGraphScene& scene, CompileContext& ctx, CompileResult& result)
	{
		using OC = NuEngine::Weave::OpCode;
		namespace NF = NuEngine::Weave::NativeFuncId;

		auto nodeById = [&](int id) -> const WeaveNode* {
			for (const auto& node : scene.GetNodes())
			{
				if (node.Id == id)
				{
					return &node;
				}
			}
			return nullptr;
			};

		// ── ПАТЧ: незв'язаний пін → LOAD_CONST_F з PinDefaultValues ────────
		// ВАЖЛИВО: ця lambda може емітувати байти, тому НЕ викликати inline
		// всередині EmitByte(). Завжди зберігати результат у локальну змінну.
		auto inputReg = [&](int toNodeId, int toPinIdx) -> uint8_t {
			const auto* conn = FindInputConnection(scene, toNodeId, toPinIdx);
			if (conn)
			{
				return GetReg(ctx, conn->FromNodeId, conn->FromPinIdx);
			}

			// Пін не підключений — генеруємо LOAD_CONST_F з PinDefaultValues
			const WeaveNode* node = nodeById(toNodeId);
			float defVal = node ? node->GetDefaultFloat(toPinIdx) : 0.0f;
			uint8_t tempReg = AllocReg(ctx);
			EmitByte(result, static_cast<uint8_t>(OC::LOAD_CONST_F));
			EmitByte(result, tempReg);
			EmitFloat(result, defVal);
			return tempReg;
			};

		for (int id : order)
		{
			const WeaveNode* node = nodeById(id);

			if (!node)
			{
				continue;
			}

			const NodeKind kind = node->Kind;

			if (kind == NodeKind::Event_OnUpdate || kind == NodeKind::Event_OnCollide)
			{
				continue;
			}

			if (ctx.FoldedConsts.count(id))
			{
				for (int i = 0; i < node->Pins.size(); ++i)
				{
					if (node->Pins[i].IsOutput)
					{
						EmitByte(result, static_cast<uint8_t>(OC::LOAD_CONST_F));
						EmitByte(result, GetReg(ctx, id, i));
						EmitFloat(result, ctx.FoldedConsts.at(id));
					}
				}
				continue;
			}

			{
				OC mathOp = OC::HALT;
				switch (kind)
				{
				case NodeKind::Math_Add:
					mathOp = OC::ADD_F;
					break;
				case NodeKind::Math_Sub:
					mathOp = OC::SUB_F;
					break;
				case NodeKind::Math_Mul:
					mathOp = OC::MUL_F;
					break;
				case NodeKind::Math_Div:
					mathOp = OC::DIV_F;
					break;
				default:
					break;
				}

				if (mathOp != OC::HALT)
				{
					// Зберігаємо регістри до EmitByte, бо inputReg може емітувати байти
					uint8_t regA = inputReg(id, 0);
					uint8_t regB = inputReg(id, 1);
					uint8_t regOut = GetReg(ctx, id, 2);
					EmitByte(result, static_cast<uint8_t>(mathOp));
					EmitByte(result, regA);
					EmitByte(result, regB);
					EmitByte(result, regOut);
					continue;
				}
			}

			if (kind == NodeKind::Math_Neg)
			{
				uint8_t regIn = inputReg(id, 0);
				uint8_t regOut = GetReg(ctx, id, 1);
				EmitByte(result, static_cast<uint8_t>(OC::NEG_F));
				EmitByte(result, regIn);
				EmitByte(result, regOut);
				continue;
			}

			{
				OC cmpOp = OC::HALT;
				switch (kind)
				{
				case NodeKind::Cmp_EQ:
					cmpOp = OC::CMP_EQ_F;
					break;
				case NodeKind::Cmp_LT:
					cmpOp = OC::CMP_LT_F;
					break;
				case NodeKind::Cmp_GT:
					cmpOp = OC::CMP_GT_F;
					break;
				case NodeKind::Cmp_LE:
					cmpOp = OC::CMP_LE_F;
					break;
				case NodeKind::Cmp_GE:
					cmpOp = OC::CMP_GE_F;
					break;
				default:
					break;
				}

				if (cmpOp != OC::HALT)
				{
					uint8_t regA = inputReg(id, 0);
					uint8_t regB = inputReg(id, 1);
					uint8_t regOut = GetReg(ctx, id, 2);
					EmitByte(result, static_cast<uint8_t>(cmpOp));
					EmitByte(result, regA);
					EmitByte(result, regB);
					EmitByte(result, regOut);
					continue;
				}
			}

			if (kind == NodeKind::Unknown)
			{
				result.AddWarning(CompileStage::Emit, id,
					std::string("Node '") + node->Title.toStdString() +
					"' has no emitter — skipped.");
			}

			if (kind == NodeKind::Native_SetVelZ)
			{
				uint8_t regIn = inputReg(id, 1);
				EmitByte(result, static_cast<uint8_t>(OC::CALL_EXTERNAL));
				EmitUInt32(result, NF::SetVelocityZ);
				EmitByte(result, 1);
				EmitByte(result, regIn);
				continue;
			}

			if (kind == NodeKind::Native_GetDeltaTime)
			{
				EmitByte(result, static_cast<uint8_t>(OC::CALL_EXTERNAL));
				EmitUInt32(result, NF::GetDeltaTime);
				EmitByte(result, 0);
				EmitByte(result, GetReg(ctx, id, 1));
				continue;
			}
		}
	}

	bool WeaveCompiler::WriteFile(const CompileResult& result, std::string_view path)
	{
		std::ofstream f(std::string(path), std::ios::binary | std::ios::trunc);
		if (!f.is_open())
		{
			return false;
		}

		NuEngine::Weave::WbcFileHeader header{};
		header.Magic = NuEngine::Weave::k_Magic;
		header.Version = NuEngine::Weave::k_BytecodeVersion;
		header.Reserved = 0;
		header.BytecodeSize = static_cast<uint32_t>(result.Bytecode.size());
		header.Checksum = Crc32(result.Bytecode.data(), result.Bytecode.size());

		f.write(reinterpret_cast<const char*>(&header), sizeof(header));
		f.write(reinterpret_cast<const char*>(result.Bytecode.data()), static_cast<std::streamsize>(result.Bytecode.size()));
		return f.good();
	}

	CompileResult WeaveCompiler::Compile(const WeaveGraphScene& scene, std::string_view outPath)
	{
		CompileResult result;
		CompileContext ctx;

		if (!Validate(scene, result))
		{
			return result;
		}

		std::vector<int> order;
		if (!TopoSort(scene, result, order))
		{
			return result;
		}

		ConstFold(order, scene, ctx);

		RegAlloc(order, scene, ctx);

		Emit(order, scene, ctx, result);

		if (result.HasErrors())
		{
			return result;
		}

		result.Success = true;

		if (!outPath.empty())
		{
			if (!WriteFile(result, outPath))
			{
				result.AddWarning(CompileStage::WriteFile, k_NoNode,
					"Bytecode generated but failed to write: " + std::string(outPath));
			}
			else
			{
				result.AddInfo(CompileStage::WriteFile, k_NoNode,
					"Written: " + std::string(outPath) +
					" (" + std::to_string(result.Bytecode.size()) + " bytes)");
			}
		}

		return result;
	}
} // namespace NuEditor::Weave
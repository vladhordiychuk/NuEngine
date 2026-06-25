#pragma once

#include <cstdint>

namespace NuEngine::Weave
{
	inline constexpr uint16_t k_BytecodeVersion = 1;

	inline constexpr uint32_t k_Magic = 0x57454156; // 'W','E','A','V'

	enum class OpCode : uint8_t
	{
		HALT = 0x00,
		JUMP = 0x01,
		JUMP_IF_FALSE = 0x02,
		JUMP_IF_TRUE  = 0x03,

		LOAD_CONST_F = 0x10,
		LOAD_CONST_I = 0x11,
		LOAD_ZERO    = 0x12,
		MOV          = 0x13,

		ADD_F = 0x20,
		SUB_F = 0x21,
		MUL_F = 0x22,
		DIV_F = 0x23,
		MOD_F = 0x24,
		NEG_F = 0x25,

		ADD_I = 0x28,
		SUB_I = 0x29,
		MUL_I = 0x2A,
		DIV_I = 0x2B,
		MOD_I = 0x2C,
		NEG_I = 0x2D,

		AND = 0x30,
		OR  = 0x31,
		NOT = 0x32,

		CMP_EQ_F = 0x35,
		CMP_LT_F = 0x36,
		CMP_GT_F = 0x37,
		CMP_LE_F = 0x38,
		CMP_GE_F = 0x39,

		CMP_EQ_I = 0x3A,
		CMP_LT_I = 0x3B,
		CMP_GT_I = 0x3C,
		CMP_LE_I = 0x3D,
		CMP_GE_I = 0x3E,

		CAST_I2F = 0x40,
		CAST_F2I = 0x41,

		SIN_F = 0x45,
		COS_F = 0x46,

		CALL_EXTERNAL   = 0x60,
		READ_COMPONENT  = 0x61,
		WRITE_COMPONENT = 0x62
	};

	union WeaveRegister
	{
		float f;
		int32_t i;
		uint32_t u;

		WeaveRegister() : u(0) {}
		explicit WeaveRegister(float v) : f(v) {}
		explicit WeaveRegister(int32_t v) : i(v) {}
		explicit WeaveRegister(uint32_t v) : u(v) {}
	};

	static_assert(sizeof(WeaveRegister) == 4, "WeaveRegister must be exactly 4 bytes");

	inline constexpr uint8_t k_RegisterCount = 16;

	#pragma pack(push, 1)
	struct WbcFileHeader
	{
		uint32_t Magic;
		uint16_t Version;
		uint16_t Reserved;
		uint32_t BytecodeSize;
		uint32_t Checksum;
	};
	#pragma pack(pop)

	static_assert(sizeof(WbcFileHeader) == 16, "WbcFileHeader must be exactly 16 bytes");

	namespace NativeFuncId
	{
		inline constexpr uint32_t GetDeltaTime = 0;
		inline constexpr uint32_t GetEntityPosX = 1;
		inline constexpr uint32_t GetEntityPosY = 2;
		inline constexpr uint32_t GetEntityPosZ = 3;
		inline constexpr uint32_t SetVelocityX = 4;
		inline constexpr uint32_t SetVelocityY = 5;
		inline constexpr uint32_t SetVelocityZ = 6;
		inline constexpr uint32_t FindPlayer = 7;
		inline constexpr uint32_t DistanceTo = 8;
		inline constexpr uint32_t SpawnEffect = 9;



		inline constexpr uint32_t k_Count = 10;
	}

	enum class NodeKind : uint8_t
	{
		Unknown = 0,

		Event_OnUpdate = 1,
		Event_OnCollide = 2,

		Const_Float = 10,
		Const_Int = 11,

		Math_Add = 20,
		Math_Sub = 21,
		Math_Mul = 22,
		Math_Div = 23,
		Math_Neg = 24,

		Cmp_EQ = 30,
		Cmp_LT = 31,
		Cmp_GT = 32,
		Cmp_LE = 33,
		Cmp_GE = 34,

		Flow_Branch = 40,

		Native_GetDeltaTime = 50,
		Native_GetPosX = 51,
		Native_GetPosY = 52,
		Native_GetPosZ = 53,
		Native_SetVelX = 54,
		Native_SetVelY = 55,
		Native_SetVelZ = 56,
		Native_FindPlayer = 57,
		Native_DistanceTo = 58,
	};
} // namespace NuEngine::Weave
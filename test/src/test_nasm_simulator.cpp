#include <gtest/gtest.h>
#include "decoder.h"
#include "instruction.hpp"
#include "simulator.hpp"

TEST(SimulatorTest, test_listing_0043)
{
	std::vector<Instruction> instructions = decode_asm_file("res/listing_0043_immediate_movs");
	struct Memory memory = simulate(instructions);

	EXPECT_EQ(memory.registers[Register::Ax], 1);
	EXPECT_EQ(memory.registers[Register::Bx], 2);
	EXPECT_EQ(memory.registers[Register::Cx], 3);
	EXPECT_EQ(memory.registers[Register::Dx], 4);
	EXPECT_EQ(memory.registers[Register::Sp], 5);
	EXPECT_EQ(memory.registers[Register::Bp], 6);
	EXPECT_EQ(memory.registers[Register::Si], 7);
	EXPECT_EQ(memory.registers[Register::Di], 8);
}

TEST(SimulatorTest, test_listing_0044)
{
	std::vector<Instruction> instructions = decode_asm_file("res/listing_0044_register_movs_execution");
	struct Memory memory = simulate(instructions);

	EXPECT_EQ(memory.registers[Register::Ax], 4);
	EXPECT_EQ(memory.registers[Register::Bx], 3);
	EXPECT_EQ(memory.registers[Register::Cx], 2);
	EXPECT_EQ(memory.registers[Register::Dx], 1);
	EXPECT_EQ(memory.registers[Register::Sp], 1);
	EXPECT_EQ(memory.registers[Register::Bp], 2);
	EXPECT_EQ(memory.registers[Register::Si], 3);
	EXPECT_EQ(memory.registers[Register::Di], 4);
}

TEST(SimulatorTest, test_listing_0045)
{
	std::vector<Instruction> instructions = decode_asm_file("res/listing_0045_challenge_register_movs");
	struct Memory memory = simulate(instructions);

	EXPECT_EQ(memory.registers[Register::Ax], 17425);
	EXPECT_EQ(memory.registers[Register::Bx], 13124);
	EXPECT_EQ(memory.registers[Register::Cx], 26231);
	EXPECT_EQ(memory.registers[Register::Dx], 30600);
	EXPECT_EQ(memory.registers[Register::Sp], 17425);
	EXPECT_EQ(memory.registers[Register::Bp], 13124);
	EXPECT_EQ(memory.registers[Register::Si], 26231);
	EXPECT_EQ(memory.registers[Register::Di], 30600);
	EXPECT_EQ(memory.registers[Register::Es], 26231);
	EXPECT_EQ(memory.registers[Register::Ss], 17425);
	EXPECT_EQ(memory.registers[Register::Ds], 13124);
}

TEST(SimulatorTest, test_listing_0046)
{
	std::vector<Instruction> instructions = decode_asm_file("res/listing_0046_add_sub_cmp_execution");
	struct Memory memory = simulate(instructions);
	EXPECT_EQ(memory.registers[Register::Bx], 57602);
	EXPECT_EQ(memory.registers[Register::Cx], 3841);
	EXPECT_EQ(memory.registers[Register::Sp], 998);
	EXPECT_EQ(memory.s_flag, false);
	EXPECT_EQ(memory.z_flag, true);

}

TEST(SimulatorTest, test_listing_0048)
{
	std::vector<Instruction> instructions = decode_asm_file("res/listing_0048_ip_register");
	struct Memory memory = simulate(instructions);
	EXPECT_EQ(memory.registers[Register::Bx], 2000);
	EXPECT_EQ(memory.registers[Register::Cx], 64736);
	EXPECT_EQ(memory.s_flag, true);
	EXPECT_EQ(memory.instruction_pointer, 14);

}

TEST(SimulatorTest, test_listing_0049)
{
	std::vector<Instruction> instructions = decode_asm_file("res/listing_0049_conditional_jumps");
	struct Memory memory = simulate(instructions);
	EXPECT_EQ(memory.registers[Register::Bx], 1030);
	EXPECT_EQ(memory.instruction_pointer, 14);
}

TEST(SimulatorTest, test_listing_0051)
{
	std::vector<Instruction> instructions = decode_asm_file("res/listing_0051_memory_mov");
	struct Memory memory = simulate(instructions);
	EXPECT_EQ(memory.registers[Register::Bx], 1);
	EXPECT_EQ(memory.registers[Register::Cx], 2);
	EXPECT_EQ(memory.registers[Register::Dx], 10);
	EXPECT_EQ(memory.registers[Register::Bp], 4);
	EXPECT_EQ(memory.instruction_pointer, 48);
}

TEST(SimulatorTest, test_listing_0052)
{
	std::vector<Instruction> instructions = decode_asm_file("res/listing_0052_memory_add_loop");
	struct Memory memory = simulate(instructions);
	EXPECT_EQ(memory.registers[Register::Bx], 6);
	EXPECT_EQ(memory.registers[Register::Cx], 4);
	EXPECT_EQ(memory.registers[Register::Dx], 6);
	EXPECT_EQ(memory.registers[Register::Bp], 1000);
	EXPECT_EQ(memory.registers[Register::Si], 6);
	EXPECT_EQ(memory.instruction_pointer, 35);
	EXPECT_EQ(memory.z_flag, true);
}

TEST(SimulatorTest, test_listing_0053)
{
	std::vector<Instruction> instructions = decode_asm_file("res/listing_0053_add_loop_challenge");
	struct Memory memory = simulate(instructions);
	EXPECT_EQ(memory.registers[Register::Bx], 6);
	EXPECT_EQ(memory.registers[Register::Dx], 6);
	EXPECT_EQ(memory.registers[Register::Bp], 998);
	EXPECT_EQ(memory.instruction_pointer, 33);
	EXPECT_EQ(memory.z_flag, true);
}

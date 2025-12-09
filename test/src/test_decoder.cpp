#include <gtest/gtest.h>
#include <unistd.h>
#include "../src/decoder.h"
    #include <unistd.h>
#include <limits.h>

void compare_instructions(Instruction received, Instruction expected)
{
	ASSERT_EQ(received.to_string(), expected.to_string());
}

TEST(OtherTest, decoder_test_single_mov)
{
	std::vector<Instruction> decoded_instructions = decode_asm_file("res/listing_0037_single_register_mov");
	ASSERT_EQ(decoded_instructions.size(), 1) << "Only one instruction expected";

	Instruction decoded = decoded_instructions.at(0);

	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov cx, bx" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.operands[0].reg, "cx");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].reg, "bx");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Register);
}

TEST(OtherTest, decoder_test_many_mov)
{
  std::vector<Instruction> decoded_instructions = decode_asm_file("res/listing_0038_many_register_mov");
	ASSERT_EQ(decoded_instructions.size(), 11); 

	Instruction decoded;
	uint8_t instruction_count = 0;

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov cx, bx" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "cx");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].reg, "bx");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Register);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov ch, ah" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "ch");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].reg, "ah");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Register);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov dx, bx" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "dx");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].reg, "bx");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Register);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov si, bx" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "si");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].reg, "bx");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Register);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov bx, di" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "bx");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].reg, "di");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Register);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov al, cl" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "al");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].reg, "cl");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Register);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov ch, ch" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "ch");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].reg, "ch");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Register);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov bx, ax" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "bx");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].reg, "ax");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Register);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov bx, si" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "bx");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].reg, "si");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Register);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov sp, di" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "sp");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].reg, "di");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Register);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov bp, ax" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "bp");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].reg, "ax");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Register);
}

TEST(OtherTest, decoder_test_more_movs)
{
  std::vector<Instruction> decoded_instructions = decode_asm_file("res/listing_0039_more_movs");
	Instruction decoded;
	uint8_t instruction_count = 0;

	// Register-to-register
	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov si, bx" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "si");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].reg, "bx");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Register);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov dh, al" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "dh");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].reg, "al");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Register);

	// 8-bit immediate-to-register
	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov cl, 12" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "cl");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].immediate, 12);
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Immediate);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov ch, -12" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "ch");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].immediate, -12);
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Immediate);

	// 16-bit immediate-to-register
	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov cx, 12" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "cx");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].immediate, 12);
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Immediate);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov cx, -12" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "cx");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].immediate, -12);
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Immediate);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov dx, 3948" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "dx");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].immediate, 3948);
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Immediate);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov dx, -3948" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "dx");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].immediate, -3948);
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Immediate);

	// Source address calculation
	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov al, [bx + si]" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "al");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].address, "bx + si");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Memory);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov bx, [bp + di]" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "bx");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].address, "bp + di");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Memory);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov dx, [bp]" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "dx");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].address, "bp");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Memory);

	// Source address calculation plus 8-bit displacement
	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov ah, [bx + si + 4]" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "ah");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].address, "bx + si");
	ASSERT_EQ(decoded.operands[1].displacement, 4);
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Memory);

	// Source address calculation plus 16-bit displacement
	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov al, [bx + si + 4999]" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "al");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].address, "bx + si");
	ASSERT_EQ(decoded.operands[1].displacement, 4999);
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Memory);

	// Dest address calculation
	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov [bx + di], cx" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].address, "bx + di");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Memory);
	ASSERT_EQ(decoded.operands[1].reg, "cx");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Register);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov [bp + si], cl" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].address, "bp + si");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Memory);
	ASSERT_EQ(decoded.operands[1].reg, "cl");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Register);

	// mov [bp], ch
	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov [bp], ch" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].address, "bp");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Memory);
	ASSERT_EQ(decoded.operands[1].reg, "ch");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Register);
}

TEST(OtherTest, decoder_test_challenge_movs)
{
  std::vector<Instruction> decoded_instructions = decode_asm_file("res/listing_0040_challenge_movs");
	Instruction decoded;
	uint8_t instruction_count = 0;

	// Signed displacements
	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov ax, [bx + di - 37]" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "ax");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].address, "bx + di");
	ASSERT_EQ(decoded.operands[1].displacement, -37);
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Memory);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov [si - 300], cx" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].address, "si");
	ASSERT_EQ(decoded.operands[0].displacement, -300);
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Memory);
	ASSERT_EQ(decoded.operands[1].reg, "cx");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Register);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov dx, [bx - 32]" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "dx");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].address, "bx");
	ASSERT_EQ(decoded.operands[1].displacement, -32);
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Memory);

	// Explicit sizes
	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov [bp + di], byte 7" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].address, "bp + di");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Memory);
	ASSERT_EQ(decoded.operands[1].immediate, 7);
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Immediate);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov [di + 901], word 347" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].address, "di");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Memory);
	ASSERT_EQ(decoded.operands[0].displacement, 901);
	ASSERT_EQ(decoded.operands[1].immediate, 347);
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Immediate);

	// Direct address
	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov bp, [5]" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "bp");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].displacement, 5);
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Direct_address);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov bx, [3458]" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "bx");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].displacement, 3458);
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Direct_address);

	// Memory-to-accumulator test
	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov ax, [2555]" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "ax");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].displacement, 2555);
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Direct_address);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov ax, [16]" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].reg, "ax");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].displacement, 16);
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Direct_address);

	// Accumulator-to-memory test
	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov [2554], ax" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].displacement, 2554);
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Direct_address);
	ASSERT_EQ(decoded.operands[1].reg, "ax");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Register);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: mov [15], ax" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "mov");
	ASSERT_EQ(decoded.operands[0].displacement, 15);
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Direct_address);
	ASSERT_EQ(decoded.operands[1].reg, "ax");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Register);
}

/*
TEST(OtherTest, decoder_test_add_sub_cmp_jnz)
{
    test_decoder("res/listing_0041_add_sub_cmp_jnz");
}
*/

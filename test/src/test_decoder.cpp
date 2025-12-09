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

TEST(OtherTest, decoder_test_add_sub_cmp_jnz)
{
  //std::vector<Instruction> decoded_instructions = decode_asm_file("res/listing_0041_add_sub_cmp_jnz");
  std::vector<Instruction> decoded_instructions = decode_asm_file("res/add");
	Instruction decoded;
	uint8_t instruction_count = 0;

	// Add
	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: add bx, [bx + si]" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "add");
	ASSERT_EQ(decoded.operands[0].reg, "bx");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].address, "bx + si");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Memory);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: add bx, [bp]" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "add");
	ASSERT_EQ(decoded.operands[0].reg, "bx");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].address, "bp");
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Memory);

	decoded = decoded_instructions.at(instruction_count++);
	std::cout << "\n=====================================================" << std::endl;
	std::cout << "Original: add si, 2" << std::endl;
	decoded.print_debug();
	ASSERT_EQ(decoded.name, "add");
	ASSERT_EQ(decoded.operands[0].reg, "si");
	ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	ASSERT_EQ(decoded.operands[1].immediate, 2);
	ASSERT_EQ(decoded.operands[1].type, Operand_type::Immediate);

	//decoded = decoded_instructions.at(instruction_count++);
	//std::cout << "\n=====================================================" << std::endl;
	//std::cout << "Original: add bp, 2" << std::endl;
	//decoded.print_debug();
	//ASSERT_EQ(decoded.name, "add");
	//ASSERT_EQ(decoded.operands[0].reg, "bp");
	//ASSERT_EQ(decoded.operands[0].type, Operand_type::Register);
	//ASSERT_EQ(decoded.operands[1].immediate, 2);
	//ASSERT_EQ(decoded.operands[1].type, Operand_type::Immediate);
	// add cx, 8
	// add bx, [bp + 0]
	// add cx, [bx + 2]
	// add bh, [bp + si + 4]
	// add di, [bp + di + 6]
	// add [bx+si], bx
	// add [bp], bx
	// add [bp + 0], bx
	// add [bx + 2], cx
	// add [bp + si + 4], bh
	// add [bp + di + 6], di
	// add byte [bx], 34
	// add word [bp + si + 1000], 29
	// add ax, [bp]
	// add al, [bx + si]
	// add ax, bx
	// add al, ah
	// add ax, 1000
	// add al, -30
	// add al, 9
	// 
	// sub bx, [bx+si]
	// sub bx, [bp]
	// sub si, 2
	// sub bp, 2
	// sub cx, 8
	// sub bx, [bp + 0]
	// sub cx, [bx + 2]
	// sub bh, [bp + si + 4]
	// sub di, [bp + di + 6]
	// sub [bx+si], bx
	// sub [bp], bx
	// sub [bp + 0], bx
	// sub [bx + 2], cx
	// sub [bp + si + 4], bh
	// sub [bp + di + 6], di
	// sub byte [bx], 34
	// sub word [bx + di], 29
	// sub ax, [bp]
	// sub al, [bx + si]
	// sub ax, bx
	// sub al, ah
	// sub ax, 1000
	// sub al, -30
	// sub al, 9
	// 
	// cmp bx, [bx+si]
	// cmp bx, [bp]
	// cmp si, 2
	// cmp bp, 2
	// cmp cx, 8
	// cmp bx, [bp + 0]
	// cmp cx, [bx + 2]
	// cmp bh, [bp + si + 4]
	// cmp di, [bp + di + 6]
	// cmp [bx+si], bx
	// cmp [bp], bx
	// cmp [bp + 0], bx
	// cmp [bx + 2], cx
	// cmp [bp + si + 4], bh
	// cmp [bp + di + 6], di
	// cmp byte [bx], 34
	// cmp word [4834], 29
	// cmp ax, [bp]
	// cmp al, [bx + si]
	// cmp ax, bx
	// cmp al, ah
	// cmp ax, 1000
	// cmp al, -30
	// cmp al, 9
	// 
	// test_label0:
	// jnz test_label1
	// jnz test_label0
	// test_label1:
	// jnz test_label0
	// jnz test_label1
	// 
	// label:
	// je label
	// jl label
	// jle label
	// jb label
	// jbe label
	// jp label
	// jo label
	// js label
	// jne label
	// jnl label
	// jg label
	// jnb label
	// ja label
	// jnp label
	// jno label
	// jns label
	// loop label
	// loopz label
	// loopnz label
	// jcxz label
}

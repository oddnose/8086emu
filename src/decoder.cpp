#include "decoder.h"
#include <csetjmp>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "instruction-table.hpp"

#define array_count(array) (sizeof(array) / sizeof((array)[0]))

const std::string reg_names_8bit[] = { 
  "al", 
  "cl", 
  "dl", 
  "bl",
  "ah", 
  "ch", 
  "dh", 
  "bh"
};
const std::string reg_names_16bit[] = { 
  "ax", // ah and al
  "cx", // ch and cl
  "dx", // dh and dl
  "bx", // bh and bl
  "sp", // stack pointer
  "bp", // base pointer
  "si", // source index
  "di" // des index
};

const std::string rm_field_encodings[] = {
  "bx + si",
  "bx + di",
  "bp + si",
  "bp + di",
  "si",
  "di",
  "bp", //not used when mod 00
  "bx"
}; 

/*
enum Instruction_name {
  Mov,
  Add,
  Sub,
  Cmp,
  Jnz,
  Je,
  Jl,
  Jle,
  Jb,
  Jbe,
  Jp,
  Jo,
  Js,
  Jne,
  Jnl,
  Jg,
  Jnb,
  Ja,
  Jnp,
  Jno,
  Jns,
  loop,
  loopz,
  loopnz,
  jcxz
};
std::map<Instruction_name, std::string> instruction_name_map = {
  {Mov, "mov"},
  {Add, "add"},
  {Sub, "sub"},
  {Cmp, "cmp"},
  {Jnz, "jnz"},
  {Je, "je"},
  {Jl, "jl"},
  {Jle, "jle"},
  {Jb, "jb"},
  {Jbe, "jbe"},
  {Jp, "jp"},
  {Jo, "jo"},
  {Js, "js"},
  {Jne, "jne"},
  {Jnl, "jnl"},
  {Jg, "jg"},
  {Jnb, "jnb"},
  {Ja, "ja"},
  {Jnp, "jnp"},
  {Jno, "jno"},
  {Jns, "jns"},
  {loop, "loop"},
  {loopz, "loopz"},
  {loopnz, "loopnz"},
  {jcxz, "jcxz"},
};
*/

/*

int16_t read_data(std::vector<unsigned char>& data, size_t& offset, bool word, bool is_signed)
{
	int16_t value;
	if (is_signed) {
		value = static_cast<int8_t>(data[offset]);
		offset += 1;
	}
	else {
		value = word ? 
			static_cast<int16_t>(data[offset]) | (static_cast<int16_t>(data[offset + 1]) << 8) :
			static_cast<int8_t>(data[offset]);

		offset += (word ? 2 : 1);
	}
	return value;
}

Operand set_rm_text(std::vector<unsigned char>& data, size_t& offset, char mod, char rm, bool word, bool is_signed)
{
	Operand rm_operand;

	switch (mod) {
		case 0b00: {// Memory Mode (no displacement, except when r/m = 110)
			if (rm == 0b110) { //direct address
				int16_t value = read_data(data, offset, true, false);

				rm_operand.type = Operand_type::Direct_address;
				rm_operand.value = value;
				rm_operand.size = (word ? Operand_size::Word : Operand_size::Byte);
			}
			else {
				rm_operand.type = Operand_type::Memory;
				rm_operand.value = rm;
				rm_operand.size = (word ? Operand_size::Word : Operand_size::Byte);
			}
			break;
		}
		case 0b01: // Memory Mode, 8-bit displacement follows
		case 0b10: {// Memory Mode, 16-bit displacement follows
			int16_t displacement = read_data(data, offset, (mod == 0b10), false);
			rm_operand.type = Operand_type::Memory;
			rm_operand.value = rm;
			rm_operand.displacement = displacement;
			rm_operand.size = (mod == 0b10 ? Operand_size::Word : Operand_size::Byte);
			break;
		}
		case 0b11: // Register Mode (no displacement)
			rm_operand.type = Operand_type::Register;
			rm_operand.value = rm;
			rm_operand.size = (word ? Operand_size::Word : Operand_size::Byte);
			break;
	}
	return rm_operand;
}

Instruction reg_or_mem_with_reg_to_either(Instruction_name name, std::vector<unsigned char>& data, size_t& offset)
{
	int offset_start = offset;
	std::stringstream output;
	bool direction = (data[offset] & 0b10) >> 1; // 0 = reg field is source for mov, 1 = reg field is destination
	bool word = data[offset] & 0b1; // 0 = move 1 byte, 1 = move 2 bytes
	offset++;

	char mod = data[offset] >> 6;
	char reg = (data[offset] & 0b111000) >> 3;
	char rm = data[offset] & 0b111;
	offset++;

	Operand rm_operand = set_rm_text(data, offset, mod, rm, word, false);
	Operand reg_operand = {.type = Operand_type::Register, .size = (word ? Operand_size::Word : Operand_size::Byte), .value = reg };

	Instruction instruction;
	instruction.name = instruction_name_map[name];
	instruction.operands[0] = (direction ? reg_operand : rm_operand);
	instruction.operands[1] = (direction ? rm_operand : reg_operand);
	instruction.size = offset - offset_start;

	return instruction;
}

Instruction immediate_to_register(Instruction_name name, std::vector<unsigned char>& data, size_t& offset)
{
	int offset_start = offset;
	bool word = (data[offset] & 0b1000) >> 3; // 0 = move 1 byte, 1 = move 2 bytes
	char reg = data[offset] & 0b111;
	offset++;

	int16_t value = read_data(data, offset, word, false);

	Instruction instruction;
	instruction.name = instruction_name_map[name];
	instruction.operands[0] = {.type = Operand_type::Register, .size = (word ? Operand_size::Word : Operand_size::Byte), .value = reg};
	instruction.operands[1] = {.type = Operand_type::Immediate, .value = value};
	instruction.size = offset - offset_start;

	return instruction;
}

Instruction immediate_to_reg_mem(Instruction_name name, std::vector<unsigned char>& data, size_t& offset) 
{
	Instruction instruction;

	int offset_start = offset;
	bool word = data[offset] & 0b1; // 0 = move 1 byte, 1 = move 2 bytes

	bool sign_extension = false;
	if (name != Mov) {
		sign_extension = (data[offset] & 0b10) >> 1; 
	}
	offset++;

	char mod = data[offset] >> 6;
	char rm = data[offset] & 0b111;
	offset++;

	Operand rm_operand = set_rm_text(data, offset, mod, rm, word, sign_extension);

	if (rm_operand.type == Operand_type::Memory || rm_operand.type == Operand_type::Direct_address) {
		rm_operand.explicit_size = true;
	}

	int16_t value = read_data(data, offset, word, sign_extension);
	Operand immediate_operand = {.type = Operand_type::Immediate, .size = (word ? Operand_size::Word : Operand_size::Byte), .value = value };

	instruction.name = instruction_name_map[name];
	instruction.operands[0] = rm_operand;
	instruction.operands[1] = immediate_operand;
	instruction.size = offset - offset_start;

	return instruction;
}

Instruction memory_or_accumulator_to_either(Instruction_name name, std::vector<unsigned char>& data, size_t& offset, bool to_memory) 
{
	int offset_start = offset;
	bool word = data[offset] & 0b1; // 0 = move 1 byte, 1 = move 2 bytes
	offset++;

	int16_t value = read_data(data, offset, word, false);

	Instruction instruction;
	Operand memory_op = {.type = Operand_type::Direct_address, .size = (word ? Operand_size::Word : Operand_size::Byte), .value = value};
	Operand acc_op = {.type = Operand_type::Register, .size = (word ? Operand_size::Word : Operand_size::Byte), .value = 0}; // ax/al register
	instruction.name = instruction_name_map[name];
	instruction.operands[0] = to_memory ? memory_op : acc_op;
	instruction.operands[1] = to_memory ? acc_op : memory_op;
	instruction.size = offset - offset_start;

	return instruction;
}

Instruction immediate_to_accumulator(Instruction_name name, std::vector<unsigned char>& data, size_t& offset) 
{
	int offset_start = offset;
	bool word = data[offset] & 0b1; // 0 = move 1 byte, 1 = move 2 bytes
	offset++;

	int16_t value = read_data(data, offset, word, false);
	Operand immediate_operand = {.type = Operand_type::Immediate, .size = (word ? Operand_size::Word : Operand_size::Byte), .explicit_size = true, .value = value };
	Operand acc_op = {.type = Operand_type::Register, .size = (word ? Operand_size::Word : Operand_size::Byte), .value = 0}; // ax/al register

	Instruction instruction;
	instruction.name = instruction_name_map[name];
	instruction.operands[0] = acc_op;
	instruction.operands[1] = immediate_operand;
	instruction.size = offset - offset_start;

	return instruction;
}

Instruction jump_instruction(Instruction_name name, std::vector<unsigned char>& data, size_t& offset) 
{
	int offset_start = offset;
	offset++;
	int8_t value = static_cast<int8_t>(data[offset]);
	offset += 1;
	Operand operand = {.type = Operand_type::Immediate, .size = Operand_size::Byte, .value = value};

	Instruction instruction;
	instruction.name = instruction_name_map[name];
	instruction.operands[0] = {.type = Operand_type::Immediate, .size = Operand_size::Byte, .value = value}; 
	instruction.single_operand = true;

	return instruction;
}
*/



Instruction_encoding find_encoding(std::vector<unsigned char>& data, size_t offset) 
{
	for (Instruction_encoding encoding : get_all_instructions()) {
		Instruction_bits bits = encoding.bytes[0].bits[0];
		if (data[offset] >> (8 - bits.size) == bits.value) {
			return encoding;
		}
	}

	std::cerr << "Didn't find encoding for bits " << std::bitset<8>(data[offset]);
	std::exit(1);
}

enum Displacement {
	None,
	Eight_bit,
	Sixteen_bit
};


std::vector<Instruction> decode_asm_file(const std::string& path)
{
	std::ifstream asm_file(path, std::ios::binary);
	if (!asm_file) {
		throw std::runtime_error("Failed to open asm file");
	}

	std::vector<unsigned char> data(
		(std::istreambuf_iterator<char>(asm_file)),
		std::istreambuf_iterator<char>());

	std::vector<Instruction> instructions;

	size_t offset = 0;
	while (offset < data.size()) {
		Instruction_encoding encoding = find_encoding(data, offset);

		Instruction instruction;
		instruction.name = encoding.name;
		instruction.encoding_description = encoding.description;
		bool wide;
		bool direction;
		char mod;
		Displacement displacement_type = Displacement::None;
		Operand reg_operand;
		Operand rm_operand;
		Operand immediate_operand;
		uint16_t data_field;

		std::vector<unsigned char> processed_bytes;

		for (Instruction_byte byte : encoding.bytes) {
			uint8_t bitPos = 8;
			bool byte_read = true;
			for (Instruction_bits bit : byte.bits) {
				bitPos -= bit.size;
				int value = (data[offset] >> bitPos) & ((1 << bit.size) - 1);
				switch (bit.usage) {
					case Instruction_bits_usage::Literal:
						if (value != bit.value) {
							std::cerr << "Invalid" << std::endl;
						}
						break;
					case Instruction_bits_usage::Mod:
						mod = value;
						break;
					case Instruction_bits_usage::	Reg: {
						std::string reg = wide ? reg_names_16bit[value] : reg_names_8bit[value];
						reg_operand.type = Operand_type::Register;
						reg_operand.reg = wide ? reg_names_16bit[value] : reg_names_8bit[value];
						break;
					}
					case Instruction_bits_usage::	Rm:
						switch (mod) {
							case 0b00: {// Memory Mode (no displacement, except when r/m = 110)
								if (value == 0b110) { //direct address
									displacement_type = Displacement::Sixteen_bit;
								}
								else {
									rm_operand.type = Operand_type::Memory;
									rm_operand.address = rm_field_encodings[value];
								}
								break;
							}
							case 0b01: // Memory Mode, 8-bit displacement follows
								displacement_type = Displacement::Eight_bit;
								rm_operand.type = Operand_type::Memory;
								rm_operand.address = rm_field_encodings[value];
								break;
							case 0b10: {// Memory Mode, 16-bit displacement follows
								displacement_type = Displacement::Sixteen_bit;
								rm_operand.address = rm_field_encodings[value];
								break;
							}
							case 0b11: // Register Mode (no displacement)
								rm_operand.type = Operand_type::Register;
								rm_operand.reg = wide ? reg_names_16bit[value] : reg_names_8bit[value];
								break;
						}
						break;
					case Instruction_bits_usage::	Disp_lo:
						if (displacement_type == Displacement::None)
						{
							byte_read = false;
							continue;
						}
						rm_operand.displacement = displacement_type == Displacement::Sixteen_bit ? 
							static_cast<int16_t>(value) : static_cast<int8_t>(value);
						break;
					case Instruction_bits_usage::	Disp_hi:
						if (displacement_type == Displacement::None || displacement_type == Displacement::Eight_bit)
						{
							byte_read = false;
							continue;
						}
						rm_operand.displacement |= (static_cast<uint16_t>(value) << 8);
						break;
					case Instruction_bits_usage::	Data:
						immediate_operand.type = Operand_type::Immediate;
						immediate_operand.immediate = wide ? static_cast<int16_t>(value) : static_cast<int8_t>(value);
						break;
					case Instruction_bits_usage::	Data_if_w:
						if (!wide)
						{
							byte_read = false;
							continue;
						}
						immediate_operand.immediate |= (static_cast<uint16_t>(value) << 8);

						break;
					case Instruction_bits_usage::	Direction:
						direction = value;
						break;
					case Instruction_bits_usage::	Wide:
						wide = value;
						break;
					case Instruction_bits_usage::	Addr:
						break;

					//Implicit usages
					case Instruction_bits_usage::Imp_Direction:
						byte_read = false;
						direction = bit.value;
						break;
				}
			}
			if (byte_read) 
			{
				instruction.processed_bytes.emplace_back(data[offset]);
				offset++;
			}
		}


		instruction.operands[0] = direction ? reg_operand : rm_operand;
		instruction.operands[1] = direction ? rm_operand : reg_operand;

		if (immediate_operand.immediate == Operand_type::Immediate) {
			instruction.operands[1] = immediate_operand;
		}

		instructions.emplace_back(instruction);
	}
	return instructions;
	/*
	while (offset < data.size()) {
		std::cout << std::bitset<8>(data[offset]) << std::endl;
		Instruction instruction;

		if (data[offset] >> 2 == 0b100010) {
			instruction =  reg_or_mem_with_reg_to_either(Mov, data, offset);
		} 
		else if (data[offset] >> 4 == 0b1011) { //MOV immediate to register
			instruction = immediate_to_register(Mov, data, offset);
		} 
		else if (data[offset] >> 1 == 0b1010000) { //MOV memory to accumulator
			instruction = memory_or_accumulator_to_either(Mov, data, offset, false);
		} 
		else if (data[offset] >> 1 == 0b1010001) { //MOV accumulator to memory
			instruction = memory_or_accumulator_to_either(Mov, data, offset, true);
		} 
		else if (data[offset] >> 1 == 0b1100011) { //MOV immediate to register/memory
			instruction = immediate_to_reg_mem(Mov, data, offset);
			instruction.explicit_size = true;
		}
		else if (data[offset] >> 2 == 0b0) { //ADD reg/memory with register to either
			instruction = reg_or_mem_with_reg_to_either(Add, data, offset);
		}
		else if (data[offset] >> 2 == 0b100000) { //ADD immediate to register/memory
			Instruction_name instruction_name;
			char instr = (data[offset + 1] & 0b111000) >> 3;
			switch (instr) {
				case 0b0:
					instruction_name = Add;
					break;
				case 0b101:
					instruction_name = Sub;
					break;
				case 0b111:
					instruction_name = Cmp;
					break;
			}
			instruction = immediate_to_reg_mem(instruction_name, data, offset);
			instruction.explicit_size = true;
		}
		else if (data[offset] >> 1 == 0b0000010) { //ADD immediate to accumulator
			instruction = immediate_to_accumulator(Add, data, offset);
		} 
		else if (data[offset] >> 2 == 0b001010) { //SUB reg/memory with register to either
			instruction = reg_or_mem_with_reg_to_either(Sub, data, offset);
		}
		else if (data[offset] >> 1 == 0b0010110) { //SUB immediate to accumulator
			instruction = immediate_to_accumulator(Sub, data, offset);
		} 
		else if (data[offset] >> 2 == 0b001110) { //CMP reg/memory with register to either
			instruction = reg_or_mem_with_reg_to_either(Cmp, data, offset);
		}
		else if (data[offset] >> 1 == 0b0011110) { //CMP immediate to accumulator
			instruction = immediate_to_accumulator(Cmp, data, offset);
		} 
		else if (data[offset] == 0b1110101) { //JNE/JNZ
			instruction = jump_instruction(Jne, data, offset);
		}
		else if (data[offset] == 0b01110100) { //JE/JZ
			instruction = jump_instruction(Je, data, offset);
		}
		else if (data[offset] == 0b01111100) { //JL/JNGE
			instruction = jump_instruction(Jl, data, offset);
		}
		else if (data[offset] == 0b01111110) { //JLE/JNG
			instruction = jump_instruction(Jle, data, offset);
		}
		else if (data[offset] == 0b01110010) { //JB/JNAE
			instruction = jump_instruction(Jb, data, offset);
		}
		else if (data[offset] == 0b01110110) { //JBE/JNA
			instruction = jump_instruction(Jbe, data, offset);
		}
		else if (data[offset] == 0b01111010) { //JP/JPE
			instruction = jump_instruction(Jp, data, offset);
		}
		else if (data[offset] == 0b01110000) { //JO
			instruction = jump_instruction(Jo, data, offset);
		}
		else if (data[offset] == 0b01111000) { //JS
			instruction = jump_instruction(Js, data, offset);
		}
		else if (data[offset] == 0b01111101) { //JNL/JGE
			instruction = jump_instruction(Jnl, data, offset);
		}
		else if (data[offset] == 0b01111111) { //JNLE/JG
			instruction = jump_instruction(Jg, data, offset);
		}
		else if (data[offset] == 0b01110011) { //JNB/JAE
			instruction = jump_instruction(Jnb, data, offset);
		}
		else if (data[offset] == 0b01110111) { //JNBE/JA
			instruction = jump_instruction(Ja, data, offset);
		}
		else if (data[offset] == 0b01111011) { //JNP/JPO
			instruction = jump_instruction(Jnp, data, offset);
		}
		else if (data[offset] == 0b01110001) { //JNO
			instruction = jump_instruction(Jno, data, offset);
		}
		else if (data[offset] == 0b01111001) { //JNS
			instruction = jump_instruction(Jns, data, offset);
		}
		else if (data[offset] == 0b11100010) { //LOOP
			instruction = jump_instruction(loop, data, offset);
		}
		else if (data[offset] == 0b11100001) { //LOOPZ/LOOPE
			instruction = jump_instruction(loopz, data, offset);
		}
		else if (data[offset] == 0b11100000) { //LOOPNZ/LOOPNE
			instruction = jump_instruction(loopnz, data, offset);
		}
		else if (data[offset] == 0b11100011) { //JCXZ
			instruction = jump_instruction(jcxz, data, offset);
		}
		else {
			throw std::runtime_error("unknown instruction");
		}

		std::cout << instruction.to_string() << "\n" << std::endl;
		output << instruction.to_string();
	}
	*/
}

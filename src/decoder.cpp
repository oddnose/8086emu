#include "decoder.h"
#include <csetjmp>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
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
		bool found = false;
		for (Instruction_encoding encoding : get_all_instructions()) {

			std::optional<Instruction> instruction = decode_instruction(data, offset, encoding);
			if (!instruction.has_value()) {
				continue;
			}
			found = true;
			instructions.emplace_back(instruction.value());
			offset += instruction->processed_bytes.size();
			break;
		}

		if (!found) {
			for (Instruction inst : instructions) {
				inst.print_debug();
			}

			std::cerr << "Didn't find an encoding for instruction starting with " << std::bitset<8>(data[offset]);
			std::exit(1);
		}
	}

	return instructions;
}


std::optional<Instruction> decode_instruction(std::vector<unsigned char> data, size_t offset, Instruction_encoding encoding)
{
	Instruction instruction = {};
	instruction.name = encoding.name;
	instruction.encoding_description = encoding.description;
	bool wide = false;
	bool direction = false;
	bool s_bit = false;
	char mod;
	Displacement displacement_type = Displacement::None;
	Operand reg_operand = {};
	Operand rm_operand = {};
	Operand immediate_operand = {};
	uint16_t data_field = 0;

	std::vector<unsigned char> processed_bytes;

	for (Instruction_byte byte : encoding.bytes) {

		uint8_t bitPos = 0;
		bool byte_read = true;
		for (Instruction_bits bit : byte.bits) {
			bitPos += bit.size;
			int value = data[offset] >> (8 - bitPos) & ((1 << bit.size) - 1);

			switch (bit.usage) {
				case Instruction_bits_usage::Literal:
					if (value != bit.value) {
						return std::nullopt;
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
								rm_operand.type = Operand_type::Direct_address;
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
							rm_operand.type = Operand_type::Memory;
							rm_operand.address = rm_field_encodings[value];
							break;
						}
						case 0b11: // Register Mode (no displacement)
							rm_operand.type = Operand_type::Register;
							rm_operand.reg = wide ? reg_names_16bit[value] : reg_names_8bit[value];
							break;
					}
					break;
				case Instruction_bits_usage::Disp_lo:
					if (displacement_type == Displacement::None)
					{
						byte_read = false;
						continue;
					}
					rm_operand.displacement = displacement_type == Displacement::Sixteen_bit ? 
						static_cast<int16_t>(value) : static_cast<int8_t>(value);
					break;
				case Instruction_bits_usage::Disp_hi:
					if (displacement_type == Displacement::None || displacement_type == Displacement::Eight_bit)
					{
						byte_read = false;
						continue;
					}
					rm_operand.displacement |= (static_cast<uint16_t>(value) << 8);
					break;
				case Instruction_bits_usage::Data:
					immediate_operand.type = Operand_type::Immediate;
					immediate_operand.immediate = wide ? static_cast<int16_t>(value) : static_cast<int8_t>(value);
					break;
				case Instruction_bits_usage::Data_if_w:
					if (!wide || s_bit)
					{
						byte_read = false;
						break;
					}

					immediate_operand.immediate |= (static_cast<uint16_t>(value) << 8);

					break;
				case Instruction_bits_usage::Direction:
					direction = value;
					break;
				case Instruction_bits_usage::Wide:
					wide = value;
					break;
				case Instruction_bits_usage::Addr_lo:
					rm_operand.type = Operand_type::Direct_address;
					rm_operand.displacement = wide ? static_cast<int16_t>(value) : static_cast<int8_t>(value);

					reg_operand.type = Operand_type::Register;
					break;
				case Instruction_bits_usage::Addr_hi:
					rm_operand.displacement |= (static_cast<uint16_t>(value) << 8);
					break;
				case Instruction_bits_usage::S_Bit:
					std::cout << "s bit" << std::endl;
					s_bit = true;
					break;

				//Implicit usages
				case Instruction_bits_usage::Imp_Direction:
					byte_read = false;
					direction = bit.value;
					break;
				case Instruction_bits_usage::Imp_Accumulator:
					byte_read = false;
					reg_operand.reg = wide ? reg_names_16bit[0] : reg_names_8bit[0]; //accumulator used
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

	if (immediate_operand.type == Operand_type::Immediate) {
		instruction.operands[1] = immediate_operand;
	}

	return std::make_optional(instruction);
}

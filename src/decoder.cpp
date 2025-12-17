#include "decoder.h"
#include <bitset>
#include <csetjmp>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <bitset>
#include <vector>
#include "instruction-table.hpp"
#include "instruction.hpp"
#include "memory.hpp"

#define array_count(array) (sizeof(array) / sizeof((array)[0]))

const enum Register reg_names_8bit[] = { 
	Register::Al,
	Register::Cl,
	Register::Dl,
	Register::Bl,
	Register::Ah,
	Register::Ch,
	Register::Dh,
	Register::Bh
};
const enum Register reg_names_16bit[] = { 
	Register::Ax,
	Register::Cx,
	Register::Dx,
	Register::Bx,
	Register::Sp,
	Register::Bp,
	Register::Si,
	Register::Di
};

const enum Register seg_reg_names[] = { 
	Register::Es,
	Register::Cs,
	Register::Ss,
	Register::Ds
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



std::optional<Instruction> decode_instruction(std::vector<unsigned char> data, size_t offset, Instruction_encoding encoding)
{
	Instruction instruction = {};
	instruction.name = encoding.name;
	instruction.encoding_description = encoding.description;
	bool wide = false;
	bool direction = false;
	bool sign = false;
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
						break;
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
					if (!wide || sign)
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

					break;
				case Instruction_bits_usage::Addr_hi:
					rm_operand.displacement |= (static_cast<uint16_t>(value) << 8);
					break;
				case Instruction_bits_usage::Sign:
					sign = value;
					break;
				case Instruction_bits_usage::Rel_offset:
					rm_operand.type = Operand_type::Relative_offset;
					rm_operand.displacement = static_cast<int8_t>(value);
					break;
				case Instruction_bits_usage::Sr:
					reg_operand.type = Operand_type::Register;
					reg_operand.reg = seg_reg_names[value];
					break;
				case Instruction_bits_usage::Port:
					reg_operand.type = Operand_type::Register;
					reg_operand.reg = wide ? reg_names_16bit[0] : reg_names_8bit[0];

					rm_operand.type = Operand_type::Immediate;
					rm_operand.immediate = value;
					break;
				case Instruction_bits_usage::Variable:
					if (value) {
						reg_operand.type = Operand_type::Register;
						reg_operand.reg = reg_names_8bit[value];
					}
					else {
						reg_operand.type = Operand_type::Immediate;
						reg_operand.immediate = 1;
					}
					break;
				case Instruction_bits_usage::IP_lo:
					rm_operand.type = Operand_type::Immediate;
					rm_operand.immediate = wide ? static_cast<int16_t>(value) : static_cast<int8_t>(value);
					break;
				case Instruction_bits_usage::IP_hi:
					rm_operand.immediate |= static_cast<int16_t>(value) << 8;
					break;
				case Instruction_bits_usage::CS_lo:
					instruction.direct_intersegment = true;
					reg_operand.type = Operand_type::Immediate;
					reg_operand.immediate = wide ? static_cast<int16_t>(value) : static_cast<int8_t>(value);
					break;
				case Instruction_bits_usage::CS_hi:
					reg_operand.immediate |= static_cast<int16_t>(value) << 8;
					break;
				case Instruction_bits_usage::IP_inc_lo:
					rm_operand.type = Operand_type::Immediate;
					rm_operand.immediate = wide ? static_cast<int16_t>(value) : static_cast<int8_t>(value);
					break;
				case Instruction_bits_usage::IP_inc_hi:
					rm_operand.immediate |= static_cast<int16_t>(value) << 8;
					rm_operand.immediate += offset + 1;
					//rm_operand.displacement += instruction.processed_bytes.size() + 1;
					break;
					

				//Implicit usages
				case Instruction_bits_usage::Imp_Direction:
					byte_read = false;
					direction = bit.value;
					break;
				case Instruction_bits_usage::Imp_Accumulator: {
					byte_read = false;
					enum Register acc = wide ? reg_names_16bit[0] : reg_names_8bit[0]; //accumulator used
					if (reg_operand.type != Operand_type::No_op_type) {
						rm_operand.type = Operand_type::Register;
						rm_operand.reg = acc;
					} 
					else {
						reg_operand.type = Operand_type::Register;
						reg_operand.reg = acc;
					}
					break;
				}
				case Instruction_bits_usage::Imp_Wide:
					byte_read = false;
					wide = bit.value;
					break;
				case Instruction_bits_usage::Imp_Port:
					byte_read = false;
					reg_operand.type = Operand_type::Register;
					reg_operand.reg = wide ? reg_names_16bit[0] : reg_names_8bit[0];

					rm_operand.type = Operand_type::Register;
					rm_operand.reg = reg_names_16bit[2]; //dx
					break;
				case Instruction_bits_usage::Imp_Prefix:
					for (Instruction_encoding enc : get_all_instructions()) {

						std::optional<Instruction> instruction = decode_instruction(data, offset, enc);
						if (instruction.has_value()) {
							instruction.value().prefix = get_instruction_name(encoding.name) + " ";
							instruction.value().processed_bytes.insert(instruction.value().processed_bytes.begin(), data[offset - 1]);

							return instruction;
						}
					}
					return std::nullopt;
				case Instruction_bits_usage::Imp_Sr_Prefix:
					for (Instruction_encoding enc : get_all_instructions()) {

						std::optional<Instruction> instruction = decode_instruction(data, offset, enc);
						if (instruction.has_value()) {
							if (instruction.value().operands[0].type == Operand_type::Memory || instruction.value().operands[0].type == Operand_type::Direct_address) {
								instruction.value().operands[0].prefix = get_register_name(reg_operand.reg) + ":";
							}
							else if (instruction.value().operands[1].type == Operand_type::Memory || instruction.value().operands[1].type == Operand_type::Direct_address) {
								instruction.value().operands[1].prefix = get_register_name(reg_operand.reg) + ":";
							}
							else {
								std::cerr << "No memory operands event though using segment prefix" << std::endl;
								std::exit(1);
							}
							instruction.value().processed_bytes.insert(instruction.value().processed_bytes.begin(), data[offset - 1]);

							return instruction;
						}
					}
					return std::nullopt;
					
				case Instruction_bits_usage::Imp_Postfix:
					byte_read = false;
					instruction.postfix = wide ? "w" : "b";
					break;
				case Instruction_bits_usage::Imp_Far: 
					byte_read = false;
					instruction.far = true;
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
		if (instruction.operands[0].type == Operand_type::No_op_type) {
			instruction.operands[0] = immediate_operand;
		}
		else {
			instruction.operands[1] = immediate_operand;
		}
	}
	instruction.wide = wide;

	return std::make_optional(instruction);
}

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
			instruction.value().instruction_pointer = offset;
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

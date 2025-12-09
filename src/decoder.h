#pragma once
#include "instruction-table.hpp"
#include <optional>
#include <string>
#include <vector>
#include <sstream>
#include <bitset>
#include <iostream>

enum Operand_type {
	Register,
	Memory,
	Immediate,
	Direct_address
};
enum Operand_size {
	Byte,
	Word
};

struct Operand {
	Operand_type type;
	//TODO: use union if possible
	std::string reg;
	int16_t displacement = 0;
	int16_t immediate = 0;
	std::string address;
	/*
	Operand_size size;
	bool explicit_size = false;
	int16_t value = 0;
	int16_t displacement = 0;
	*/

	std::string to_string() 
	{
		switch (type) {
			case Register:
				return reg;
			case Memory:
				if (displacement == 0) {
					return "[" + address + "]";
				}
				else if (displacement > 0) {
					return "[" + address + " + " + std::to_string(displacement) +  "]";
				}
				else {
					return "[" + address + " - " + std::to_string(-displacement) +  "]";
				}
			case Immediate:
				return std::to_string(immediate);
			case Direct_address:
				return "[" + std::to_string(displacement) + "]";
		}
		return "";
	}
};

struct Instruction {
	std::string name;
	Operand operands[2];
	bool single_operand = false;
	bool wide;

	std::vector<unsigned char> processed_bytes;
	std::string encoding_description;

	std::string to_string() 
	{
		/*
		if (single_operand) {
			return name + " " + operands[0].to_string() + "\n";
		}
		*/

		std::string operand_size;
		if (operands[0].type != Register) {
			operand_size = wide ? "word " : "byte ";
		}
		return name + " " + operand_size +  operands[0].to_string() + ", " + operands[1].to_string();
	}

	void print_debug()
	{
		std::stringstream ss;
		for (auto b : processed_bytes)
		{
			ss << std::bitset<8>(b) << " ";
		}

		std::cout << "Used encoding: " << encoding_description << std::endl;
		std::cout << "to_string: " << to_string() << std::endl;
		std::cout << "Affected bytes: " << std::endl;
		std::cout << ss.str() << std::endl;
	}
};

std::vector<Instruction> decode_asm_file(const std::string& path);
std::optional<Instruction> decode_instruction(std::vector<unsigned char> data, size_t offset, Instruction_encoding encoding);

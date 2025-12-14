#pragma once

#include "instruction-table.hpp"
#include <string>

enum Operand_type {
  No_op_type,
  Register,
  Memory,
  Immediate,
  Direct_address,
  Relative_offset
};
enum Operand_size { Byte, Word };

struct Operand {
  Operand_type type = No_op_type;
  std::string prefix;
  // TODO: use union if possible
  std::string reg;
  int16_t displacement = 0;
  int16_t immediate = 0;
  std::string address;
  size_t rel_size = 2;

	std::string to_string();
};

struct Instruction {
  Instruction_type name;
  std::string prefix;
  std::string postfix;
  int instruction_pointer;
  Operand operands[2];
  bool wide;
  bool direct_intersegment;
  bool far;

  std::vector<unsigned char> processed_bytes;
  std::string encoding_description;

	std::string to_string();
	void print_debug();
};

std::string get_instruction_name(Instruction_type type);

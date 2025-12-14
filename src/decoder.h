#pragma once
#include "instruction-table.hpp"
#include <bitset>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

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

  std::string to_string() {
    switch (type) {
    case No_op_type:
      return "";
    case Register:
      return reg;
    case Memory:
      if (displacement == 0) {
        return prefix + "[" + address + "]";
      } else if (displacement > 0) {
        return prefix + "[" + address + " + " + std::to_string(displacement) + "]";
      } else {
        return prefix + "[" + address + " - " + std::to_string(-displacement) + "]";
      }
    case Immediate:
      return std::to_string(immediate);
    case Direct_address:
      return prefix + "[" + std::to_string(displacement) + "]";
    case Relative_offset:
      // Adding 2 since that is the size of instructions using relative offset
      if (displacement + 2 > 0) {
        return "$+" + std::to_string(displacement + 2) + "+0";
      } else if (displacement + 2 == 0) {
        return "$+0";
      } else {
        return "$" + std::to_string(displacement + 2) + "+0";
      }
    }
    return "";
  }
};

struct Instruction {
  std::string name;
  std::string prefix;
  std::string postfix;
  Operand operands[2];
  bool wide;

  std::vector<unsigned char> processed_bytes;
  std::string encoding_description;

  std::string to_string() {
    std::ostringstream output;

    if (!prefix.empty()) {
      output << prefix;
    }
    output << name;

    std::string operand_size;
    if (operands[0].type == Memory || operands[0].type == Direct_address) {
      output << (wide ? " word" : " byte");
    }

    std::string op0_string = operands[0].to_string();
    std::string op1_string = operands[1].to_string();

    if (!op0_string.empty()) {
      output << " " << op0_string;
    }
    if (!op1_string.empty()) {
      output << ", " << op1_string;
    }

    if (!postfix.empty()) {
      output << postfix;
    }
    return output.str();
  }

  void print_debug() {
    std::stringstream ss;
    for (auto b : processed_bytes) {
      ss << std::bitset<8>(b) << " ";
    }

    std::cout << "Used encoding: " << encoding_description << std::endl;
    std::cout << "to_string: " << to_string() << std::endl;
    std::cout << "Affected bytes: " << std::endl;
    std::cout << ss.str() << std::endl;
  }
};

std::vector<Instruction> decode_asm_file(const std::string &path);
std::optional<Instruction> decode_instruction(std::vector<unsigned char> data,
                                              size_t offset,
                                              Instruction_encoding encoding);

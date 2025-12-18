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

enum Register {
	Ax,
	Cx,
	Dx,
	Bx,
	Sp, // stack pointer
	Bp, // base pointer
	Si, // source index
	Di, // des index

	//Low registers
	Al,
	Cl,
	Dl,
	Bl,

	//High registers
	Ah,
	Ch,
	Dh,
	Bh,

	//Segment registers
	Es,
	Cs,
	Ss,
	Ds
};

enum Rm_field_encodings {
	Bx_si_rm,
	Bx_di_rm,
	Bp_si_rm,
	Bp_di_rm,
	Si_rm,
	Di_rm,
	Bp_rm,
	Bx_rm,
};

struct Operand {
  Operand_type type = No_op_type;
  std::string prefix;
	Operand_size size;
  // TODO: use union if possible
  enum Register reg;
  int16_t displacement = 0;
  int16_t immediate = 0;
  Rm_field_encodings address;
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
std::string get_register_name(enum Register type);
std::string get_rm_encoding_name(enum Rm_field_encodings encoding);

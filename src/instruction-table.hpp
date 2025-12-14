#pragma once
#include <stdint.h>
#include <string>
#include <vector>

enum Instruction_bits_usage
{
	Literal,
	Mod,
	Reg,
	Rm,
	Disp_lo,
	Disp_hi,
	Data,
	Data_if_w,
	Direction,
	Sign, 
	Wide,
	Addr_lo,
	Addr_hi,
	Rel_offset,
	Sr, //Segment Register
	Port,
	Variable,
	Zero,

	Imp_Direction,
	Imp_Accumulator,
	Imp_Wide,
	Imp_Port,
	Imp_Prefix,
	Imp_Sr_Prefix,
	Imp_Postfix

};

struct Instruction_bits 
{
	Instruction_bits_usage usage;
	uint8_t value;
	uint8_t size;
};

struct Instruction_byte
{
	std::vector<Instruction_bits> bits;
};

struct Instruction_encoding 
{
	std::string name;
	const char* description;
	std::vector<Instruction_byte> bytes;
};

std::vector<Instruction_encoding> get_all_instructions();


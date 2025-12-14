#pragma once
#include <stdint.h>
#include <string>
#include <vector>

enum Instruction_type {
	Mov,
	Push,
	Pop,
	Xchg,
	In,
	Out,
	Xlat,
	Lea,
	Lds,
	Les,
	Lahf,
	Sahf,
	Pushf,
	Popf,
	Add,
	Adc,
	Inc,
	Aaa,
	Daa,
	Sub,
	Sbb,
	Dec,
	Neg,
	Cmp,
	Aas,
	Das,
	Mul,
	Imul,
	Aam,
	Div,
	Idiv,
	Aad,
	Cbw,
	Cwd,
	Not,
	Shl_Sal,
	Shr,
	Sar,
	Rol,
	Ror,
	Rcl,
	Rcr,
	And,
	Test,
	Or,
	Xor,
	Rep,
	Movs,
	Cmps,
	Scas,
	Lods,
	Stos,
	Call,
	Jmp,
	Ret,
	Retf,
	Je_Jz,
	Jl_Jnge,
	Jle_Jng,
	Jb_Jnae,
	Jbe_Jna,
	Jp_Jpe,
	Jo,
	Js,
	Jne_Jnz,
	Jnl_Jge,
	Jnle_Jg,
	Jnb_Jae,
	Jnbe_Ja,
	Jnp_Jpo,
	Jno,
	Jns,
	Loop,
	Loopz_Loope,
	Loopnz_Loopne,
	Jcxz,
	Int,
	Int3,
	Into,
	Iret,
	Clc,
	Cmc,
	Stc,
	Cld,
	Std,
	Cli,
	Sti,
	Hlt,
	Wait,
	Esc, //Not supported, due to NASM not supporting it as of now
	Lock,
	Segment
};

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
	IP_lo, //Instruction Pointer
	IP_hi,
	CS_lo, //Code Segment
	CS_hi,
	IP_inc_lo, 
	IP_inc_hi,

	Imp_Direction,
	Imp_Accumulator,
	Imp_Wide,
	Imp_Port,
	Imp_Prefix,
	Imp_Sr_Prefix,
	Imp_Postfix,
	Imp_Far

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
	Instruction_type name;
	const char* description;
	std::vector<Instruction_byte> bytes;
};

std::vector<Instruction_encoding> get_all_instructions();


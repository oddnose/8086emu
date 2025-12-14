#include "instruction.hpp"
#include <bitset>
#include <iostream>
#include <sstream>

std::string Operand::to_string() {
	switch (type) {
	case No_op_type:
		return "";
	case Register:
		return reg;
	case Memory:
		if (displacement == 0) {
			return prefix + "[" + address + "]";
		} else if (displacement > 0) {
			return prefix + "[" + address + " + " + std::to_string(displacement) +
						 "]";
		} else {
			return prefix + "[" + address + " - " + std::to_string(-displacement) +
						 "]";
		}
	case Immediate:
		return std::to_string(immediate);
	case Direct_address:
		return prefix + "[" + std::to_string(displacement) + "]";
	case Relative_offset:
		// Adding 2 since that is the size of instructions using relative offset
		if (displacement + rel_size > 0) {
			return "$+" + std::to_string(displacement + rel_size) + "+0";
		} else if (displacement + rel_size == 0) {
			return "$+0";
		} else {
			return "$" + std::to_string(displacement + rel_size) + "+0";
		}
	}
	return "";
}

std::string Instruction::to_string() {
	std::ostringstream output;

	if (!prefix.empty()) {
		output << prefix;
	}
	output << get_instruction_name(name);

	std::string operand_size;
	if (operands[0].type == Memory || operands[0].type == Direct_address) {
		output << (wide ? " word" : " byte");
	}

	if (far) {
		output << " far";
	}

	std::string op0_string = operands[0].to_string();
	std::string op1_string = operands[1].to_string();

	if (!op0_string.empty()) {
		output << " " << op0_string;
	}
	if (!op1_string.empty()) {
		output << (direct_intersegment ? ":" : ", ") << op1_string;
	}

	if (!postfix.empty()) {
		output << postfix;
	}
	return output.str();
}

void Instruction::print_debug() {
	std::stringstream ss;
	for (auto b : processed_bytes) {
		ss << std::bitset<8>(b) << " ";
	}

	std::cout << "Used encoding: " << encoding_description << std::endl;
	std::cout << "to_string: " << to_string() << std::endl;
	std::cout << "Affected bytes: " << std::endl;
	std::cout << ss.str() << std::endl;
}

std::string get_instruction_name(Instruction_type type) {
	switch(type) {
		case Instruction_type::Mov: return "mov";
		case Instruction_type::Push: return "push";
		case Instruction_type::Pop: return "pop";
		case Instruction_type::Xchg: return "xchg";
		case Instruction_type::In: return "in";
		case Instruction_type::Out: return "out";
		case Instruction_type::Xlat: return "xlat";
		case Instruction_type::Lea: return "lea";
		case Instruction_type::Lds: return "lds";
		case Instruction_type::Les: return "les";
		case Instruction_type::Lahf: return "lahf";
		case Instruction_type::Sahf: return "sahf";
		case Instruction_type::Pushf: return "pushf";
		case Instruction_type::Popf: return "popf";
		case Instruction_type::Add: return "add";
		case Instruction_type::Adc: return "adc";
		case Instruction_type::Inc: return "inc";
		case Instruction_type::Aaa: return "aaa";
		case Instruction_type::Daa: return "daa";
		case Instruction_type::Sub: return "sub";
		case Instruction_type::Sbb: return "sbb";
		case Instruction_type::Dec: return "dec";
		case Instruction_type::Neg: return "neg";
		case Instruction_type::Cmp: return "cmp";
		case Instruction_type::Aas: return "aas";
		case Instruction_type::Das: return "das";
		case Instruction_type::Mul: return "mul";
		case Instruction_type::Imul: return "imul";
		case Instruction_type::Aam: return "aam";
		case Instruction_type::Div: return "div";
		case Instruction_type::Idiv: return "idiv";
		case Instruction_type::Aad: return "aad";
		case Instruction_type::Cbw: return "cbw";
		case Instruction_type::Cwd: return "cwd";
		case Instruction_type::Not: return "not";
		case Instruction_type::Shl_Sal: return "shl";
		case Instruction_type::Shr: return "shr";
		case Instruction_type::Sar: return "sar";
		case Instruction_type::Rol: return "rol";
		case Instruction_type::Ror: return "ror";
		case Instruction_type::Rcl: return "rcl";
		case Instruction_type::Rcr: return "rcr";
		case Instruction_type::And: return "and";
		case Instruction_type::Test: return "test";
		case Instruction_type::Or: return "or";
		case Instruction_type::Xor: return "xor";
		case Instruction_type::Rep: return "rep";
		case Instruction_type::Movs: return "movs";
		case Instruction_type::Cmps: return "cmps";
		case Instruction_type::Scas: return "scas";
		case Instruction_type::Lods: return "lods";
		case Instruction_type::Stos: return "stos";
		case Instruction_type::Call: return "call";
		case Instruction_type::Jmp: return "jmp";
		case Instruction_type::Ret: return "ret";
		case Instruction_type::Retf: return "retf";
		case Instruction_type::Je_Jz: return "je";
		case Instruction_type::Jl_Jnge: return "jl";
		case Instruction_type::Jle_Jng: return "jle";
		case Instruction_type::Jb_Jnae: return "jb";
		case Instruction_type::Jbe_Jna: return "jbe";
		case Instruction_type::Jp_Jpe: return "jp";
		case Instruction_type::Jo: return "jo";
		case Instruction_type::Js: return "js";
		case Instruction_type::Jne_Jnz: return "jne";
		case Instruction_type::Jnl_Jge: return "jnl";
		case Instruction_type::Jnle_Jg: return "jnle";
		case Instruction_type::Jnb_Jae: return "jnb";
		case Instruction_type::Jnbe_Ja: return "jnbe";
		case Instruction_type::Jnp_Jpo: return "jnp";
		case Instruction_type::Jno: return "jno";
		case Instruction_type::Jns: return "jns";
		case Instruction_type::Loop: return "loop";
		case Instruction_type::Loopz_Loope: return "loopz";
		case Instruction_type::Loopnz_Loopne: return "loopnz";
		case Instruction_type::Jcxz: return "jcxz";
		case Instruction_type::Int: return "int";
		case Instruction_type::Int3: return "int3";
		case Instruction_type::Into: return "into";
		case Instruction_type::Iret: return "iret";
		case Instruction_type::Clc: return "clc";
		case Instruction_type::Cmc: return "cmc";
		case Instruction_type::Stc: return "stc";
		case Instruction_type::Cld: return "cld";
		case Instruction_type::Std: return "std";
		case Instruction_type::Cli: return "cli";
		case Instruction_type::Sti: return "sti";
		case Instruction_type::Hlt: return "hlt";
		case Instruction_type::Wait: return "wait";
		case Instruction_type::Esc: return "esc";
		case Instruction_type::Lock: return "lock";
		case Instruction_type::Segment: return ""; //doesn't have any printable instruction name
	}
	std::cerr << "Get text not supported for instruction type" << std::endl;
	std::exit(1);
}

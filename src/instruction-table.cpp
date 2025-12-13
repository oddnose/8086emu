#include "instruction-table.hpp"
#include <string>

#define BIT(usage, value, size) { usage, value, size }
#define BITS(...) { __VA_ARGS__}
#define BYTE(...) { BITS(__VA_ARGS__) }
#define ENCODING(name, desc, ...) { name, desc, { __VA_ARGS__ } }

#define FULL_BYTE(usage) BYTE(BIT(usage, 0, 8))

#define D BIT(Direction, 0, 1)
#define W BIT(Wide, 0, 1)
#define S BIT(S_Bit, 0, 1)

#define MOD BIT(Mod, 0, 2)
#define REG BIT(Reg, 0, 3)
#define RM BIT(Rm, 0, 3)

// Implied bits that are hard-coded in an instruction, or otherwise implied
#define EMPTY_BYTE(...) BYTE(__VA_ARGS__)
#define IMP_D(value) BIT(Imp_Direction, value, 0)
#define IMP_ACC BIT(Imp_Accumulator, 0, 0)


std::string mov = "mov";
std::string add = "add";
std::string sub = "sub";
std::string cmp = "cmp";

std::vector<Instruction_encoding> get_all_instructions()
{
	std::vector<Instruction_encoding> all_instructions = {
		// DATA TRANSFER

		// MOV
		ENCODING(mov, "Register/memory to/from register", BYTE(BIT(Literal, 0b100010, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(mov, "Immediate to register/memory", BYTE(BIT(Literal, 0b1100011, 7), W), BYTE(MOD, BIT(Literal, 0b000, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(0))),
		ENCODING(mov, "Immediate to register", BYTE(BIT(Literal, 0b1011, 4), W, REG), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1))),
		ENCODING(mov, "Memory to accumulator", BYTE(BIT(Literal, 0b1010000, 7), W), FULL_BYTE(Addr_lo), FULL_BYTE(Addr_hi), EMPTY_BYTE(IMP_D(1), IMP_ACC)),
		ENCODING(mov, "Accumulator to memory", BYTE(BIT(Literal, 0b1010001, 7), W), FULL_BYTE(Addr_lo), FULL_BYTE(Addr_hi), EMPTY_BYTE(IMP_D(0), IMP_ACC)),
		// Register/memory to segment register
		// Segment register to register/memory
		
		// PUSH
		// POP
		// XCHG
		// IN
		// OUT
		// XLAT
		// LEA
		// LDS
		// LES
		// LAHF
		// SAHF
		// PUSHF
		// POPF

		// ARITHEMIC
		
		// ADD
		ENCODING(add, "Reg/memory with register to either", BYTE(BIT(Literal, 0b000000, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(add, "Immediate to register/memory", BYTE(BIT(Literal, 0b100000, 6), S, W), BYTE(MOD, BIT(Literal, 0b000, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(0))),
		ENCODING(add, "Immediate to accumulator", BYTE(BIT(Literal, 0b0000010, 7), W), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1), IMP_ACC)),

		// ADC
		// INC
		// AAA
		// DAA
		
		//SUB
		ENCODING(sub, "Reg/memory and register to either", BYTE(BIT(Literal, 0b001010, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(sub, "Immediate from register/memory", BYTE(BIT(Literal, 0b100000, 6), S, W), BYTE(MOD, BIT(Literal, 0b101, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w)),
		ENCODING(sub, "Immediate from accumulator", BYTE(BIT(Literal, 0b0010110, 7), W), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1), IMP_ACC)),
		
		// SBB
		// DEC

		//CMP
		ENCODING(cmp, "Register/memory and register", BYTE(BIT(Literal, 0b001110, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(cmp, "Immediate with register/memory", BYTE(BIT(Literal, 0b100000, 6), S, W), BYTE(MOD, BIT(Literal, 0b111, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(0))),
		ENCODING(cmp, "Immediate with accumulator", BYTE(BIT(Literal, 0b0011110, 7), W), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1), IMP_ACC)),

		// AAS
		// DAS
		// MUL
		// IMUL
		// AAM
		// DIV
		// IDIV
		// AAD
		// CBW
		// CWD

		//LOGIC

		// NOT
		// SHL/SAL
		// SHR
		// SAR
		// ROL
		// ROR
		// RCL
		// RCR
		// AND
		// TEST
		// OR
		// XOR

		// STRING MANIPULATION

		// REP
		// MOVS
		// CMPS
		// SCAS
		// LODS
		// STOS

		// CONTROL TRANSFER

		// CALL
		// JMP
		// RET
		// JE/JZ
		ENCODING("je", "Jump on equal/zero", BYTE(BIT(Literal, 0b01110100, 8)),FULL_BYTE(Rel_offset)),
		ENCODING("jl", "Jump on less/not greater or equal", BYTE(BIT(Literal, 0b01111100, 8)),FULL_BYTE(Rel_offset)),
		ENCODING("jle", "Jump on less/or equal/not greater", BYTE(BIT(Literal, 0b01111110, 8)),FULL_BYTE(Rel_offset)),
		ENCODING("jb", "Jump on below/not above or equal", BYTE(BIT(Literal, 0b01110010, 8)),FULL_BYTE(Rel_offset)),
		ENCODING("jbe", "Jump on below or equal/not above", BYTE(BIT(Literal, 0b01110110, 8)),FULL_BYTE(Rel_offset)),
		ENCODING("jp", "Jump on below parity/parity even", BYTE(BIT(Literal, 0b01111010, 8)),FULL_BYTE(Rel_offset)),
		ENCODING("jo", "Jump on overflow", BYTE(BIT(Literal, 0b01110000, 8)),FULL_BYTE(Rel_offset)),
		ENCODING("js", "Jump on sign", BYTE(BIT(Literal, 0b01111000, 8)),FULL_BYTE(Rel_offset)),
		ENCODING("jne", "Jump on not equal/not zero", BYTE(BIT(Literal, 0b01110101, 8)),FULL_BYTE(Rel_offset)),
		ENCODING("jnl", "Jump on not less/greater or equal", BYTE(BIT(Literal, 0b01111101, 8)),FULL_BYTE(Rel_offset)),
		ENCODING("jnle", "Jump on not less or equal/greater", BYTE(BIT(Literal, 0b01111111, 8)),FULL_BYTE(Rel_offset)),
		ENCODING("jnb", "Jump on not below/above or equal", BYTE(BIT(Literal, 0b01110011, 8)),FULL_BYTE(Rel_offset)),
		ENCODING("jnbe", "Jump on not below or equal/above", BYTE(BIT(Literal, 0b01110111, 8)),FULL_BYTE(Rel_offset)),
		ENCODING("jnp", "Jump on not par/par odd", BYTE(BIT(Literal, 0b01111011, 8)),FULL_BYTE(Rel_offset)),
		ENCODING("jno", "Jump on not overflow", BYTE(BIT(Literal, 0b01110001, 8)),FULL_BYTE(Rel_offset)),
		// RET TODO: what?
		ENCODING("jns", "Jump on not sign", BYTE(BIT(Literal, 0b01111001, 8)),FULL_BYTE(Rel_offset)),
		ENCODING("loop", "Loop CX times", BYTE(BIT(Literal, 0b11100010, 8)),FULL_BYTE(Rel_offset)),
		ENCODING("loopz", "Loop while zero/equal", BYTE(BIT(Literal, 0b11100001, 8)),FULL_BYTE(Rel_offset)),
		ENCODING("loopnz", "Loop while not zero/equal", BYTE(BIT(Literal, 0b11100000, 8)),FULL_BYTE(Rel_offset)),
		ENCODING("jcxz", "Jump on CX zero", BYTE(BIT(Literal, 0b11100011, 8)),FULL_BYTE(Rel_offset)),

		// INT
		// INTO
		// IRET
		
		// PROCESSOR CONTROL

		// CLC
		// CMC
		// STC
		// CLD
		// STD
		// CLI
		// STI
		// HLT
		// WAIT
		// ESC
		// LOCK
		// SEGMENT
	};
	
	return all_instructions;

}

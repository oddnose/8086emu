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

// Implied bits that are hard-coded in an instruction
#define EMPTY_BYTE(...) BYTE(__VA_ARGS__)
#define IMP_D(value) BIT(Imp_Direction, value, 0)
#define IMP_ACC BIT(Imp_Accumulator, 0, 0)


std::string mov = "mov";
std::string add = "add";

std::vector<Instruction_encoding> get_all_instructions()
{
	std::vector<Instruction_encoding> all_instructions = {
	// MOV
	ENCODING(mov, "Register/memory to/from register", BYTE(BIT(Literal, 0b100010, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
	ENCODING(mov, "Immediate to register/memory", BYTE(BIT(Literal, 0b1100011, 7), W), BYTE(MOD, BIT(Literal, 0b000, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(0))),
	ENCODING(mov, "Immediate to register", BYTE(BIT(Literal, 0b1011, 4), W, REG), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1))),
	ENCODING(mov, "Memory to accumulator", BYTE(BIT(Literal, 0b1010000, 7), W), FULL_BYTE(Addr_lo), FULL_BYTE(Addr_hi), EMPTY_BYTE(IMP_D(1), IMP_ACC)),
	ENCODING(mov, "Accumulator to memory", BYTE(BIT(Literal, 0b1010001, 7), W), FULL_BYTE(Addr_lo), FULL_BYTE(Addr_hi), EMPTY_BYTE(IMP_D(0), IMP_ACC)),
	//TODO: Register/memory to segment register
	//TODO: Segment register to register/memory
	
	//TODO: PUSH
	
	//TODO: POP
	
	//TODO: XCHG
	
	//TODO: IN
	
	//TODO: OUT
	
	// ARITHEMIC
	
	// ADD
	ENCODING(add, "Reg/memory with register to either", BYTE(BIT(Literal, 0b000000, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
	ENCODING(add, "Immediate to register/memory", BYTE(BIT(Literal, 0b100000, 6), S, W), BYTE(MOD, BIT(Literal, 0b000, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(0))),
	ENCODING(add, "Immediate to accumulator", BYTE(BIT(Literal, 0b0000010, 7), W), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1), IMP_ACC))
	};
	return all_instructions;

}

#include "instruction-table.hpp"
#include <string>

#define BIT(usage, value, size) { usage, value, size }
#define BITS(...) { __VA_ARGS__}
#define BYTE(...) { BITS(__VA_ARGS__) }
#define ENCODING(name, desc, ...) { name, desc, { __VA_ARGS__ } }

#define FULL_BYTE(usage) BYTE(BIT(usage, 0, 8))

#define D BIT(Direction, 0, 1)
#define W BIT(Wide, 0, 1)
#define S BIT(Sign, 0, 1)
#define V BIT(Variable, 0, 1)
#define Z BIT(Zero, 0, 1)

#define MOD BIT(Mod, 0, 2)
#define REG BIT(Reg, 0, 3)
#define RM BIT(Rm, 0, 3)
#define SR BIT(Sr, 0, 2)

// Implied bits that are hard-coded in an instruction, or otherwise implied
#define EMPTY_BYTE(...) BYTE(__VA_ARGS__)
#define IMP_D(value) BIT(Imp_Direction, value, 0)
#define IMP_ACC BIT(Imp_Accumulator, 0, 0)
#define IMP_W(value) BIT(Imp_Wide, value, 0)
#define IMP_PORT BIT(Imp_Port, 0, 0)
#define IMP_PREFIX BIT(Imp_Prefix, 0, 0)
#define IMP_SR_PREFIX BIT(Imp_Sr_Prefix, 0, 0)
#define IMP_POSTFIX BIT(Imp_Postfix, 0, 0)


std::string mov = "mov";
std::string add = "add";
std::string sub = "sub";
std::string cmp = "cmp";

std::vector<Instruction_encoding> get_all_instructions()
{
	std::vector<Instruction_encoding> all_instructions = {
		// DATA TRANSFER
		ENCODING(mov, "Register/memory to/from register", BYTE(BIT(Literal, 0b100010, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(mov, "Immediate to register/memory", BYTE(BIT(Literal, 0b1100011, 7), W), BYTE(MOD, BIT(Literal, 0b000, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(0))),
		ENCODING(mov, "Immediate to register", BYTE(BIT(Literal, 0b1011, 4), W, REG), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1))),
		ENCODING(mov, "Memory to accumulator", BYTE(BIT(Literal, 0b1010000, 7), W), FULL_BYTE(Addr_lo), FULL_BYTE(Addr_hi), EMPTY_BYTE(IMP_D(1), IMP_ACC)),
		ENCODING(mov, "Accumulator to memory", BYTE(BIT(Literal, 0b1010001, 7), W), FULL_BYTE(Addr_lo), FULL_BYTE(Addr_hi), EMPTY_BYTE(IMP_D(0), IMP_ACC)),
		ENCODING(mov, "Register/memory to segment register", EMPTY_BYTE(IMP_W(1)), BYTE(BIT(Literal, 0b10001110, 8)), BYTE(MOD, BIT(Literal, 0b0, 1), SR, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(mov, "Segment register to register/memory", EMPTY_BYTE(IMP_W(1)), BYTE(BIT(Literal, 0b10001100, 8)), BYTE(MOD, BIT(Literal, 0b0, 1), SR, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		
		ENCODING("push", "Register/memory", EMPTY_BYTE(IMP_D(0), IMP_W(1)), BYTE(BIT(Literal, 0b11111111, 8)), BYTE(MOD, BIT(Literal, 0b110, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("push", "Register", EMPTY_BYTE(IMP_D(1), IMP_W(1)), BYTE(BIT(Literal, 0b01010, 5), REG)),
		ENCODING("push", "Segment register", BYTE(BIT(Literal, 0b000, 3), SR, BIT(Literal, 0b110, 3)), EMPTY_BYTE(IMP_D(1))),

		ENCODING("pop", "Register/memory", EMPTY_BYTE(IMP_D(0), IMP_W(1)), BYTE(BIT(Literal, 0b10001111, 8)), BYTE(MOD, BIT(Literal, 0b000, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("pop", "Register", EMPTY_BYTE(IMP_D(1), IMP_W(1)), BYTE(BIT(Literal, 0b01011, 5), REG)),
		ENCODING("pop", "Segment register", BYTE(BIT(Literal, 0b000, 3), SR, BIT(Literal, 0b111, 3)), EMPTY_BYTE(IMP_D(1))),

		ENCODING("xchg", "Register/memory with register", BYTE(BIT(Literal, 0b1000011, 7), W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), EMPTY_BYTE(IMP_D(1))),
		ENCODING("xchg", "Register with accumulator", EMPTY_BYTE(IMP_W(1), IMP_D(1)), BYTE(BIT(Literal, 0b10010, 5), REG), EMPTY_BYTE(IMP_ACC)),

		ENCODING("in", "Fixed port", BYTE(BIT(Literal, 0b1110010, 7), W), FULL_BYTE(Port), EMPTY_BYTE(IMP_D(1))),
		ENCODING("in", "Variable port", BYTE(BIT(Literal, 0b1110110, 7), W), EMPTY_BYTE(IMP_PORT, IMP_D(1))),

		ENCODING("out", "Fixed port", BYTE(BIT(Literal, 0b1110011, 7), W), FULL_BYTE(Port), EMPTY_BYTE(IMP_D(0))),
		ENCODING("out", "Variable port", BYTE(BIT(Literal, 0b1110111, 7), W), EMPTY_BYTE(IMP_PORT, IMP_D(0))),

		ENCODING("xlat", "Translate byte to AL", BYTE(BIT(Literal, 0b11010111, 8))),

		ENCODING("lea", "Load EA to register", EMPTY_BYTE(IMP_D(1), IMP_W(1)), BYTE(BIT(Literal, 0b10001101, 8)), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("lds", "Load pointer to DS", EMPTY_BYTE(IMP_D(1), IMP_W(1)), BYTE(BIT(Literal, 0b11000101, 8)), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("les", "Load pointer to ES", EMPTY_BYTE(IMP_D(1), IMP_W(1)), BYTE(BIT(Literal, 0b11000100, 8)), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),

		ENCODING("lahf", "Load AH with flags", BYTE(BIT(Literal, 0b10011111, 8))),
		ENCODING("sahf", "Store AH into flags", BYTE(BIT(Literal, 0b10011110, 8))),
		ENCODING("pushf", "Push flags", BYTE(BIT(Literal, 0b10011100, 8))),
		ENCODING("popf", "Pop flags", BYTE(BIT(Literal, 0b10011101, 8))),

		// ARITHEMIC
		
		ENCODING(add, "Reg/memory with register to either", BYTE(BIT(Literal, 0b000000, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(add, "Immediate to register/memory", BYTE(BIT(Literal, 0b100000, 6), S, W), BYTE(MOD, BIT(Literal, 0b000, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(0))),
		ENCODING(add, "Immediate to accumulator", BYTE(BIT(Literal, 0b0000010, 7), W), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1), IMP_ACC)),

		ENCODING("adc", "Reg/memory with register to either", BYTE(BIT(Literal, 0b000100, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("adc", "Immediate to register/memory", BYTE(BIT(Literal, 0b100000, 6), S, W), BYTE(MOD, BIT(Literal, 0b010, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(0))),
		ENCODING("adc", "Immediate to accumulator", BYTE(BIT(Literal, 0b0001010, 7), W), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1), IMP_ACC)),

		ENCODING("inc", "Register/memory", BYTE(BIT(Literal, 0b1111111, 7), W), BYTE(MOD, BIT(Literal, 0b000, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("inc", "Register", EMPTY_BYTE(IMP_D(1), IMP_W(1)), BYTE(BIT(Literal, 0b01000, 5), REG)),

		ENCODING("aaa", "ASCII adjust for add", BYTE(BIT(Literal, 0b00110111, 8))),
		ENCODING("daa", "Decimal adjust for add", BYTE(BIT(Literal, 0b00100111, 8))),
		
		ENCODING(sub, "Reg/memory and register to either", BYTE(BIT(Literal, 0b001010, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(sub, "Immediate from register/memory", BYTE(BIT(Literal, 0b100000, 6), S, W), BYTE(MOD, BIT(Literal, 0b101, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w)),
		ENCODING(sub, "Immediate from accumulator", BYTE(BIT(Literal, 0b0010110, 7), W), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1), IMP_ACC)),
		
		ENCODING("sbb", "Reg/memory and register to either", BYTE(BIT(Literal, 0b000110, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("sbb", "Immediate from register/memory", BYTE(BIT(Literal, 0b100000, 6), S, W), BYTE(MOD, BIT(Literal, 0b011, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w)),
		ENCODING("sbb", "Immediate from accumulator", BYTE(BIT(Literal, 0b0001110, 7), W), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1), IMP_ACC)),

		ENCODING("dec", "Register/memory", BYTE(BIT(Literal, 0b1111111, 7), W), BYTE(MOD, BIT(Literal, 0b001, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("dec", "Register", EMPTY_BYTE(IMP_D(1), IMP_W(1)), BYTE(BIT(Literal, 0b01001, 5), REG)),

		ENCODING("neg", "Change sign", BYTE(BIT(Literal, 0b1111011, 7), W), BYTE(MOD, BIT(Literal, 0b011, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),

		ENCODING(cmp, "Register/memory and register", BYTE(BIT(Literal, 0b001110, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(cmp, "Immediate with register/memory", BYTE(BIT(Literal, 0b100000, 6), S, W), BYTE(MOD, BIT(Literal, 0b111, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(0))),
		ENCODING(cmp, "Immediate with accumulator", BYTE(BIT(Literal, 0b0011110, 7), W), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1), IMP_ACC)),

		ENCODING("aas", "ASCII adjust for subtract", BYTE(BIT(Literal, 0b00111111, 8))),
		ENCODING("das", "Decimal adjust for subtract", BYTE(BIT(Literal, 0b00101111, 8))),

		ENCODING("mul", "Multiply (unsigned)", BYTE(BIT(Literal, 0b1111011, 7), W), BYTE(MOD, BIT(Literal, 0b100, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("imul", "Integer multiply (signed)", BYTE(BIT(Literal, 0b1111011, 7), W), BYTE(MOD, BIT(Literal, 0b101, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("aam", "ASCII adjust for multiply", BYTE(BIT(Literal, 0b11010100, 8)), BYTE(BIT(Literal, 0b00001010, 8))), 
		ENCODING("div", "Divide (unsigned)", BYTE(BIT(Literal, 0b1111011, 7), W), BYTE(MOD, BIT(Literal, 0b110, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("idiv", "Integer divide (signed)", BYTE(BIT(Literal, 0b1111011, 7), W), BYTE(MOD, BIT(Literal, 0b111, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("aad", "ASCII adjust for divide", BYTE(BIT(Literal, 0b11010101, 8)), BYTE(BIT(Literal, 0b00001010, 8))), 

		ENCODING("cbw", "Convert byte to word", BYTE(BIT(Literal, 0b10011000, 8))),
		ENCODING("cwd", "Convert word to double word", BYTE(BIT(Literal, 0b10011001, 8))),

		//LOGIC

		ENCODING("not", "Invert", BYTE(BIT(Literal, 0b1111011, 7), W), BYTE(MOD, BIT(Literal, 0b010, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("shl", "Shift logical/arithmetic left", BYTE(BIT(Literal, 0b110100, 6), V, W), BYTE(MOD, BIT(Literal, 0b100, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("shr", "Shift logical right", BYTE(BIT(Literal, 0b110100, 6), V, W), BYTE(MOD, BIT(Literal, 0b101, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("sar", "Shift arithmetic right", BYTE(BIT(Literal, 0b110100, 6), V, W), BYTE(MOD, BIT(Literal, 0b111, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("rol", "Rotate left", BYTE(BIT(Literal, 0b110100, 6), V, W), BYTE(MOD, BIT(Literal, 0b000, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("ror", "Rotate left", BYTE(BIT(Literal, 0b110100, 6), V, W), BYTE(MOD, BIT(Literal, 0b001, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("rcl", "Rotate through carry flag left", BYTE(BIT(Literal, 0b110100, 6), V, W), BYTE(MOD, BIT(Literal, 0b010, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("rcr", "Rotate through carry right", BYTE(BIT(Literal, 0b110100, 6), V, W), BYTE(MOD, BIT(Literal, 0b011, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),

		ENCODING("and", "Reg/memory and register to either", BYTE(BIT(Literal, 0b001000, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("and", "Immediate from register/memory", BYTE(BIT(Literal, 0b100000, 6), S, W), BYTE(MOD, BIT(Literal, 0b100, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w)),
		ENCODING("and", "Immediate from accumulator", BYTE(BIT(Literal, 0b0010010, 7), W), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1), IMP_ACC)),

		ENCODING("test", "Reg/memory and register", BYTE(BIT(Literal, 0b100001, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("test", "Immediate data and register/memory", BYTE(BIT(Literal, 0b1111011, 7), W), BYTE(MOD, BIT(Literal, 0b000, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w)),
		ENCODING("test", "Immediate data and accumulator", BYTE(BIT(Literal, 0b1010100, 7), W), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1), IMP_ACC)), 

		ENCODING("or", "Reg/memory and register to either", BYTE(BIT(Literal, 0b000010, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("or", "Immediate to register/memory", BYTE(BIT(Literal, 0b1000000, 7), W), BYTE(MOD, BIT(Literal, 0b001, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w)),
		ENCODING("or", "Immediate to accumulator", BYTE(BIT(Literal, 0b0000110, 7), W), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1), IMP_ACC)), 

		ENCODING("xor", "Reg/memory and register to either", BYTE(BIT(Literal, 0b001100, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("xor", "Immediate to register/memory", BYTE(BIT(Literal, 0b1000000, 7), W), BYTE(MOD, BIT(Literal, 0b110, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w)), 
		ENCODING("xor", "Immediate to accumulator", BYTE(BIT(Literal, 0b0011010, 7), W), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1), IMP_ACC)), 

		// STRING MANIPULATION

		// REP
		ENCODING("rep", "Repeat", BYTE(BIT(Literal, 0b1111001, 7), Z), EMPTY_BYTE(IMP_PREFIX)),
		ENCODING("movs", "Move byte/word", BYTE(BIT(Literal, 0b1010010, 7), W), EMPTY_BYTE(IMP_POSTFIX)),
		ENCODING("cmps", "Compare byte/word", BYTE(BIT(Literal, 0b1010011, 7), W), EMPTY_BYTE(IMP_POSTFIX)),
		ENCODING("scas", "Scan byte/word", BYTE(BIT(Literal, 0b1010111, 7), W), EMPTY_BYTE(IMP_POSTFIX)),
		ENCODING("lods", "Load byte/wd to AL/AX", BYTE(BIT(Literal, 0b1010110, 7), W), EMPTY_BYTE(IMP_POSTFIX)),
		ENCODING("stos", "Store byte/wd from AL/AX", BYTE(BIT(Literal, 0b1010101, 7), W), EMPTY_BYTE(IMP_POSTFIX)),

		// CONTROL TRANSFER

		ENCODING("call", "Direct within segment", EMPTY_BYTE(IMP_W(1)), BYTE(BIT(Literal, 0b11101000, 8)), FULL_BYTE(IP_inc_lo), FULL_BYTE(IP_inc_hi)),
		ENCODING("call", "Indirect within segment", EMPTY_BYTE(IMP_W(1)), BYTE(BIT(Literal, 0b11111111, 8)), BYTE(MOD, BIT(Literal, 0b010, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("call", "Direct intersegment", EMPTY_BYTE(IMP_W(1), IMP_D(1)), BYTE(BIT(Literal, 0b10011010, 8)), FULL_BYTE(IP_lo), FULL_BYTE(IP_hi), FULL_BYTE(CS_lo), FULL_BYTE(CS_hi)),
		ENCODING("call", "Indirect intersegment", EMPTY_BYTE(IMP_W(1)), BYTE(BIT(Literal, 0b11111111, 8)), BYTE(MOD, BIT(Literal, 0b011, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),

		ENCODING("jmp", "Direct within segment", EMPTY_BYTE(IMP_W(1)), BYTE(BIT(Literal, 0b11101001, 8)), FULL_BYTE(IP_inc_lo), FULL_BYTE(IP_inc_hi)),
		ENCODING("jmp", "Direct within segment-short", BYTE(BIT(Literal, 0b11101011, 8)), FULL_BYTE(IP_lo)),
		ENCODING("jmp", "Indirect within segment", EMPTY_BYTE(IMP_W(1)), BYTE(BIT(Literal, 0b11111111, 8)), BYTE(MOD, BIT(Literal, 0b100, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING("jmp", "Direct intersegment", EMPTY_BYTE(IMP_W(1), IMP_D(1)), BYTE(BIT(Literal, 0b11101010, 8)), FULL_BYTE(IP_lo), FULL_BYTE(IP_hi), FULL_BYTE(CS_lo), FULL_BYTE(CS_hi)),
		ENCODING("jmp", "Indirect intersegment", EMPTY_BYTE(IMP_W(1)), BYTE(BIT(Literal, 0b11111111, 8)), BYTE(MOD, BIT(Literal, 0b101, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),

		ENCODING("ret", "Within segment", BYTE(BIT(Literal, 0b11000011, 8))),
		ENCODING("ret", "Within seg adding immed to SP", EMPTY_BYTE(IMP_W(1)), BYTE(BIT(Literal, 0b11000010, 8)), FULL_BYTE(Data), FULL_BYTE(Data_if_w)),
		ENCODING("ret", "Intersegment", BYTE(BIT(Literal, 0b11001011, 8))),
		ENCODING("ret", "Intersegment adding immediate to SP", EMPTY_BYTE(IMP_W(1)), BYTE(BIT(Literal, 0b11001010, 8)), FULL_BYTE(Data), FULL_BYTE(Data_if_w)),

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
		ENCODING("jns", "Jump on not sign", BYTE(BIT(Literal, 0b01111001, 8)),FULL_BYTE(Rel_offset)),
		ENCODING("loop", "Loop CX times", BYTE(BIT(Literal, 0b11100010, 8)),FULL_BYTE(Rel_offset)),
		ENCODING("loopz", "Loop while zero/equal", BYTE(BIT(Literal, 0b11100001, 8)),FULL_BYTE(Rel_offset)),
		ENCODING("loopnz", "Loop while not zero/equal", BYTE(BIT(Literal, 0b11100000, 8)),FULL_BYTE(Rel_offset)),
		ENCODING("jcxz", "Jump on CX zero", BYTE(BIT(Literal, 0b11100011, 8)),FULL_BYTE(Rel_offset)),

		ENCODING("int", "Type specified", BYTE(BIT(Literal, 0b11001101, 8)), FULL_BYTE(Data)),
		ENCODING("int3", "Type 3", BYTE(BIT(Literal, 0b11001100, 8))), //TODO: nasm requires 3, even though manual doesn't seem to
		ENCODING("into", "Interrupt on overflow", BYTE(BIT(Literal, 0b11001110, 8))),
		ENCODING("iret", "Interrupt return", BYTE(BIT(Literal, 0b11001111, 8))),
		
		// PROCESSOR CONTROL

		ENCODING("clc", "Clear carry", BYTE(BIT(Literal, 0b11111000, 8))),
		ENCODING("cmc", "Complement carry", BYTE(BIT(Literal, 0b11110101, 8))),
		ENCODING("stc", "Set carry", BYTE(BIT(Literal, 0b11111001, 8))),
		ENCODING("cld", "Clear direction", BYTE(BIT(Literal, 0b11111100, 8))),
		ENCODING("std", "Set direction", BYTE(BIT(Literal, 0b11111101, 8))),
		ENCODING("cli", "Clear interrupt", BYTE(BIT(Literal, 0b11111010, 8))),
		ENCODING("sti", "Set interrupt", BYTE(BIT(Literal, 0b11111011, 8))),
		ENCODING("hlt", "Halt", BYTE(BIT(Literal, 0b11110100, 8))),
		ENCODING("wait", "Wait", BYTE(BIT(Literal, 0b10011011, 8))),

		// TODO: ESC

		ENCODING("lock", "Bus lock prefix", BYTE(BIT(Literal, 0b11110000, 8)), EMPTY_BYTE(IMP_PREFIX)),

		ENCODING("segment", "Override prefix", BYTE(BIT(Literal, 0b001, 3), SR, BIT(Literal, 0b110, 3)), EMPTY_BYTE(IMP_SR_PREFIX)),
	};
	
	return all_instructions;

}

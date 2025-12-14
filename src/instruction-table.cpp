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
#define IMP_FAR BIT(Imp_Far, 0, 0)


std::vector<Instruction_encoding> get_all_instructions()
{
	std::vector<Instruction_encoding> all_instructions = {
		// DATA TRANSFER
		ENCODING(Mov, "Register/memory to/from register", BYTE(BIT(Literal, 0b100010, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Mov, "Immediate to register/memory", BYTE(BIT(Literal, 0b1100011, 7), W), BYTE(MOD, BIT(Literal, 0b000, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(0))),
		ENCODING(Mov, "Immediate to register", BYTE(BIT(Literal, 0b1011, 4), W, REG), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1))),
		ENCODING(Mov, "Memory to accumulator", BYTE(BIT(Literal, 0b1010000, 7), W), FULL_BYTE(Addr_lo), FULL_BYTE(Addr_hi), EMPTY_BYTE(IMP_D(1), IMP_ACC)),
		ENCODING(Mov, "Accumulator to memory", BYTE(BIT(Literal, 0b1010001, 7), W), FULL_BYTE(Addr_lo), FULL_BYTE(Addr_hi), EMPTY_BYTE(IMP_D(0), IMP_ACC)),
		ENCODING(Mov, "Register/memory to segment register", EMPTY_BYTE(IMP_W(1)), BYTE(BIT(Literal, 0b10001110, 8)), BYTE(MOD, BIT(Literal, 0b0, 1), SR, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Mov, "Segment register to register/memory", EMPTY_BYTE(IMP_W(1)), BYTE(BIT(Literal, 0b10001100, 8)), BYTE(MOD, BIT(Literal, 0b0, 1), SR, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		
		ENCODING(Push, "Register/memory", EMPTY_BYTE(IMP_D(0), IMP_W(1)), BYTE(BIT(Literal, 0b11111111, 8)), BYTE(MOD, BIT(Literal, 0b110, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Push, "Register", EMPTY_BYTE(IMP_D(1), IMP_W(1)), BYTE(BIT(Literal, 0b01010, 5), REG)),
		ENCODING(Push, "Segment register", BYTE(BIT(Literal, 0b000, 3), SR, BIT(Literal, 0b110, 3)), EMPTY_BYTE(IMP_D(1))),

		ENCODING(Pop, "Register/memory", EMPTY_BYTE(IMP_D(0), IMP_W(1)), BYTE(BIT(Literal, 0b10001111, 8)), BYTE(MOD, BIT(Literal, 0b000, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Pop, "Register", EMPTY_BYTE(IMP_D(1), IMP_W(1)), BYTE(BIT(Literal, 0b01011, 5), REG)),
		ENCODING(Pop, "Segment register", BYTE(BIT(Literal, 0b000, 3), SR, BIT(Literal, 0b111, 3)), EMPTY_BYTE(IMP_D(1))),

		ENCODING(Xchg, "Register/memory with register", BYTE(BIT(Literal, 0b1000011, 7), W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), EMPTY_BYTE(IMP_D(1))),
		ENCODING(Xchg, "Register with accumulator", EMPTY_BYTE(IMP_W(1), IMP_D(1)), BYTE(BIT(Literal, 0b10010, 5), REG), EMPTY_BYTE(IMP_ACC)),

		ENCODING(In, "Fixed port", BYTE(BIT(Literal, 0b1110010, 7), W), FULL_BYTE(Port), EMPTY_BYTE(IMP_D(1))),
		ENCODING(In, "Variable port", BYTE(BIT(Literal, 0b1110110, 7), W), EMPTY_BYTE(IMP_PORT, IMP_D(1))),

		ENCODING(Out, "Fixed port", BYTE(BIT(Literal, 0b1110011, 7), W), FULL_BYTE(Port), EMPTY_BYTE(IMP_D(0))),
		ENCODING(Out, "Variable port", BYTE(BIT(Literal, 0b1110111, 7), W), EMPTY_BYTE(IMP_PORT, IMP_D(0))),

		ENCODING(Xlat, "Translate byte to AL", BYTE(BIT(Literal, 0b11010111, 8))),

		ENCODING(Lea, "Load EA to register", EMPTY_BYTE(IMP_D(1), IMP_W(1)), BYTE(BIT(Literal, 0b10001101, 8)), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Lds, "Load pointer to DS", EMPTY_BYTE(IMP_D(1), IMP_W(1)), BYTE(BIT(Literal, 0b11000101, 8)), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Les, "Load pointer to ES", EMPTY_BYTE(IMP_D(1), IMP_W(1)), BYTE(BIT(Literal, 0b11000100, 8)), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),

		ENCODING(Lahf, "Load AH with flags", BYTE(BIT(Literal, 0b10011111, 8))),
		ENCODING(Sahf, "Store AH into flags", BYTE(BIT(Literal, 0b10011110, 8))),
		ENCODING(Pushf, "Push flags", BYTE(BIT(Literal, 0b10011100, 8))),
		ENCODING(Popf, "Pop flags", BYTE(BIT(Literal, 0b10011101, 8))),

		// ARITHEMIC
		
		ENCODING(Add, "Reg/memory with register to either", BYTE(BIT(Literal, 0b000000, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Add, "Immediate to register/memory", BYTE(BIT(Literal, 0b100000, 6), S, W), BYTE(MOD, BIT(Literal, 0b000, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(0))),
		ENCODING(Add, "Immediate to accumulator", BYTE(BIT(Literal, 0b0000010, 7), W), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1), IMP_ACC)),

		ENCODING(Adc, "Reg/memory with register to either", BYTE(BIT(Literal, 0b000100, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Adc, "Immediate to register/memory", BYTE(BIT(Literal, 0b100000, 6), S, W), BYTE(MOD, BIT(Literal, 0b010, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(0))),
		ENCODING(Adc, "Immediate to accumulator", BYTE(BIT(Literal, 0b0001010, 7), W), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1), IMP_ACC)),

		ENCODING(Inc, "Register/memory", BYTE(BIT(Literal, 0b1111111, 7), W), BYTE(MOD, BIT(Literal, 0b000, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Inc, "Register", EMPTY_BYTE(IMP_D(1), IMP_W(1)), BYTE(BIT(Literal, 0b01000, 5), REG)),

		ENCODING(Aaa, "ASCII adjust for add", BYTE(BIT(Literal, 0b00110111, 8))),
		ENCODING(Daa, "Decimal adjust for add", BYTE(BIT(Literal, 0b00100111, 8))),
		
		ENCODING(Sub, "Reg/memory and register to either", BYTE(BIT(Literal, 0b001010, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Sub, "Immediate from register/memory", BYTE(BIT(Literal, 0b100000, 6), S, W), BYTE(MOD, BIT(Literal, 0b101, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w)),
		ENCODING(Sub, "Immediate from accumulator", BYTE(BIT(Literal, 0b0010110, 7), W), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1), IMP_ACC)),
		
		ENCODING(Sbb, "Reg/memory and register to either", BYTE(BIT(Literal, 0b000110, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Sbb, "Immediate from register/memory", BYTE(BIT(Literal, 0b100000, 6), S, W), BYTE(MOD, BIT(Literal, 0b011, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w)),
		ENCODING(Sbb, "Immediate from accumulator", BYTE(BIT(Literal, 0b0001110, 7), W), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1), IMP_ACC)),

		ENCODING(Dec, "Register/memory", BYTE(BIT(Literal, 0b1111111, 7), W), BYTE(MOD, BIT(Literal, 0b001, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Dec, "Register", EMPTY_BYTE(IMP_D(1), IMP_W(1)), BYTE(BIT(Literal, 0b01001, 5), REG)),

		ENCODING(Neg, "Change sign", BYTE(BIT(Literal, 0b1111011, 7), W), BYTE(MOD, BIT(Literal, 0b011, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),

		ENCODING(Cmp, "Register/memory and register", BYTE(BIT(Literal, 0b001110, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Cmp, "Immediate with register/memory", BYTE(BIT(Literal, 0b100000, 6), S, W), BYTE(MOD, BIT(Literal, 0b111, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(0))),
		ENCODING(Cmp, "Immediate with accumulator", BYTE(BIT(Literal, 0b0011110, 7), W), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1), IMP_ACC)),

		ENCODING(Aas, "ASCII adjust for subtract", BYTE(BIT(Literal, 0b00111111, 8))),
		ENCODING(Das, "Decimal adjust for subtract", BYTE(BIT(Literal, 0b00101111, 8))),

		ENCODING(Mul, "Multiply (unsigned)", BYTE(BIT(Literal, 0b1111011, 7), W), BYTE(MOD, BIT(Literal, 0b100, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Imul, "Integer multiply (signed)", BYTE(BIT(Literal, 0b1111011, 7), W), BYTE(MOD, BIT(Literal, 0b101, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Aam, "ASCII adjust for multiply", BYTE(BIT(Literal, 0b11010100, 8)), BYTE(BIT(Literal, 0b00001010, 8))), 
		ENCODING(Div, "Divide (unsigned)", BYTE(BIT(Literal, 0b1111011, 7), W), BYTE(MOD, BIT(Literal, 0b110, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Idiv, "Integer divide (signed)", BYTE(BIT(Literal, 0b1111011, 7), W), BYTE(MOD, BIT(Literal, 0b111, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Aad, "ASCII adjust for divide", BYTE(BIT(Literal, 0b11010101, 8)), BYTE(BIT(Literal, 0b00001010, 8))), 

		ENCODING(Cbw, "Convert byte to word", BYTE(BIT(Literal, 0b10011000, 8))),
		ENCODING(Cwd, "Convert word to double word", BYTE(BIT(Literal, 0b10011001, 8))),

		//LOGIC

		ENCODING(Not, "Invert", BYTE(BIT(Literal, 0b1111011, 7), W), BYTE(MOD, BIT(Literal, 0b010, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Shl_Sal, "Shift logical/arithmetic left", BYTE(BIT(Literal, 0b110100, 6), V, W), BYTE(MOD, BIT(Literal, 0b100, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Shr, "Shift logical right", BYTE(BIT(Literal, 0b110100, 6), V, W), BYTE(MOD, BIT(Literal, 0b101, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Sar, "Shift arithmetic right", BYTE(BIT(Literal, 0b110100, 6), V, W), BYTE(MOD, BIT(Literal, 0b111, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Rol, "Rotate left", BYTE(BIT(Literal, 0b110100, 6), V, W), BYTE(MOD, BIT(Literal, 0b000, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Ror, "Rotate left", BYTE(BIT(Literal, 0b110100, 6), V, W), BYTE(MOD, BIT(Literal, 0b001, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Rcl, "Rotate through carry flag left", BYTE(BIT(Literal, 0b110100, 6), V, W), BYTE(MOD, BIT(Literal, 0b010, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Rcr, "Rotate through carry right", BYTE(BIT(Literal, 0b110100, 6), V, W), BYTE(MOD, BIT(Literal, 0b011, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),

		ENCODING(And, "Reg/memory and register to either", BYTE(BIT(Literal, 0b001000, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(And, "Immediate from register/memory", BYTE(BIT(Literal, 0b100000, 6), S, W), BYTE(MOD, BIT(Literal, 0b100, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w)),
		ENCODING(And, "Immediate from accumulator", BYTE(BIT(Literal, 0b0010010, 7), W), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1), IMP_ACC)),

		ENCODING(Test, "Reg/memory and register", BYTE(BIT(Literal, 0b100001, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Test, "Immediate data and register/memory", BYTE(BIT(Literal, 0b1111011, 7), W), BYTE(MOD, BIT(Literal, 0b000, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w)),
		ENCODING(Test, "Immediate data and accumulator", BYTE(BIT(Literal, 0b1010100, 7), W), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1), IMP_ACC)), 

		ENCODING(Or, "Reg/memory and register to either", BYTE(BIT(Literal, 0b000010, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Or, "Immediate to register/memory", BYTE(BIT(Literal, 0b1000000, 7), W), BYTE(MOD, BIT(Literal, 0b001, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w)),
		ENCODING(Or, "Immediate to accumulator", BYTE(BIT(Literal, 0b0000110, 7), W), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1), IMP_ACC)), 

		ENCODING(Xor, "Reg/memory and register to either", BYTE(BIT(Literal, 0b001100, 6), D, W), BYTE(MOD, REG, RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Xor, "Immediate to register/memory", BYTE(BIT(Literal, 0b1000000, 7), W), BYTE(MOD, BIT(Literal, 0b110, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi), FULL_BYTE(Data), FULL_BYTE(Data_if_w)), 
		ENCODING(Xor, "Immediate to accumulator", BYTE(BIT(Literal, 0b0011010, 7), W), FULL_BYTE(Data), FULL_BYTE(Data_if_w), EMPTY_BYTE(IMP_D(1), IMP_ACC)), 

		// STRING MANIPULATION

		ENCODING(Rep, "Repeat", BYTE(BIT(Literal, 0b1111001, 7), Z), EMPTY_BYTE(IMP_PREFIX)),
		ENCODING(Movs, "Move byte/word", BYTE(BIT(Literal, 0b1010010, 7), W), EMPTY_BYTE(IMP_POSTFIX)),
		ENCODING(Cmps, "Compare byte/word", BYTE(BIT(Literal, 0b1010011, 7), W), EMPTY_BYTE(IMP_POSTFIX)),
		ENCODING(Scas, "Scan byte/word", BYTE(BIT(Literal, 0b1010111, 7), W), EMPTY_BYTE(IMP_POSTFIX)),
		ENCODING(Lods, "Load byte/wd to AL/AX", BYTE(BIT(Literal, 0b1010110, 7), W), EMPTY_BYTE(IMP_POSTFIX)),
		ENCODING(Stos, "Store byte/wd from AL/AX", BYTE(BIT(Literal, 0b1010101, 7), W), EMPTY_BYTE(IMP_POSTFIX)),

		// CONTROL TRANSFER

		ENCODING(Call, "Direct within segment", EMPTY_BYTE(IMP_W(1)), BYTE(BIT(Literal, 0b11101000, 8)), FULL_BYTE(IP_inc_lo), FULL_BYTE(IP_inc_hi)),
		ENCODING(Call, "Indirect within segment", EMPTY_BYTE(IMP_W(1)), BYTE(BIT(Literal, 0b11111111, 8)), BYTE(MOD, BIT(Literal, 0b010, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Call, "Direct intersegment", EMPTY_BYTE(IMP_W(1), IMP_D(1)), BYTE(BIT(Literal, 0b10011010, 8)), FULL_BYTE(IP_lo), FULL_BYTE(IP_hi), FULL_BYTE(CS_lo), FULL_BYTE(CS_hi)),
		ENCODING(Call, "Indirect intersegment", EMPTY_BYTE(IMP_W(1), IMP_FAR), BYTE(BIT(Literal, 0b11111111, 8)), BYTE(MOD, BIT(Literal, 0b011, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),

		ENCODING(Jmp, "Direct within segment", EMPTY_BYTE(IMP_W(1)), BYTE(BIT(Literal, 0b11101001, 8)), FULL_BYTE(IP_inc_lo), FULL_BYTE(IP_inc_hi)),
		ENCODING(Jmp, "Direct within segment-short", BYTE(BIT(Literal, 0b11101011, 8)), FULL_BYTE(IP_lo)),
		ENCODING(Jmp, "Indirect within segment", EMPTY_BYTE(IMP_W(1)), BYTE(BIT(Literal, 0b11111111, 8)), BYTE(MOD, BIT(Literal, 0b100, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),
		ENCODING(Jmp, "Direct intersegment", EMPTY_BYTE(IMP_W(1), IMP_D(1)), BYTE(BIT(Literal, 0b11101010, 8)), FULL_BYTE(IP_lo), FULL_BYTE(IP_hi), FULL_BYTE(CS_lo), FULL_BYTE(CS_hi)),
		ENCODING(Jmp, "Indirect intersegment", EMPTY_BYTE(IMP_W(1), IMP_FAR), BYTE(BIT(Literal, 0b11111111, 8)), BYTE(MOD, BIT(Literal, 0b101, 3), RM), FULL_BYTE(Disp_lo), FULL_BYTE(Disp_hi)),

		ENCODING(Ret, "Within segment", BYTE(BIT(Literal, 0b11000011, 8))),
		ENCODING(Ret, "Within seg adding immed to SP", EMPTY_BYTE(IMP_W(1)), BYTE(BIT(Literal, 0b11000010, 8)), FULL_BYTE(Data), FULL_BYTE(Data_if_w)),
		ENCODING(Retf, "Intersegment", BYTE(BIT(Literal, 0b11001011, 8))),
		ENCODING(Retf, "Intersegment adding immediate to SP", EMPTY_BYTE(IMP_W(1)), BYTE(BIT(Literal, 0b11001010, 8)), FULL_BYTE(Data), FULL_BYTE(Data_if_w)),

		ENCODING(Je_Jz, "Jump on equal/zero", BYTE(BIT(Literal, 0b01110100, 8)),FULL_BYTE(Rel_offset)),
		ENCODING(Jl_Jnge, "Jump on less/not greater or equal", BYTE(BIT(Literal, 0b01111100, 8)),FULL_BYTE(Rel_offset)),
		ENCODING(Jle_Jng, "Jump on less/or equal/not greater", BYTE(BIT(Literal, 0b01111110, 8)),FULL_BYTE(Rel_offset)),
		ENCODING(Jb_Jnae, "Jump on below/not above or equal", BYTE(BIT(Literal, 0b01110010, 8)),FULL_BYTE(Rel_offset)),
		ENCODING(Jbe_Jna, "Jump on below or equal/not above", BYTE(BIT(Literal, 0b01110110, 8)),FULL_BYTE(Rel_offset)),
		ENCODING(Jp_Jpe, "Jump on below parity/parity even", BYTE(BIT(Literal, 0b01111010, 8)),FULL_BYTE(Rel_offset)),
		ENCODING(Jo, "Jump on overflow", BYTE(BIT(Literal, 0b01110000, 8)),FULL_BYTE(Rel_offset)),
		ENCODING(Js, "Jump on sign", BYTE(BIT(Literal, 0b01111000, 8)),FULL_BYTE(Rel_offset)),
		ENCODING(Jne_Jnz, "Jump on not equal/not zero", BYTE(BIT(Literal, 0b01110101, 8)),FULL_BYTE(Rel_offset)),
		ENCODING(Jnl_Jge, "Jump on not less/greater or equal", BYTE(BIT(Literal, 0b01111101, 8)),FULL_BYTE(Rel_offset)),
		ENCODING(Jnle_Jg, "Jump on not less or equal/greater", BYTE(BIT(Literal, 0b01111111, 8)),FULL_BYTE(Rel_offset)),
		ENCODING(Jnb_Jae, "Jump on not below/above or equal", BYTE(BIT(Literal, 0b01110011, 8)),FULL_BYTE(Rel_offset)),
		ENCODING(Jnbe_Ja, "Jump on not below or equal/above", BYTE(BIT(Literal, 0b01110111, 8)),FULL_BYTE(Rel_offset)),
		ENCODING(Jnp_Jpo, "Jump on not par/par odd", BYTE(BIT(Literal, 0b01111011, 8)),FULL_BYTE(Rel_offset)),
		ENCODING(Jno, "Jump on not overflow", BYTE(BIT(Literal, 0b01110001, 8)),FULL_BYTE(Rel_offset)),
		ENCODING(Jns, "Jump on not sign", BYTE(BIT(Literal, 0b01111001, 8)),FULL_BYTE(Rel_offset)),
		ENCODING(Loop, "Loop CX times", BYTE(BIT(Literal, 0b11100010, 8)),FULL_BYTE(Rel_offset)),
		ENCODING(Loopz_Loope, "Loop while zero/equal", BYTE(BIT(Literal, 0b11100001, 8)),FULL_BYTE(Rel_offset)),
		ENCODING(Loopnz_Loopne, "Loop while not zero/equal", BYTE(BIT(Literal, 0b11100000, 8)),FULL_BYTE(Rel_offset)),
		ENCODING(Jcxz, "Jump on CX zero", BYTE(BIT(Literal, 0b11100011, 8)),FULL_BYTE(Rel_offset)),

		ENCODING(Int, "Type specified", BYTE(BIT(Literal, 0b11001101, 8)), FULL_BYTE(Data)),
		ENCODING(Int3, "Type 3", BYTE(BIT(Literal, 0b11001100, 8))), //TODO: nasm requires 3, even though manual doesn't seem to
		ENCODING(Into, "Interrupt on overflow", BYTE(BIT(Literal, 0b11001110, 8))),
		ENCODING(Iret, "Interrupt return", BYTE(BIT(Literal, 0b11001111, 8))),
		
		// PROCESSOR CONTROL

		ENCODING(Clc, "Clear carry", BYTE(BIT(Literal, 0b11111000, 8))),
		ENCODING(Cmc, "Complement carry", BYTE(BIT(Literal, 0b11110101, 8))),
		ENCODING(Stc, "Set carry", BYTE(BIT(Literal, 0b11111001, 8))),
		ENCODING(Cld, "Clear direction", BYTE(BIT(Literal, 0b11111100, 8))),
		ENCODING(Std, "Set direction", BYTE(BIT(Literal, 0b11111101, 8))),
		ENCODING(Cli, "Clear interrupt", BYTE(BIT(Literal, 0b11111010, 8))),
		ENCODING(Sti, "Set interrupt", BYTE(BIT(Literal, 0b11111011, 8))),
		ENCODING(Hlt, "Halt", BYTE(BIT(Literal, 0b11110100, 8))),
		ENCODING(Wait, "Wait", BYTE(BIT(Literal, 0b10011011, 8))),

		// TODO: ESC

		ENCODING(Lock, "Bus lock prefix", BYTE(BIT(Literal, 0b11110000, 8)), EMPTY_BYTE(IMP_PREFIX)),

		ENCODING(Segment, "Override prefix", BYTE(BIT(Literal, 0b001, 3), SR, BIT(Literal, 0b110, 3)), EMPTY_BYTE(IMP_SR_PREFIX)),
	};
	
	return all_instructions;

}

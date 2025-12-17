#include "simulator.hpp"
#include "instruction-table.hpp"
#include "instruction.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>

uint16_t lowRegMask = 0x00FF;
uint16_t highRegMask = 0xFF00;

uint16_t read(Operand operand, struct Memory *memory) {
  switch (operand.type) {
  case Register:
		switch (operand.reg) {
			case Register::Al:
				return memory->registers[Register::Ax] & 0x00FF;
			case Register::Ah:
				return memory->registers[Register::Ax] >> 8;
			case Register::Bl:
				return memory->registers[Register::Bx] & 0x00FF;
			case Register::Bh:
				return memory->registers[Register::Bx] >> 8;
			case Register::Cl:
				return memory->registers[Register::Cx] & 0x00FF;
			case Register::Ch:
				return memory->registers[Register::Cx] >> 8;
			case Register::Dl:
				return memory->registers[Register::Dx] & 0x00FF;
			case Register::Dh:
				return memory->registers[Register::Dx] >> 8;
			default:
				return memory->registers[operand.reg];
		}
  case Memory:
  case Immediate:
    return operand.immediate;
  case Direct_address:
  case Relative_offset:
  case No_op_type:
    throw std::runtime_error("Operand type not set");
  }
  throw std::runtime_error("Unknown operand type");
}

void write(Operand operand, struct Memory *memory, uint16_t value) {

  switch (operand.type) {
  case Register:
		switch (operand.reg) {
			case Register::Al:
				memory->registers[Register::Ax] &= 0xFF00;
				memory->registers[Register::Ax] |= value;
				break;
			case Register::Ah:
				memory->registers[Register::Ax] &= 0x00FF;
				memory->registers[Register::Ax] |= (value << 8);
				break;
			case Register::Bl:
				memory->registers[Register::Bx] &= 0xFF00;
				memory->registers[Register::Bx] |= value;
				break;
			case Register::Bh:
				memory->registers[Register::Bx] &= 0x00FF;
				memory->registers[Register::Bx] |= (value << 8);
				break;
			case Register::Cl:
				memory->registers[Register::Cx] &= 0xFF00;
				memory->registers[Register::Cx] |= value;
				break;
			case Register::Ch:
				memory->registers[Register::Cx] &= 0x00FF;
				memory->registers[Register::Cx] |= (value << 8);
				break;
			case Register::Dl:
				memory->registers[Register::Dx] &= 0xFF00;
				memory->registers[Register::Dx] |= value;
				break;
			case Register::Dh:
				memory->registers[Register::Dx] &= 0x00FF;
				memory->registers[Register::Dx] |= (value << 8);
				break;
			default:
				memory->registers[operand.reg] = value;
				break;
		}
		return;
  case Memory:
  case Immediate:
    throw std::runtime_error("Not possible to write to immediate");
  case Direct_address:
  case Relative_offset:
  case No_op_type:
    throw std::runtime_error("Operand type not set");
  }
  throw std::runtime_error("Unknown operand type");
}

struct Memory simulate(std::vector<Instruction> instructions) 
{

	std::map<int, Instruction> ip_lookup;

	for (Instruction instruction : instructions) {
		ip_lookup[instruction.instruction_pointer] = instruction;
	}

  struct Memory memory;

	Instruction first_instruction = instructions.at(0);
	Instruction last_instruction = instructions.at(instructions.size() - 1);

	memory.instruction_pointer = first_instruction.instruction_pointer;
	int end_ip = last_instruction.instruction_pointer + last_instruction.processed_bytes.size();

  while (memory.instruction_pointer < end_ip) {
		Instruction instruction = ip_lookup[memory.instruction_pointer];
		memory.instruction_pointer += instruction.processed_bytes.size();
    std::ostringstream output;
    output << instruction.to_string() << " ; ";

		bool z_flag = false;
		bool s_flag = false;

    switch (instruction.name) {
			case Mov: {
				uint16_t src_value = read(instruction.operands[1], &memory);
				uint16_t dest_value = read(instruction.operands[0], &memory);
				write(instruction.operands[0], &memory, src_value);
				output << instruction.operands[0].to_string() << ": ";
				output << "0x" << std::hex << dest_value << "->0x" << std::hex << src_value;
				break;
			}
			case Sub: {
				uint16_t src_value = read(instruction.operands[1], &memory);
				uint16_t dest_value = read(instruction.operands[0], &memory);
				uint16_t result = dest_value - src_value;
				write(instruction.operands[0], &memory, result);
				output << instruction.operands[0].to_string() << ": ";
				output << "0x" << std::hex << dest_value << "->0x" << std::hex << result;
				if ((result & 0x8000) > 0) {
					s_flag = true;
					output << " S_flag";
				}
				if (result == 0) {
					z_flag = true;
					output << " Z_flag";
				}
				break;
			}
			case Cmp: {
				uint16_t src_value = read(instruction.operands[1], &memory);
				uint16_t dest_value = read(instruction.operands[0], &memory);
				uint16_t result = dest_value - src_value;
				output << " Result" << result;
				if ((result & 0x8000) > 0) {
					s_flag = true;
					output << " S_flag";
				}
				if (result == 0) {
					z_flag = true;
					output << " Z_flag";
				}
				break;
			}
			case Add: {
				uint16_t src_value = read(instruction.operands[1], &memory);
				uint16_t dest_value = read(instruction.operands[0], &memory);
				uint16_t result = dest_value + src_value;
				write(instruction.operands[0], &memory, result);
				output << instruction.operands[0].to_string() << ": ";
				output << "0x" << std::hex << dest_value << "->0x" << std::hex << result;
				break;
			}
			case Jne_Jnz: {
				if (!memory.z_flag) {
					memory.instruction_pointer += instruction.operands[0].displacement;
				}
				break;
			}
		}

		memory.s_flag = s_flag;
		memory.z_flag = z_flag;
    std::cout << output.str() << std::endl;
  }

	std::cout << "\nFinal registers: " << std::endl;
	std::cout << get_register_name(Register::Ax) << " 0x" << std::hex << memory.registers[Register::Ax] << '\n';
	std::cout << get_register_name(Register::Bx) << " 0x" << std::hex << memory.registers[Register::Bx] << '\n';
	std::cout << get_register_name(Register::Cx) << " 0x" << std::hex << memory.registers[Register::Cx] << '\n';
	std::cout << get_register_name(Register::Dx) << " 0x" << std::hex << memory.registers[Register::Dx] << '\n';
	std::cout << get_register_name(Register::Sp) << " 0x" << std::hex << memory.registers[Register::Sp] << '\n';
	std::cout << get_register_name(Register::Bp) << " 0x" << std::hex << memory.registers[Register::Bp] << '\n';
	std::cout << get_register_name(Register::Si) << " 0x" << std::hex << memory.registers[Register::Si] << '\n';
	std::cout << get_register_name(Register::Di) << " 0x" << std::hex << memory.registers[Register::Di] << '\n';
	std::cout << get_register_name(Register::Es) << " 0x" << std::hex << memory.registers[Register::Es] << '\n';
	std::cout << get_register_name(Register::Cs) << " 0x" << std::hex << memory.registers[Register::Cs] << '\n';
	std::cout << get_register_name(Register::Ss) << " 0x" << std::hex << memory.registers[Register::Ss] << '\n';
	std::cout << get_register_name(Register::Ds) << " 0x" << std::hex << memory.registers[Register::Ds] << '\n';

	std::cout << "\nFlags: " << std::endl;
	std::cout << "S: " << memory.s_flag << '\n';
	std::cout << "Z: " << memory.z_flag << '\n';
  return memory;
}

#pragma once

#include "instruction.hpp"
#include <cstdint>
#include <map>
#include <array>


struct Memory {
	Memory() {
		memory_map.fill(0);
	}
	std::map<enum Register, uint16_t> registers;
	bool s_flag; //sign flag
	bool z_flag; //zero flag
	int instruction_pointer;

	std::array<uint8_t, 0xFFFF> memory_map; // TODO: Actually the 8086 has 1MB of memory, but for addressing to work, segment registers needs to be used
};

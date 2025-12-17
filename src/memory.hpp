#pragma once

#include "instruction.hpp"
#include <cstdint>
#include <map>


struct Memory {
	std::map<enum Register, uint16_t> registers;
	bool s_flag; //sign flag
	bool z_flag; //zero flag
};

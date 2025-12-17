#pragma once

#include "instruction.hpp"
#include <cstdint>
#include <map>


struct Memory {
	std::map<enum Register, uint16_t> registers;
};

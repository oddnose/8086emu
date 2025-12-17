#pragma once

#include "instruction.hpp"
#include "memory.hpp"
#include <vector>

struct Memory simulate(std::vector<Instruction> instructions);

#pragma once
#include "instruction.hpp"
#include "instruction-table.hpp"
#include <optional>
#include <string>
#include <vector>

std::optional<Instruction> decode_instruction(std::vector<unsigned char> data, size_t offset, Instruction_encoding encoding);
std::vector<Instruction> decode_asm_file(const std::string &path);

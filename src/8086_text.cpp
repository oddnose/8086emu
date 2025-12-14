#include "8086_text.h"
#include <sstream>

std::string get_print_instructions(std::vector<Instruction> instructions) 
{
	std::ostringstream output;
	output << "bits 16" << "\n";

	for (Instruction inst : instructions) {
		output << inst.to_string() << "\n";
	}
	return output.str();
}

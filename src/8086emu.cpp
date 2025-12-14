#include "8086_text.h"
#include "decoder.h"
#include <cstring>
#include <iostream>


int main(int arg_count, char **args)
{
	if (arg_count == 1) {
		std::cerr << "Usage: " << args[0] << " <8086 binary file>" << std::endl;
		std::cerr << "Flags:" << std::endl;
		std::cerr << "-d: print debug info" << std::endl;
		std::exit(1);
	}

	std::vector<Instruction> instructions;
	bool debug = false;

	for (int arg_index = 1; arg_index < arg_count; arg_index++) {
		char* arg = args[arg_index];

		if (strcmp(arg, "-d") == 0) {
			debug = true;
		}
		else {
			instructions = decode_asm_file(arg);
		}
	}

	if (instructions.empty()) {
		std::cerr << "No binary file provided" << std::endl;
		std::exit(1);
	}

	if (debug) {
		for (Instruction inst : instructions) {
			inst.print_debug();
			std::cout << "\n";
		}
		std::exit(0);
	}

	std::cout << get_print_instructions(instructions) << std::endl;
}

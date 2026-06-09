#include "8086_text.h"
#include "decoder.h"
#include "simulator.hpp"
#include <cstring>
#include <iostream>
#include <stdio.h>


int main(int arg_count, char **args)
{
	if (arg_count == 1) {
		std::cerr << "Usage: " << args[0] << " <8086 binary file>" << std::endl;
		std::cerr << "Flags:" << std::endl;
		std::cerr << "-d: print debug info" << std::endl;
		std::cerr << "-s: run simulation and print out memory state after program. Also dumps all memory to memory.data" << std::endl;
		std::exit(1);
	}

	std::vector<Instruction> instructions;
	bool debug = false;
	bool run_simulate = false;

	for (int arg_index = 1; arg_index < arg_count; arg_index++) {
		char* arg = args[arg_index];

		if (strcmp(arg, "-d") == 0) {
			debug = true;
		} else if (strcmp(arg, "-s") == 0) {
			run_simulate = true;
		} else {
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
	} else {
		std::cout << get_print_instructions(instructions) << std::endl;
	}

	if (run_simulate) {
		std::cout << "Simulation:" << std::endl;
		struct Memory memory = simulate(instructions);
		FILE *file = fopen("memory.data", "wb");
		std::fwrite(memory.memory_map.data(), sizeof memory.memory_map, 1, file);
		fclose(file);

	}

}

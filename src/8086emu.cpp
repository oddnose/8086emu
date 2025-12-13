#include "8086_text.h"
#include "decoder.h"
#include <iostream>


int main(int arg_count, char **args)
{
	if (arg_count == 1) {
		std::cerr << "Usage: " << args[0] << " <8086 binary file>" << std::endl;
	}

	std::cout << get_print_instructions(decode_asm_file(args[1])) << std::endl;
}

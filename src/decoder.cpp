
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>


const char Mov_instruction = 0b100010; 

const std::string reg_names_8bit[] = { "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
const std::string reg_names_16bit[] = { "ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};

std::string decode_asm_file(const std::string& path)
{
    std::ifstream asm_file(path, std::ios::binary);
    if (!asm_file) {
        throw std::runtime_error("Failed to open asm file");
    }
    std::vector<unsigned char> data((std::istream_iterator<char>(asm_file)), {});

    std::stringstream output;
    output << ';' << path << " disassembly:" << std::endl;
    output << "bits 16" << std::endl;

    size_t offset = 0;
    while (offset < data.size()) {
        char op_code = data[offset] >> 2; 

        switch (op_code) {
            case Mov_instruction:
                output << "mov ";
                bool direction = (data[offset] & 0b10) >> 1; // 0 = reg field is source for mov, 1 = reg field is destination
                bool word = data[offset] & 0b1; // 0 = move 1 byte, 1 = move 2 bytes

                offset++;

                char mod = data[offset] >> 6;
                char reg = (data[offset] & 0b111000) >> 3;
                char rm = data[offset] & 0b111;
                
                if (word) {
                    output << reg_names_16bit[direction ? reg : rm] << ", ";
                    output << reg_names_16bit[direction ? rm : reg];
                } 
                else {
                    output << reg_names_8bit[direction ? reg : rm] << ", ";
                    output << reg_names_8bit[direction ? rm : reg];
                }
                output << std::endl;
                offset++;
                break;
        }
    }

    return output.str();
}

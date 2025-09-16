#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>


const std::string reg_names_8bit[] = { 
  "al", 
  "cl", 
  "dl", 
  "bl",
  "ah", 
  "ch", 
  "dh", 
  "bh"
};
const std::string reg_names_16bit[] = { 
  "ax", // ah and al
  "cx", // ch and cl
  "dx", // dh and dl
  "bx", // bh and bl
  "sp", // stack pointer
  "bp", // base pointer
  "si", // source index
  "di" // des index
};

const std::string rm_field_encodings[] = {
  "bx + si",
  "bx + di",
  "bp + si",
  "bp + di",
  "si",
  "di",
  "bp", //not used when mod 00
  "bx"
}; 

enum Operand_type {
    Register,
    Memory,
    Immediate,
    Direct_address
};
enum Operand_size {
    Byte,
    Word
};

struct Operand {
    Operand_type type;
    Operand_size size;
    bool explicit_size = false;
    int16_t value = 0;
    int16_t displacement = 0;

    std::string to_string() 
    {
      switch (type) {
        case Register:
          return size == Word ? reg_names_16bit[value] : reg_names_8bit[value];
        case Memory:
          if (displacement == 0) {
            return "[" + rm_field_encodings[value] + "]";
          }
          else if (displacement > 0) {
            return "[" + rm_field_encodings[value] + " + " + std::to_string(displacement) +  "]";
          }
          else {
            return "[" + rm_field_encodings[value] + " - " + std::to_string(-displacement) +  "]";
          }
        case Immediate:
          if (explicit_size) {
            return (size == Word ? "word " : "byte ") + std::to_string(value);
          }
          else {
            return std::to_string(value);
          }
        case Direct_address:
          return "[" + std::to_string(value) + "]";
      }
      return "";
    }
};

struct Instruction {
    std::string name;
    size_t size = 0;
    Operand operands[2];

    std::string to_string() 
    {
      return name + " " + operands[0].to_string() + ", " + operands[1].to_string() + '\n';
    }
};

int16_t read_data(std::vector<unsigned char>& data, size_t& offset, bool word)
{
    int16_t value = word ? 
        static_cast<int16_t>(data[offset]) | (static_cast<int16_t>(data[offset + 1]) << 8) :
        static_cast<int8_t>(data[offset]);

    offset += (word ? 2 : 1);
    return value;
}

Operand set_rm_text(std::vector<unsigned char>& data, size_t& offset, char mod, char rm, bool word)
{
    Operand rm_operand;

    switch (mod) {
        case 0b00: {// Memory Mode (no displacement, except when r/m = 110)
            if (rm == 0b110) { //direct address
                int16_t value = read_data(data, offset, word);

                rm_operand.type = Operand_type::Direct_address;
                rm_operand.value = value;
                rm_operand.size = (word ? Operand_size::Word : Operand_size::Byte);
            }
            else {
                rm_operand.type = Operand_type::Memory;
                rm_operand.value = rm;
                rm_operand.size = (word ? Operand_size::Word : Operand_size::Byte);
            }
            break;
        }
        case 0b01: // Memory Mode, 8-bit displacement follows
        case 0b10: {// Memory Mode, 16-bit displacement follows
            int16_t displacement = read_data(data, offset, (mod == 0b10));
            rm_operand.type = Operand_type::Memory;
            rm_operand.value = rm;
            rm_operand.displacement = displacement;
            rm_operand.size = (mod == 0b10 ? Operand_size::Word : Operand_size::Byte);
            break;
        }
        case 0b11: // Register Mode (no displacement)
            rm_operand.type = Operand_type::Register;
            rm_operand.value = rm;
            rm_operand.size = (word ? Operand_size::Word : Operand_size::Byte);
            break;
    }
    return rm_operand;
}

Instruction reg_or_mem_with_reg_to_either(std::vector<unsigned char>& data, size_t& offset)
{
    int offset_start = offset;
    std::stringstream output;
    bool direction = (data[offset] & 0b10) >> 1; // 0 = reg field is source for mov, 1 = reg field is destination
    bool word = data[offset] & 0b1; // 0 = move 1 byte, 1 = move 2 bytes
    offset++;

    char mod = data[offset] >> 6;
    char reg = (data[offset] & 0b111000) >> 3;
    char rm = data[offset] & 0b111;
    offset++;

    Operand rm_operand = set_rm_text(data, offset, mod, rm, word);
    Operand reg_operand = {.type = Operand_type::Register, .size = (word ? Operand_size::Word : Operand_size::Byte), .value = reg };

    Instruction instruction;
    instruction.operands[0] = (direction ? reg_operand : rm_operand);
    instruction.operands[1] = (direction ? rm_operand : reg_operand);
    instruction.size = offset - offset_start;

    return instruction;
}

Instruction immediate_to_register(std::vector<unsigned char>& data, size_t& offset)
{
    int offset_start = offset;
    bool word = (data[offset] & 0b1000) >> 3; // 0 = move 1 byte, 1 = move 2 bytes
    char reg = data[offset] & 0b111;
    offset++;

    int16_t value = read_data(data, offset, word);

    Instruction instruction;
    instruction.operands[0] = {.type = Operand_type::Register, .size = (word ? Operand_size::Word : Operand_size::Byte), .value = reg};
    instruction.operands[1] = {.type = Operand_type::Immediate, .value = value};
    instruction.size = offset - offset_start;

    return instruction;
}

Instruction immediate_to_reg_mem(std::vector<unsigned char>& data, size_t& offset) 
{
    int offset_start = offset;
    bool word = data[offset] & 0b1; // 0 = move 1 byte, 1 = move 2 bytes
    offset++;

    char mod = data[offset] >> 6;

    // TODO read out instruction name
    //char reg = (data[offset] & 0b111000) >> 3;

    char rm = data[offset] & 0b111;
    offset++;


    Operand rm_operand = set_rm_text(data, offset, mod, rm, word);
    int16_t value = read_data(data, offset, word);
    Operand immediate_operand = {.type = Operand_type::Immediate, .size = (word ? Operand_size::Word : Operand_size::Byte), .explicit_size = true, .value = value };

    Instruction instruction;
    instruction.operands[0] = rm_operand;
    instruction.operands[1] = immediate_operand;
    instruction.size = offset - offset_start;

  return instruction;
}

Instruction memory_or_accumulator_to_either(std::vector<unsigned char>& data, size_t& offset, bool to_memory) 
{
  int offset_start = offset;
  bool word = data[offset] & 0b1; // 0 = move 1 byte, 1 = move 2 bytes
  offset++;

  int16_t value = read_data(data, offset, word);

  Instruction instruction;
  Operand memory_op = {.type = Operand_type::Direct_address, .size = (word ? Operand_size::Word : Operand_size::Byte), .value = value};
  Operand acc_op = {.type = Operand_type::Register, .size = Operand_size::Word, .value = 0}; // ax register
  instruction.operands[0] = to_memory ? memory_op : acc_op;
  instruction.operands[1] = to_memory ? acc_op : memory_op;
  instruction.size = offset - offset_start;

  return instruction;
}

std::string decode_asm_file(const std::string& path)
{
    std::cout << "oooo";
  std::ifstream asm_file(path, std::ios::binary);
  if (!asm_file) {
    throw std::runtime_error("Failed to open asm file");
  }

  std::vector<unsigned char> data(
    (std::istreambuf_iterator<char>(asm_file)),
    std::istreambuf_iterator<char>());

  std::stringstream output;
  output << ';' << path << " disassembly:" << std::endl;
  output << "bits 16" << std::endl;

  size_t offset = 0;
  while (offset < data.size()) {
    Instruction instruction;

    if (data[offset] >> 2 == 0b100010) {
      instruction = reg_or_mem_with_reg_to_either(data, offset);
      instruction.name = "mov";

    } else if (data[offset] >> 4 == 0b1011) { //MOV immediate to register
      instruction = immediate_to_register(data, offset);
      instruction.name = "mov";

    } else if (data[offset] >> 1 == 0b1010000) { //MOV memory to accumulator
      instruction = memory_or_accumulator_to_either(data, offset, false);
      instruction.name = "mov";

    } else if (data[offset] >> 1 == 0b1010001) { //MOV accumulator to memory
      instruction = memory_or_accumulator_to_either(data, offset, true);
      instruction.name = "mov";

    } else if (data[offset] >> 1 == 0b1100011) { //MOV immediate to register/memory
      instruction = immediate_to_reg_mem(data, offset);
      instruction.name = "mov";
    }
    output << instruction.to_string();
  }

  return output.str();
}

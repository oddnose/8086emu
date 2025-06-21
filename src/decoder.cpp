#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>


const char Mov_instruction = 0b100010; 

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

std::string decode_asm_file(const std::string& path)
{
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

    if (data[offset] >> 2 == Mov_instruction) {

      output << "mov ";
      bool direction = (data[offset] & 0b10) >> 1; // 0 = reg field is source for mov, 1 = reg field is destination
      bool word = data[offset] & 0b1; // 0 = move 1 byte, 1 = move 2 bytes

      offset++;

      char mod = data[offset] >> 6;
      char reg = (data[offset] & 0b111000) >> 3;
      char rm = data[offset] & 0b111;

      std::string rm_text;
      std::string reg_text;
      switch (mod) {
        case 0b00: {// Memory Mode (no displacement, except when r/m = 110)
          if (rm == 0b110) { //direct address
            int16_t value = word ? 
              static_cast<int16_t>(data[offset + 1]) | (static_cast<int16_t>(data[offset + 2]) << 8) :
              static_cast<int8_t>(data[offset + 1]);
            
            offset += (word ? 2 : 1);
            rm_text = '[' + std::to_string(value) + ']';
            reg_text = (word ? reg_names_16bit[reg] : reg_names_8bit[reg]);
          }
          else {
            rm_text = '[' + rm_field_encodings[rm] + ']';
            reg_text = (word ? reg_names_16bit[reg] : reg_names_8bit[reg]);
          }
          break;
        }
        case 0b01: // Memory Mode, 8-bit displacement follows
        case 0b10: {// Memory Mode, 16-bit displacement follows
          int16_t value = mod == 0b01 ? 
            static_cast<int8_t>(data[offset + 1]) :
            static_cast<int16_t>(data[offset + 1]) | (static_cast<int16_t>(data[offset + 2]) << 8);
          std::string value_text = (value >= 0) ? " + " + std::to_string(value) :
            " - " + std::to_string(-value);

          offset += (mod == 0b01 ? 1 : 2);

          rm_text = '[' + rm_field_encodings[rm] + value_text + ']';
          reg_text = (word ? reg_names_16bit[reg] : reg_names_8bit[reg]);
          break;
        }
        case 0b11: // Register Mode (no displacement)
          rm_text = (word ? reg_names_16bit[rm] : reg_names_8bit[rm]);
          reg_text = (word ? reg_names_16bit[reg] : reg_names_8bit[reg]);
          break;
      }
      output << (direction ? reg_text : rm_text) << ", ";
      output << (direction ? rm_text : reg_text);
      output << std::endl;
      offset++;
      continue;

    } else if (data[offset] >> 4 == 0b1011) { //MOV immediate to register
      output << "mov ";
      bool word = (data[offset] & 0b1000) >> 3; // 0 = move 1 byte, 1 = move 2 bytes
      char reg = data[offset] & 0b111;

      offset++;
      if (word == 0) {
        int value = static_cast<int>(data[offset]);
        output << reg_names_8bit[reg] << ", " << value << std::endl;
        offset++;
      } 
      else {
        //little endian order
        int value = static_cast<int>(data[offset]) | (static_cast<int>(data[offset + 1]) << 8);
        output << reg_names_16bit[reg] << ", " << value << std::endl;
        offset += 2;
      }
    } else if (data[offset] >> 1 == 0b1010000) { //MOV memory to accumulator
      output << "mov ";
      bool word = data[offset] & 0b1; // 0 = move 1 byte, 1 = move 2 bytes

      int16_t value = word ? 
          static_cast<int16_t>(data[offset + 1]) | (static_cast<int16_t>(data[offset + 2]) << 8) :
          static_cast<int8_t>(data[offset + 1]);
      
      offset += (word ? 3 : 2);
      output << "ax, [" << std::to_string(value) << ']' << std::endl;

    } else if (data[offset] >> 1 == 0b1010001) { //MOV accumulator to memory
      output << "mov ";
      bool word = data[offset] & 0b1; // 0 = move 1 byte, 1 = move 2 bytes

      int16_t value = word ? 
          static_cast<int16_t>(data[offset + 1]) | (static_cast<int16_t>(data[offset + 2]) << 8) :
          static_cast<int8_t>(data[offset + 1]);
      
      offset += (word ? 3 : 2);
      output << '[' << std::to_string(value) << "], ax" << std::endl;

    } else if (data[offset] >> 1 == 0b1100011) { //MOV immediate to register/memory
      output << "mov ";
      bool word = data[offset] & 0b1; // 0 = move 1 byte, 1 = move 2 bytes

      offset++;

      char mod = data[offset] >> 6;
      char rm = data[offset] & 0b111;

      std::string rm_text;
      switch (mod) {
        case 0b00: {// Memory Mode (no displacement, except when r/m = 110)
          if (rm == 0b110) {
            int16_t value = word ? 
              static_cast<int16_t>(data[offset + 1]) | (static_cast<int16_t>(data[offset + 2]) << 8) :
              static_cast<int8_t>(data[offset + 1]);
            
            offset += (word ? 2 : 1);
            rm_text = '[' + std::to_string(value) + ']';
          }
          else {
            rm_text = '[' + rm_field_encodings[rm] + ']';
          }
          break;
        }
        case 0b01: // Memory Mode, 8-bit displacement follows
        case 0b10: {// Memory Mode, 16-bit displacement follows
          int16_t value = mod == 0b01 ? 
            static_cast<int8_t>(data[offset + 1]) :
            static_cast<int16_t>(data[offset + 1]) | (static_cast<int16_t>(data[offset + 2]) << 8);
          std::string value_text = (value >= 0) ? " + " + std::to_string(value) :
            " - " + std::to_string(-value);

          offset += (mod == 0b01 ? 1 : 2);

          rm_text = '[' + rm_field_encodings[rm] + value_text + ']';
          break;
        }
        case 0b11: // Register Mode (no displacement)
          rm_text = (word ? reg_names_16bit[rm] : reg_names_8bit[rm]);
          break;
      }
      output << rm_text << ", ";
      offset++;

      if (word == 0) {
        int value = static_cast<int>(data[offset]);
        output << "byte " << value << std::endl;
        offset++;
      } 
      else {
        //little endian order
        int value = static_cast<int>(data[offset]) | (static_cast<int>(data[offset + 1]) << 8);
        output << "word " << value << std::endl;
        offset += 2;
      }

      continue;
    }
  }

  return output.str();
}

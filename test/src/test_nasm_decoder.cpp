#include "decoder.h"
#include "../src/8086_text.h"
#include <fstream>
#include <gtest/gtest.h>
#include <stdexcept>
#include <unistd.h>
#include <unistd.h>
#include <limits.h>

void test_decoder(std::string file_name)
{
	std::string decoded_file_name = file_name + "_decoded";
	std::string decoded_file_name_asm = file_name + "_decoded.asm";
	std::ofstream decoded_file;
	decoded_file.open(decoded_file_name_asm);
	std::string instructions = get_print_instructions(decode_asm_file(file_name));
	decoded_file << instructions;
	decoded_file.close();

	std::string cmd = "nasm " + decoded_file_name_asm + " -o " + decoded_file_name;
	int result = system(cmd.data());
	if (result != 0) {
		throw std::runtime_error("nasm failed");
	}

	result = system(std::string("diff " + decoded_file_name + " " + file_name).c_str());
	ASSERT_TRUE(result == 0);
}

TEST(NasmTest, decoder_test_single_mov)
{
	test_decoder("res/listing_0037_single_register_mov");
}

TEST(NasmTest, decoder_test_many_mov)
{
	test_decoder("res/listing_0038_many_register_mov");
}

TEST(NasmTest, decoder_test_more_movs)
{
	test_decoder("res/listing_0039_more_movs");
}

TEST(NasmTest, decoder_test_challenge_movs)
{
	test_decoder("res/listing_0040_challenge_movs");
}

TEST(NasmTest, decoder_test_add_sub_cmp_jnz)
{
	test_decoder("res/listing_0041_add_sub_cmp_jnz");
}

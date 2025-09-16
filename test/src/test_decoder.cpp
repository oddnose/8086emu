#include <fstream>
#include <gtest/gtest.h>
#include <stdexcept>
#include <unistd.h>
#include "../src/decoder.h"
    #include <unistd.h>
#include <limits.h>

void test_decoder(std::string file_name)
{
    std::ofstream decoded_file;
    decoded_file.open("decoded_file.asm");
    std::string decoded_str = decode_asm_file(file_name);
    decoded_file << decoded_str;
    decoded_file.close();

    int result = system("nasm decoded_file.asm -o decoded_file");
    if (result != 0) {
        throw std::runtime_error("nasm failed");
    }

    result = system(std::string("diff decoded_file " + file_name).c_str());
    ASSERT_TRUE(result == 0);
}
TEST(OtherTest, decoder_test_single_mov)
{
    test_decoder("res/listing_0037_single_register_mov");
}

TEST(OtherTest, decoder_test_many_mov)
{
    test_decoder("res/listing_0038_many_register_mov");
}

TEST(OtherTest, decoder_test_more_movs)
{
    test_decoder("res/listing_0039_more_movs");
}

TEST(OtherTest, decoder_test_challenge_movs)
{
    test_decoder("res/listing_0040_challenge_movs");
}

/*
TEST(OtherTest, decoder_test_add_sub_cmp_jnz)
{
    test_decoder("res/listing_0041_add_sub_cmp_jnz_copy");
}*/
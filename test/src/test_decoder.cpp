#include <fstream>
#include <gtest/gtest.h>
#include <stdexcept>
#include <unistd.h>
#include "../src/decoder.h"
    #include <unistd.h>
#include <limits.h>
#include <iostream>

/*TEST(OtherTest, decoder_test_single_mov)
{
    std::ofstream decoded_file;
    decoded_file.open("decoded_file.asm");
    decoded_file << decode_asm_file("res/listing_0037_single_register_mov");
    decoded_file.close();

    int result = system("nasm decoded_file.asm -o decoded_file");
    if (result != 0) {
        throw std::runtime_error("nasm failed");
    }

    result = system("diff res/listing_0037_single_register_mov decoded_file");
    ASSERT_TRUE(result == 0);
}*/

TEST(OtherTest, decoder_test_many_mov)
{
    std::ofstream decoded_file;
    decoded_file.open("decoded_file.asm");
    decoded_file << decode_asm_file("res/listing_0038_many_register_mov");
    decoded_file.close();

    int result = system("nasm decoded_file.asm -o decoded_file");
    if (result != 0) {
        throw std::runtime_error("nasm failed");
    }

    result = system("diff res/listing_0038_many_register_mov decoded_file");
    ASSERT_TRUE(result == 0);
}
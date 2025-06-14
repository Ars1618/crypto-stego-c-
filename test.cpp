#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "steganography.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

TEST_CASE("Testing Vigenere Cipher") {
    SUBCASE("Basic Encryption") {
        CHECK(encrypt_vigenere("CRYPTO", "KEY") == "MVWZXM");
        CHECK(encrypt_vigenere("hello", "key") == "rijvs");
        CHECK(encrypt_vigenere("ATTACKATDAWN", "LEMON") == "LXFOPVEFRNHR");
    }
}
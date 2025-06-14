#ifndef STEGANOGRAPHY_H
#define STEGANOGRAPHY_H

#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <cmath>

//Способы шифрования сообщения
std::string encrypt_vigenere(const std::string& text, const std::string& key);
std::string decrypt_vigenere(const std::string& encrypted, const std::string& key);

std::string encrypt_opentxt(const std::string& text, const std::string& key);
std::string decrypt_opentxt(const std::string& text, const std::string& key);

std::string encrypt_shifrtxt(const std::string& text, const std::string& key);
std::string decrypt_shifrtxt(const std::string& text, const std::string& key);

std::string affine_shifr(const std::string& text, int type);
std::string affine_shifr_rec(const std::string& text, int type);

const int DELTA = 16;  // Шаг квантования

std::vector<unsigned char> ToGray(unsigned char* data, int width, int height, int channels);
std::vector<bool> stringToBits(const std::string& message);
void embedMessage(std::vector<unsigned char>& image, const std::vector<bool>& message);
bool stegoImage(const std::string& filename,const std::vector<unsigned char>& imagePixels,int width, int height);
std::vector<bool> extractBits(const std::vector<unsigned char>& image, size_t startIndex, size_t maxBitsToExtract);
std::string bitsToString(const std::vector<bool>& bits);
std::string extractMessage(const std::string& stegoImagePath);

#endif
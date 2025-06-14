#ifndef STEGANOGRAPHY_H
#define STEGANOGRAPHY_H

#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <cmath>

//Способы шифрования сообщения
/**
* @brief Шифрует текст с использованием шифра Виженера
*
* @param[in] text Исходный текст для шифрования
* @param[in] key Ключ шифрования
* @return Зашифрованная строка
*
* @details Алгоритм шифрования:
* 1. Генерируется гамма путем повторения ключа до длины текста
* 2. Для каждого символа в тексте:
*    - Если это буква, вычисляется сдвиг на основе соответствующего символа в гамме
*    - Не-буквенные символы остаются без изменений
* 3. Регистр символов сохраняется
*
* @note Пример:
* @code
        * encrypt_vigenere("HELLO", "KEY") // Возвращает "RIJVS"
* @endcode
*/
std::string encrypt_vigenere(const std::string& text, const std::string& key);
/**
 * @brief Дешифрует текст, зашифрованный шифром Виженера
 *
 * @param[in] encrypted Зашифрованный текст
 * @param[in] key Ключ, использованный при шифровании
 * @return Расшифрованная строка
 *
 * @details Алгоритм дешифрования обратен алгоритму шифрования:
 * 1. Генерируется гамма (та же, что при шифровании)
 * 2. Для каждого символа в зашифрованном тексте:
 *    - Если это буква, вычисляется обратный сдвиг
 *    - Не-буквенные символы остаются без изменений
 *
 * @note Пример:
 * @code
 * decrypt_vigenere("RIJVS", "KEY") // Возвращает "HELLO"
 * @endcode
 */
std::string decrypt_vigenere(const std::string& encrypted, const std::string& key);

std::string encrypt_opentxt(const std::string& text, const std::string& key);
std::string decrypt_opentxt(const std::string& text, const std::string& key);

std::string encrypt_shifrtxt(const std::string& text, const std::string& key);
std::string decrypt_shifrtxt(const std::string& text, const std::string& key);

std::string affine_shifr(const std::string& text, int type);
std::string affine_shifr_rec(const std::string& text, int type);

/**
 * @brief Шаг квантования для стеганографического метода QIM
 *
 * @details Определяет размер "ячейки" при встраивании битов в изображение.
 * Большие значения DELTA повышают устойчивость к шумам, но уменьшают
 * вместимость скрытого сообщения.
 */
const int DELTA = 16;  // Шаг квантования

std::vector<unsigned char> ToGray(unsigned char* data, int width, int height, int channels);
/**
 * @brief Преобразует строку в последовательность битов
 *
 * @param[in] message Входная строка
 * @return Вектор битов (порядок: старший бит первого символа первым)
 *
 * @details Каждый символ преобразуется в 8 бит (бинарное представление)
 */
std::vector<bool> stringToBits(const std::string& message);
void embedMessage(std::vector<unsigned char>& image, const std::vector<bool>& message);
bool stegoImage(const std::string& filename,const std::vector<unsigned char>& imagePixels,int width, int height);
std::vector<bool> extractBits(const std::vector<unsigned char>& image, size_t startIndex, size_t maxBitsToExtract);
std::string bitsToString(const std::vector<bool>& bits);
std::string extractMessage(const std::string& stegoImagePath);

#endif
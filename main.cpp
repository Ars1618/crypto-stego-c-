#include <iostream>
#include <string>
#include <cctype>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
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



int main() {
    int action;//Первичное действие. Чтобы начать что-то делать, нужно выбрать с чего начать
    int width, height, channels;//Ширина, высота и каналы, все это объявление переменных для работы со стегой
    std::string mess;
    int type;//Переменная для выбора способа зашифрвоания встраиваемого изображения
    std::string key;//Тут будет храниться ключ
    std::string temp_mess;
    unsigned char* imageData = stbi_load("picture.jpg", &width, &height, &channels, 0);
    std::vector<unsigned char> grayImage; //С черно-белым изобаржением работать удобнее, это матрица "черно-белых" значений
    if (channels >= 3) {
        grayImage = ToGray(imageData, width, height, channels);
    } else {
        grayImage = std::vector<unsigned char>(imageData, imageData + width * height);
    }
    std::cout << "Выберите действие: Встроить текст в изображение(1) или извлеч сообщение из изображения(2): " << std::endl;
    std::cin >> action;
    if (action == 1){
        std::cout << "Введите сообщение которое хотите встроить: " << std::endl;
        std::cin >> mess;
        std::cout << "Выберите шифр, с помощью которого, сообщение будет зашифрованно "
                     "и встроенно в изображение: Шифр Виженера(1), шифр Виженера по открытому тексту(2), "
                     "по шифр тексту(3), афинный(4), афинный рекурентный(5)" << std::endl;
        std::cin >> type;
        if (type == 1){
            std::cout << "Введите ключ: " << std::endl;
            std::cin >> key;
            temp_mess = encrypt_vigenere(mess, key);
        }
        else if (type == 2){
            std::cout << "Введите ключ: ";
            std::cin >> key;
            if (key.length() == 1 && key.length() != 0) {
                temp_mess = encrypt_shifrtxt(mess, key);
            }
            else{
                std::cout << "Некоректный ключ!" << std::endl;
            }
        }
        else if (type == 3){
            std::cout << "Введите ключ: ";
            std::cin >> key;
            if (key.length() == 1 && key.length() != 0) {
                temp_mess = encrypt_shifrtxt(mess, key);
            }
        }
        else if (type == 4){
            temp_mess = affine_shifr(mess, type);
        }
        else if(type == 5){
            temp_mess = affine_shifr_rec(mess, type);
        }
        else{
            std::cout << "Не выбран шифр!" << std::endl;
        }
        std::vector<bool> message = stringToBits(temp_mess);
        embedMessage(grayImage, message);
        stegoImage("stego.png", grayImage, width, height);
    }
    else if (action == 2){
        std::string extract_message = extractMessage("stego.png");
        std::cout << "Выберите способ расшифровки извлеченного текста: Вижинер(1), по открытому(2), "
                     "по шифртексту(3), по афинному(4) или по афинному рекуретному(5): " << std::endl;
        std::cin >> type;
        if (type == 1){
            std::cout << "Введите ключ: " << std::endl;
            std::cin >> key;
            temp_mess = decrypt_vigenere(extract_message, key);
        }
        else if (type == 2){
            std::cout << "Введите ключ: ";
            std::cin >> key;
            if (key.length() == 1 && key.length() != 0) {
                temp_mess = decrypt_shifrtxt(extract_message, key);
            }
            else{
                std::cout << "Некоректный ключ!" << std::endl;
            }
        }
        else if (type == 3){
            std::cout << "Введите ключ: ";
            std::cin >> key;
            temp_mess = decrypt_shifrtxt(extract_message, key);
        }
        else if (type == 4){
            temp_mess = affine_shifr(extract_message, type);
        }
        else if(type == 5){
            temp_mess = affine_shifr_rec(extract_message, type);
        }
        else{
            std::cout << "Не выбран шифр!" << std::endl;
        }
        std::cout << temp_mess << std::endl;
    }
    return 0;
}



//Шифр Виженера
std::string encrypt_vigenere(const std::string& text, const std::string& key) {
    const std::string upper_alf = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string lower_alf = "abcdefghijklmnopqrstuvwxyz";
    std::string result;

    // Создаю гамму
    std::string gamma;
    for (int i = 0; i < text.length(); ++i) {
        gamma += key[i % key.length()];
    }

    for (int i = 0; i < text.length(); ++i) {
        char c = text[i];
        char g = gamma[i];

        if (isupper(c)) {
            // Для заглавных букв
            int text_pos = upper_alf.find(c);
            int key_pos = upper_alf.find(toupper(g));
            if (text_pos != std::string::npos && key_pos != std::string::npos) {
                result += upper_alf[(text_pos + key_pos) % 26];
            } else {
                result += c; // если символ не найден в алфавите, оставляем как есть
            }
        } else if (islower(c)) {
            // Для строчных букв
            int text_pos = lower_alf.find(c);
            int key_pos = lower_alf.find(tolower(g));
            if (text_pos != std::string::npos && key_pos != std::string::npos) {
                result += lower_alf[(text_pos + key_pos) % 26];
            } else {
                result += c;
            }
        } else {
            // Не-буквенные символы остаются без изменений
            result += c;
        }
    }

    return result;
}
std::string decrypt_vigenere(const std::string& encrypted, const std::string& key) {
    const std::string upper_alf = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string lower_alf = "abcdefghijklmnopqrstuvwxyz";
    std::string result;

    // Создаем гамму (повторяем ключ до длины текста)
    std::string gamma;
    for (int i = 0; i < encrypted.length(); ++i) {
        gamma += key[i % key.length()];
    }

    for (int i = 0; i < encrypted.length(); ++i) {
        char c = encrypted[i];
        char g = gamma[i];

        if (isupper(c)) {
            // Для заглавных букв
            int enc_pos = upper_alf.find(c);
            int key_pos = upper_alf.find(toupper(g));
            if (enc_pos != std::string::npos && key_pos != std::string::npos) {
                // Обратите внимание на +26 перед вычитанием для корректного модуля
                result += upper_alf[(enc_pos - key_pos + 26) % 26];
            } else {
                result += c; // если символ не найден в алфавите, оставляем как есть
            }
        } else if (islower(c)) {
            // Для строчных букв
            int enc_pos = lower_alf.find(c);
            int key_pos = lower_alf.find(tolower(g));
            if (enc_pos != std::string::npos && key_pos != std::string::npos) {
                result += lower_alf[(enc_pos - key_pos + 26) % 26];
            } else {
                result += c;
            }
        } else {
            // Не-буквенные символы остаются без изменений
            result += c;
        }
    }

    return result;
}

//Шифр Виженера по открытому тексту
std::string encrypt_opentxt(const std::string& text, const std::string& key){
    const std::string upper_alf = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string lower_alf = "abcdefghijklmnopqrstuvwxyz";
    std::string result;
    std::string gamma = key;

    for(int i = 0; i < text.length()-1; i++){
        gamma = gamma + text[i];
    }

    for(int i = 0; i < text.length(); i++){
        char c = text[i];
        char g = gamma[i];
        if (isupper(text[i])){
            int text_pos = upper_alf.find(c);
            int g_pos = upper_alf.find(toupper(g));
            if (text_pos != std::string::npos && g_pos != std::string::npos) {
                result += upper_alf[(text_pos + g_pos) % 26];
            } else {
                result += c;
            }
        }
        else if (islower(c)) {
            int text_pos = lower_alf.find(c);
            int key_pos = lower_alf.find(tolower(g));
            if (text_pos != std::string::npos && key_pos != std::string::npos) {
                result += lower_alf[(text_pos + key_pos) % 26];
            } else {
                result += c;
            }
        } else {
            result += c;
        }

    }
    return result;
}
std::string decrypt_opentxt(const std::string& text, const std::string& key){
    const std::string upper_alf = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string lower_alf = "abcdefghijklmnopqrstuvwxyz";
    std::string result;
    std::string gamma = key;

    for(int i = 0; i < text.length(); i++){
        char c = text[i];
        char g = gamma[i];
        if (isupper(text[i])){
            int text_pos = upper_alf.find(c);
            int g_pos = upper_alf.find(toupper(g));
            if (text_pos != std::string::npos && g_pos != std::string::npos) {
                result += upper_alf[(text_pos - g_pos +26) % 26];
                gamma += result[i];
            } else {
                result += c;
            }
        }
        else if (islower(c)) {
            int text_pos = lower_alf.find(c);
            int key_pos = lower_alf.find(tolower(g));
            if (text_pos != std::string::npos && key_pos != std::string::npos) {
                result += lower_alf[(text_pos - key_pos + 26) % 26];
                gamma += result[i];
            } else {
                result += c;
            }
        } else {
            result += c;
        }

    }
    return result;
}

//Шифр Виженера по шифртексту
std::string encrypt_shifrtxt(const std::string& text, const std::string& key){
    const std::string upper_alf = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string lower_alf = "abcdefghijklmnopqrstuvwxyz";
    std::string result;
    char k = key[0];
    for(int i = 0; i < text.length(); i++){
        char c = text[i];
        if (isupper(c)){
            char text_pos = upper_alf.find(c);
            char key_pos = upper_alf.find(k);
            if (text_pos != std::string::npos && key_pos != std::string::npos) {
                result += upper_alf[(text_pos + key_pos) % 26];
                k = result[i];
            } else {
                result += c;
            }
        }
        else if (islower(c)) {
            int text_pos = lower_alf.find(c);
            int key_pos = lower_alf.find(tolower(k));
            if (text_pos != std::string::npos && key_pos != std::string::npos) {
                result += lower_alf[(text_pos + key_pos) % 26];
                k = result[i];
            } else {
                result += c;
            }
        } else {
            result += c;
        }

    }
    return result;
}
std::string decrypt_shifrtxt(const std::string& text, const std::string& key) {
    const std::string upper_alf = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string lower_alf = "abcdefghijklmnopqrstuvwxyz";
    std::string result;
    std::string gamma = key + text;
    for(size_t i = 0; i < text.length(); i++) {
        if(i >= gamma.length()) {
            break;
        }
        char c = text[i];
        char g = gamma[i];
        if(isupper(c)) {
            int text_pos = upper_alf.find(c);
            int g_pos = upper_alf.find(toupper(g));
            if(text_pos != std::string::npos && g_pos != std::string::npos) {
                result += upper_alf[(text_pos - g_pos + 26) % 26];
            } else {
                result += c;
            }
        }
        else if(islower(c)) {
            int text_pos = lower_alf.find(c);
            int g_pos = lower_alf.find(tolower(g));
            if(text_pos != std::string::npos && g_pos != std::string::npos) {
                result += lower_alf[(text_pos - g_pos + 26) % 26];
            } else {
                result += c;
            }
        } else {
            result += c;
        }
    }
    return result;
}

//Афинный и афинный рекурентный шифр
std::string affine_shifr(const std::string& text, int type){
    const std::string upper_alf = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string lower_alf = "abcdefghijklmnopqrstuvwxyz";
    std::string result;
    int key[2];
    std::cout << "Задайте параметр a и b для ключа (НОД(a,m) должен = 1): " << std::endl;
    std::cin >> key[0] >> key[1];
    if (key[0]%26 != 1){
        return "НОД(a,m) не равен 1!";
    }
    if (type == 1){
        for(int i = 0; i < text.length(); i++){
            char c = text[i];
            if (isupper(c)){
                char text_pos = upper_alf.find(c);
                result = result + upper_alf[(key[0]*text_pos + key[1])%26];
            }
            else if (islower(c)){
                char text_pos = lower_alf.find(c);
                result = result + lower_alf[(key[0]*text_pos + key[1])%26];
            }
        }
    }
    else if (type == 2){
        int neg_a;
        for (int i = 0; i < 100; i++){
            if ((key[0]*i)%26 == 1) {
                neg_a = i;
                break;
            }
        }
        for(int i = 0; i < text.length(); i++){
            char c = text[i];
            if (isupper(c)){
                char text_pos = upper_alf.find(c);
                result = result + upper_alf[((text_pos-key[1]+26)*neg_a)%26];
            }
            else if (islower(c)){
                char text_pos = lower_alf.find(c);
                result = result + lower_alf[((text_pos-key[1]+26)*neg_a + 26)%26];
            }
        }
    }

    return result;
}
std::string affine_shifr_rec(const std::string& text, int type){
    const std::string upper_alf = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string lower_alf = "abcdefghijklmnopqrstuvwxyz";
    std::string result;
    std::vector<std::vector<int> > key(2, std::vector<int>(2));
    std::cout << "Введите параметр a b для первого и второго ключа (НОД(a, 26) должен = 1: " << std::endl;
    std::cin >> key[0][0] >> key[0][1] >> key[1][0] >> key[1][1];
    if (type == 1){
        for (int i = 0; i < text.length(); i++){
            char c = text[i];
            if (result.length() >= 2){
                key.push_back(std::vector<int>());
                key[i].push_back((key[i-1][0] * key[i-2][0]) % 26);
                key[i].push_back((key[i-1][1] + key[i-2][1]) % 26);
            }
            if (isupper(c)) {
                char text_pos = upper_alf.find(c);
                if (text_pos != std::string::npos) {
                    int encrypted = (key[i][0] * text_pos + key[i][1]) % 26;
                    result += upper_alf[encrypted];
                }
            } else if (islower(c)) {
                size_t text_pos = lower_alf.find(c);
                if (text_pos != std::string::npos) {
                    int encrypted = (key[i][0] * text_pos + key[i][1]) % 26;
                    result += lower_alf[encrypted];
                }
            } else {
                result += c;
            }
        }
    }
    else if(type == 2){
        int neg_a;
        for (int i = 0; i < text.length(); i++){
            char c = text[i];
            if (result.length() >= 2){
                key.push_back(std::vector<int>());
                key[i].push_back((key[i-1][0] * key[i-2][0]) % 26);
                key[i].push_back((key[i-1][1] + key[i-2][1]) % 26);
            }
            for (int j = 0; j < 26; j++){
                if((key[i][0]*j)%26 == 1){
                    neg_a = j;
                    break;
                }
            }
            if (isupper(c)) {
                char text_pos = upper_alf.find(c);
                result = result + upper_alf[((text_pos-key[i][1]+26)*neg_a)%26];
            }
            else if (islower(c)) {
                char text_pos = lower_alf.find(c);
                result = result + lower_alf[((text_pos-key[i][1]+26)*neg_a)%26];
            }
            else {
                result += c;
            }
        }

    }
    return result;
}


//Как же встраивать сообщение в изображение при помощи метода QIM?
//Часть - 1 встраивание
//Для реализации этого, мне потребуется шак квантования (пусть он будет постоянный без возможности выбоар)
//Потребуется функция преобразования изобарежние из цветного (если оно цветное) в черно-белое (потому что
//(иначе я в край запутаюсь, попытка была))
// В полученное изоабрежение мне нужно внести сообщение, для этого нужна функция разбиения сообщения
//на биты, следовательно создам функцию пеервода строки сообщения в последовательность битов
//После этого можно наконец встраивать сообщение в изображение. Создам функцию встраивания
//После этого, полученное стегоизображение нужно сохранить, сделаю это (создам функцию)

std::vector<unsigned char> ToGray(unsigned char* data, int width, int height, int channels) {
    std::vector<unsigned char> grayImage(width * height);
    for (int i = 0; i < width * height; i++) {
        unsigned char r = data[i * channels];
        unsigned char g = data[i * channels + 1];
        unsigned char b = data[i * channels + 2];
        grayImage[i] = static_cast<unsigned char>(0.299 * r + 0.587 * g + 0.114 * b);
    }
    return grayImage;
}//Преобразую цветное изображение в черно-белое
std::vector<bool> stringToBits(const std::string& message) {
    std::vector<bool> bits;

    for (char c : message) {
        std::bitset<8> charBits(c); // Преобразуем символ в 8 бит
        for (int i = 7; i >= 0; --i) {
            bits.push_back(charBits[i]); // Добавляем биты в вектор
        }
    }

    return bits;
}
void embedMessage(std::vector<unsigned char>& image, const std::vector<bool>& message) {
    // Добавляем 32-битный заголовок с длиной сообщения
    std::vector<bool> fullMessage;
    uint32_t len = message.size();
    for (int i = 31; i >= 0; i--) {
        fullMessage.push_back((len >> i) & 1);
    }
    fullMessage.insert(fullMessage.end(), message.begin(), message.end());

    // Встраивание комбинированных битов
    int bitIndex = 0;
    for (size_t i = 0; i < image.size(); i++) {
        if (bitIndex >= fullMessage.size()) return;

        unsigned char pixel = image[i];
        bool currentBit = fullMessage[bitIndex];

        if (!currentBit) {
            image[i] = round(pixel / static_cast<float>(DELTA)) * DELTA;
        } else {
            float quantized = round((pixel - DELTA/2.0) / DELTA) * DELTA + DELTA/2.0;
            image[i] = static_cast<unsigned char>(quantized);
        }
        bitIndex++;
    }
}
bool stegoImage(const std::string& filename,const std::vector<unsigned char>& imagePixels,int width, int height) {
    int save = stbi_write_png(filename.c_str(),width,height,1,imagePixels.data(),width);
    return save != 0;
}
//Ну и наконец зачем все это, если нельзя извлеч обратно? Потребуется функция
//Часть - 2 извлечение
//Нужно достать сообщение из изобаржения, следовательно нужно понять, какие биты в стегоизображении
//это часть сообщения, а какие нет. Нужна функция, которая бы извлекала битовую поледовательность
//сообещния из изображения. Но нужны не биты, а строка сообщения, а значит нужна еще одна функция
//которая бы преобразовывала последовательность битов в осмысленую строку встроенного сообещния
//в стегоизобаржение. Ну и нужна функция (для удобаства и красоты) которая бы в себе содержала
//эти действия. Готово, изображение извлеченно!
std::vector<bool> extractBits(const std::vector<unsigned char>& image, size_t startIndex, size_t maxBitsToExtract) {
    std::vector<bool> bits;
    for (size_t i = startIndex; i < image.size() && bits.size() < maxBitsToExtract; ++i) {
        unsigned char pixel = image[i];
        float remainder = fmod(pixel, DELTA);
        bool bit = (remainder >= DELTA / 2.0f);
        bits.push_back(bit);
    }
    return bits;
}
std::string bitsToString(const std::vector<bool>& bits) {
    std::string message;
    for (size_t i = 0; i + 7 < bits.size(); i += 8) {
        std::bitset<8> charBits;
        for (int j = 0; j < 8; ++j) {
            charBits[7 - j] = bits[i + j]; // Собираем биты в байт
        }
        message += static_cast<char>(charBits.to_ulong());
    }
    return message;
}
std::string extractMessage(const std::string& stegoImagePath) {
    int width, height, channels;
    unsigned char* imageData = stbi_load(stegoImagePath.c_str(), &width, &height, &channels, 1);
    if (!imageData) {
        return "Ошибка загрузки изображения!";
    }

    std::vector<unsigned char> image(imageData, imageData + width * height);
    stbi_image_free(imageData);

    // Извлечение длины сообщения (первые 32 бита)
    std::vector<bool> lenBits = extractBits(image, 0, 32);
    uint32_t msgLen = 0;
    for (int i = 0; i < 32; i++) {
        msgLen = (msgLen << 1) | (lenBits[i] ? 1 : 0);
    }

    // Извлечение основного сообщения
    std::vector<bool> msgBits = extractBits(image, 32, msgLen);
    return bitsToString(msgBits);
}

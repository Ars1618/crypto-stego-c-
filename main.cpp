#include "steganography.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


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
        std::cout << "Текущее извлеченное зашифрованное сообщение: " << extract_message << std::endl;
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




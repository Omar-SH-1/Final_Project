#include <iostream>
#include <fstream>
#include <string>

int main() {
    const int SIZE = 195;
    int connections[SIZE][SIZE] = {0};
    std::string countries[SIZE];


    std::ifstream file("195_countries.txt");
    if (!file) {
        std::cerr << "Не удалось открыть файл 195_countries.txt!" << std::endl;
        return 1;
    }


    for (int i = 0; i < SIZE; ++i) {
        if (!std::getline(file, countries[i])) {
            std::cerr << "Ошибка чтения файла или недостаточно строк в файле!" << std::endl;
            return 1;
        }
    }
    file.close();


    int country1, country2;
    char response;

    std::cout << "Список стран загружен. Вы можете задавать связи между ними.\n";

    while (true) {
        std::cout << "\nВведите номера двух стран (от 0 до " << SIZE - 1 << ") для создания связи:\n";
        for (int i = 0; i < SIZE; ++i) {
            std::cout << i << ": " << countries[i] << std::endl;
        }

        std::cin >> country1 >> country2;


        if (country1 < 0 || country1 >= SIZE || country2 < 0 || country2 >= SIZE) {
            std::cout << "Неверный номер страны. Попробуйте снова.\n";
            continue;
        }


        connections[country1][country2] = 1;
        connections[country2][country1] = 1;

        std::cout << "Добавить еще связь? (y/n): ";
        std::cin >> response;
        if (response == 'n' || response == 'N') break;
    }

    // Вывод матрицы связей
    std::cout << "\nМатрица связей:\n";
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            std::cout << connections[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}

#include <iostream>
#include <fstream>
#include <cstring>

const int SIZE = 195;           // Количество стра
const int MAX_NAME_LENGTH = 100; // Максимальная длина названия страны

void saveMatrixToFile(int connections[SIZE][SIZE], const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Не удалось открыть файл для сохранения матрицы!" << std::endl;
        return;
    }

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            file << connections[i][j] << " ";
        }
        file << "\n";
    }

    file.close();
    std::cout << "Матрица успешно сохранена в файл " << filename << "\n";
}

void loadMatrixFromFile(int connections[SIZE][SIZE], const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Файл матрицы не найден. Начинаем с пустой матрицы.\n";
        return;
    }

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            file >> connections[i][j];
        }
    }

    file.close();
    std::cout << "Матрица успешно загружена из файла " << filename << "\n";
}

int main() {
    int connections[SIZE][SIZE] = {0};   // Матрица связей
    char countries[SIZE][MAX_NAME_LENGTH]; // Названия стран

    // Считывание списка стран
    std::ifstream countriesFile("195_countries.txt");
    if (!countriesFile) {
        std::cerr << "Не удалось открыть файл 195_countries.txt\n";
        return 1;
    }

    for (int i = 0; i < SIZE; ++i) {
        countriesFile.getline(countries[i], MAX_NAME_LENGTH);
    }
    countriesFile.close();

    // Загрузка матрицы из файла
    loadMatrixFromFile(connections, "connections_matrix.txt");

    // Вручную задаем связи
    int country1, country2;
    char response;

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
        connections[country2][country1] = 1; // Симметричная связь

        std::cout << "Добавить еще связь? (y/n): ";
        std::cin >> response;
        if (response == 'n' || response == 'N') break;
    }

    // Сохранение матрицы в файл
    saveMatrixToFile(connections, "connections_matrix.txt");

    return 0;
}

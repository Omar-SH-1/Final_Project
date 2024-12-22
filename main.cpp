#include <iostream>
#include <cstdio>
#include <cstring>

const int SIZE = 195;           // Количество стран
const int MAX_NAME_LENGTH = 100; // Максимальная длина названия страны

int main() {
    int connections[SIZE][SIZE] = {0};   // Матрица связей, изначально заполненная нулями
    char countries[SIZE][MAX_NAME_LENGTH]; // Массив для хранения названий стран

    // Открытие файла с названиями стран
    FILE* file = fopen("195_countries.txt", "r");
    if (!file) {
        perror("Не удалось открыть файл 195_countries.txt");
        return 1;
    }

    // Считывание списка стран
    for (int i = 0; i < SIZE; ++i) {
        if (fgets(countries[i], MAX_NAME_LENGTH, file) == nullptr) {
            std::cerr << "Ошибка чтения файла или недостаточно строк в файле!" << std::endl;
            fclose(file);
            return 1;
        }

        // Удаляем символ новой строки, если он есть
        size_t len = strlen(countries[i]);
        if (len > 0 && countries[i][len - 1] == '\n') {
            countries[i][len - 1] = '\0';
        }
    }

    fclose(file); // Закрываем файл

    // Вручную задаем связи между странами
    int country1, country2;
    char response;

    while (true) {
        std::cout << "\nВведите номера двух стран (от 0 до " << SIZE - 1 << ") для создания связи:\n";
        std::cin >> country1 >> country2;

        if (country1 < 0 || country1 >= SIZE || country2 < 0 || country2 >= SIZE) {
            std::cout << "Неверный номер страны. Попробуйте снова.\n";
            continue;
        }

        connections[country1][country2] = 1;
        connections[country2][country1] = 1; // Если связь симметрична

        std::cout << "Добавить еще связь? (y/n): ";
        std::cin >> response;
        if (response == 'n' || response == 'N') break;
    }

    // Сохранение матрицы в файл
    FILE* output = fopen("connections_matrix.txt", "w");
    if (!output) {
        perror("Не удалось открыть файл для записи");
        return 1;
    }

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            fprintf(output, "%d ", connections[i][j]); // Записываем элемент матрицы
        }
        fprintf(output, "\n"); // Переход на новую строку
    }

    fclose(output); // Закрываем файл
    std::cout << "Матрица связей успешно сохранена в файл connections_matrix.txt\n";

    return 0;
}

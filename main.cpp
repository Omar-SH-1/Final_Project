#include "Final.h"

int main() {
    std::vector<std::string> countries = loadCountries("195_countries.txt");
    if (countries.empty()) {
        std::cerr << "Ошибка: Список стран пуст!" << std::endl;
        return 1;
    }

    std::vector<std::vector<int>> matrix = loadMatrix("connections_matrix.txt");
    if (matrix.size() != countries.size()) {
        std::cerr << "Ошибка: Размер матрицы не соответствует количеству стран!" << std::endl;
        return 1;
    }

    std::unordered_map<std::string, std::vector<int>> continents = {
            {"Africa", {4, 18, 26, 27, 28, 30, 32, 33, 37, 38, 44, 50, 52, 53, 60, 61, 64, 68, 69, 85, 94, 95, 96, 100, 101, 104, 107, 108, 115, 116, 118, 124, 125, 151, 153, 154, 159, 160, 162, 165, 171, 174, 181, 193, 194}},
            {"Asia", {0, 12, 13, 77, 78, 79, 81, 84, 85, 86, 88, 89, 90, 91, 129, 130, 132, 135, 136, 140, 169, 170, 172, 173, 178, 179, 187, 191, 192}},
            {"Europe", {1, 3, 9, 15, 16, 25, 40, 42, 43, 45, 59, 63, 65, 74, 75, 80, 82, 92, 93, 97, 98, 99, 112, 114, 121, 127, 128, 138, 139, 141, 142, 152, 156, 157, 167, 168}},
            {"North America", {5, 11, 17, 31, 39, 41, 47, 48, 51, 66, 67, 70, 71, 72, 73, 109, 110, 123, 131, 133, 134, 144, 145, 146, 176, 185}},
            {"South America", {6, 20, 23, 49, 135, 136, 186, 190}},
            {"Oceania", {8}}
    };

    std::string country1, country2;
    bool validInput = false;

    while (!validInput) {
        std::cout << "Введите две страны, между которыми оборвалась связь: ";
        std::cin >> country1 >> country2;

        auto it1 = std::find(countries.begin(), countries.end(), country1);
        auto it2 = std::find(countries.begin(), countries.end(), country2);

        if (it1 == countries.end() || it2 == countries.end()) {
            std::cerr << "Ошибка: Одна или обе введенные страны не найдены в списке! Попробуйте ещё раз.\n";
        } else {
            validInput = true;

            int country1Idx = std::distance(countries.begin(), it1);
            int country2Idx = std::distance(countries.begin(), it2);

            removeConnection(matrix, country1Idx, country2Idx);

            std::cout << "Проверка доступности и маршрутов для " << country1 << ":\n";
            checkConnectionsToContinents(matrix, country1Idx, continents, countries);

            std::cout << "\nПроверка доступности и маршрутов для " << country2 << ":\n";
            checkConnectionsToContinents(matrix, country2Idx, continents, countries);
        }
    }

    return 0;
}

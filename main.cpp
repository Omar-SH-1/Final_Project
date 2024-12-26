#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <sstream>

// Проверка, что строка содержит только цифры и пробелы
bool isValidMatrixLine(const std::string& line) {
    for (char c : line) {
        if (!std::isdigit(c) && c != ' ' && c != '\t') {
            return false;
        }
    }
    return true;
}

// Функция для загрузки списка стран
std::vector<std::string> loadCountries(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << filename << std::endl;
        exit(1);
    }

    std::vector<std::string> countries;
    std::string country;
    while (std::getline(file, country)) {
        countries.push_back(country);
    }

    file.close();
    return countries;
}

// Функция для загрузки матрицы из файла
std::vector<std::vector<int>> loadMatrix(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << filename << std::endl;
        exit(1);
    }

    std::vector<std::vector<int>> matrix;
    std::string line;
    while (std::getline(file, line)) {
        // Удаление начальных и конечных пробелов
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty()) continue; // Пропуск пустых строк

        if (!isValidMatrixLine(line)) {
            std::cerr << "Ошибка: Некорректные данные в строке: \"" << line << "\"" << std::endl;
            exit(1);
        }

        std::vector<int> row;
        std::istringstream stream(line);
        int value;
        while (stream >> value) {
            row.push_back(value);
        }

        matrix.push_back(row);
    }

    file.close();
    return matrix;
}

// Удаление связи между двумя странами
void removeConnection(std::vector<std::vector<int>>& matrix, int country1Idx, int country2Idx) {
    matrix[country1Idx][country2Idx] = 0;
    matrix[country2Idx][country1Idx] = 0;
}

// Поиск кратчайшего маршрута с использованием BFS
std::vector<int> findShortestRoute(const std::vector<std::vector<int>>& matrix, int startIdx, const std::unordered_set<int>& targetIdxs) {
    std::queue<std::vector<int>> q; // Очередь для хранения путей
    std::vector<bool> visited(matrix.size(), false);

    // Инициализация очереди начальной точкой
    q.push({startIdx});
    visited[startIdx] = true;

    while (!q.empty()) {
        std::vector<int> path = q.front();
        q.pop();
        int current = path.back();

        // Если достигли одной из целевых стран, возвращаем путь
        if (targetIdxs.count(current)) {
            return path;
        }

        // Добавляем соседей в очередь
        for (size_t i = 0; i < matrix.size(); ++i) {
            if (matrix[current][i] == 1 && !visited[i]) {
                visited[i] = true;
                std::vector<int> newPath = path;
                newPath.push_back(i);
                q.push(newPath);
            }
        }
    }

    return {}; // Путь не найден
}

// Проверка доступности до континентов и вывод маршрутов
void checkConnectionsToContinents(const std::vector<std::vector<int>>& matrix, int countryIdx,
                                  const std::unordered_map<std::string, std::vector<int>>& continents,
                                  const std::vector<std::string>& countries) {
    for (const auto& [continent, countryIdxs] : continents) {
        std::unordered_set<int> targetIdxs(countryIdxs.begin(), countryIdxs.end());

        std::vector<int> route = findShortestRoute(matrix, countryIdx, targetIdxs);

        if (!route.empty()) {
            std::cout << "Страна " << countries[countryIdx] << " имеет доступ к континенту " << continent << " по следующему маршруту:\n";
            for (size_t i = 0; i < route.size(); ++i) {
                std::cout << countries[route[i]];
                if (i < route.size() - 1) std::cout << " -> ";
            }
            std::cout << "\n";
        } else {
            std::cout << "Страна " << countries[countryIdx] << " НЕ имеет доступа к континенту " << continent << std::endl;
        }
    }
}

int main() {
    // Загрузка списка стран
    std::vector<std::string> countries = loadCountries("195_countries.txt");
    if (countries.empty()) {
        std::cerr << "Ошибка: Список стран пуст!" << std::endl;
        return 1;
    }

    // Загрузка матрицы связей
    std::vector<std::vector<int>> matrix = loadMatrix("connections_matrix.txt");
    if (matrix.size() != countries.size()) {
        std::cerr << "Ошибка: Размер матрицы не соответствует количеству стран!" << std::endl;
        return 1;
    }

    // Задание континентов
    std::unordered_map<std::string, std::vector<int>> continents = {
            {"Africa", {4, 18, 26, 27, 28, 30, 32, 33, 37, 38, 44, 50, 52, 53, 60, 61, 64, 68, 69, 85, 94, 95, 96, 100, 101, 104, 107, 108, 115, 116, 118, 124, 125, 151, 153, 154, 159, 160, 162, 165, 171, 174, 181, 193, 194}},
            {"Asia", {0, 12, 13, 77, 78, 79, 81, 84, 85, 86, 88, 89, 90, 91, 129, 130, 132, 135, 136, 140, 169, 170, 172, 173, 178, 179, 187, 191, 192}},
            {"Europe", {1, 3, 9, 15, 16, 25, 40, 42, 43, 45, 59, 63, 65, 74, 75, 80, 82, 92, 93, 97, 98, 99, 112, 114, 121, 127, 128, 138, 139, 141, 142, 152, 156, 157, 167, 168}},
            {"North America", {5, 11, 17, 31, 39, 41, 47, 48, 51, 66, 67, 70, 71, 72, 73, 109, 110, 123, 131, 133, 134, 144, 145, 146, 176, 185}},
            {"South America", {6, 20, 23, 49, 135, 136, 186, 190}},
            {"Oceania", {8}}
    };

    // Ввод двух стран, связь между которыми оборвалась
    std::string country1, country2;
    bool validInput = false;

    while (!validInput) {
        std::cout << "Введите две страны, между которыми оборвалась связь: ";
        std::cin >> country1 >> country2;

        // Проверка существования стран в списке
        auto it1 = std::find(countries.begin(), countries.end(), country1);
        auto it2 = std::find(countries.begin(), countries.end(), country2);

        if (it1 == countries.end() || it2 == countries.end()) {
            std::cerr << "Ошибка: Одна или обе введенные страны не найдены в списке! Попробуйте ещё раз.\n";
        } else {
            validInput = true;

            int country1Idx = std::distance(countries.begin(), it1);
            int country2Idx = std::distance(countries.begin(), it2);

            // Удаление связи из матрицы
            removeConnection(matrix, country1Idx, country2Idx);

            // Проверка доступности и построение маршрутов для обеих стран
            std::cout << "Проверка доступности и маршрутов для " << country1 << ":\n";
            checkConnectionsToContinents(matrix, country1Idx, continents, countries);

            std::cout << "\nПроверка доступности и маршрутов для " << country2 << ":\n";
            checkConnectionsToContinents(matrix, country2Idx, continents, countries);
        }
    }

    return 0;
}

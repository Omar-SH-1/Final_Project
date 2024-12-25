#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cctype>
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
        std::cerr << "Ne udalos' otkryt' fail: " << filename << std::endl;
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
        std::cerr << "Ne udalos' otkryt' fail: " << filename << std::endl;
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
            std::cerr << "Oshibka: Nekorrektnye dannye v stroke: \"" << line << "\"" << std::endl;
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

// Поиск альтернативных маршрутов с использованием BFS
std::vector<std::vector<int>> findRoutes(const std::vector<std::vector<int>>& matrix, int startIdx, const std::unordered_set<int>& targetIdxs) {
    std::queue<std::vector<int>> q; // Очередь для хранения путей
    std::vector<std::vector<int>> routes; // Список найденных маршрутов
    std::vector<bool> visited(matrix.size(), false);

    // Инициализация очереди начальной точкой
    q.push({startIdx});
    visited[startIdx] = true;

    while (!q.empty()) {
        std::vector<int> path = q.front();
        q.pop();
        int current = path.back();

        // Если достигли одной из целевых стран, сохраняем маршрут
        if (targetIdxs.count(current)) {
            routes.push_back(path);
            continue;
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

    return routes;
}

// Проверка доступности до континентов и вывод маршрутов
void checkConnectionsToContinents(const std::vector<std::vector<int>>& matrix, int countryIdx,
                                  const std::unordered_map<std::string, std::vector<int>>& continents,
                                  const std::vector<std::string>& countries) {
    for (const auto& [continent, countryIdxs] : continents) {
        std::unordered_set<int> targetIdxs(countryIdxs.begin(), countryIdxs.end());

        std::vector<std::vector<int>> routes = findRoutes(matrix, countryIdx, targetIdxs);

        if (!routes.empty()) {
            std::cout << "Strana " << countries[countryIdx] << " imeet dostup k kontinentu " << continent << " po sleduyushchim marshrutam:\n";
            for (const auto& route : routes) {
                for (size_t i = 0; i < route.size(); ++i) {
                    std::cout << countries[route[i]];
                    if (i < route.size() - 1) std::cout << " -> ";
                }
                std::cout << "\n";
            }
        } else {
            std::cout << "Strana " << countries[countryIdx] << " NE imeet dostupa k kontinentu " << continent << std::endl;
        }
    }
}

int main() {
    // Загрузка списка стран
    std::vector<std::string> countries = loadCountries("195_countries.txt");
    if (countries.empty()) {
        std::cerr << "Oshibka: Spisok stran pust!" << std::endl;
        return 1;
    }

    // Загрузка матрицы связей
    std::vector<std::vector<int>> matrix = loadMatrix("connections_matrix.txt");
    if (matrix.size() != countries.size()) {
        std::cerr << "Oshibka: Razmer matricy ne sootvetstvuet kolichestvu stran!" << std::endl;
        return 1;
    }

    // Задание континентов
    std::unordered_map<std::string, std::vector<int>> continents = {
            {"Africa", {2, 4, 18, 22, 25, 27, 28, 29, 32, 33, 37, 38, 44, 45, 46, 48, 53, 54, 60, 61, 68, 69, 87, 94, 95, 96, 100, 101, 104, 107, 108, 115, 116, 118, 124, 125, 143, 149, 150, 153, 154, 155, 159, 160, 162, 165, 171, 174, 177, 181, 193, 194}},
            {"Asia", {0, 7, 10, 12, 13, 14, 24, 29, 35, 42, 62, 76, 77, 78, 79, 81, 84, 85, 86, 89, 90, 91, 92, 93, 102, 103, 113, 117, 120, 126, 129, 130, 132, 137, 140, 150, 155, 161, 164, 169, 170, 172, 173, 178, 179, 183, 187, 191}},
            {"Europe", {1, 3, 9, 15, 16, 21, 25, 40, 43, 45, 54, 58, 59, 63, 65, 74, 75, 80, 82, 92, 97, 98, 99, 105, 111, 112, 113, 114, 121, 127, 128, 138, 139, 141, 142, 148, 152, 156, 157, 163, 167, 168, 182, 184, 189}},
            {"North America", {5, 11, 14, 17, 31, 39, 41, 47, 48, 51, 66, 67, 71, 73, 83, 109, 122, 123, 133, 144, 145, 146, 176, 185}},
            {"South America", {6, 20, 23, 34, 36, 49, 70, 135, 136, 166, 186, 190}},
            {"Oceania", {8, 57, 88, 106, 110, 119, 122, 131, 134, 147, 158, 174, 180, 188}}
    };

    // Ввод двух стран, связь между которыми оборвалась
    std::string country1, country2;
    std::cout << "Vvedite dve strany, mezhdu kotorymi oborvalas' svyaz': ";
    std::cin >> country1 >> country2;

    // Проверка существования стран в списке
    auto it1 = std::find(countries.begin(), countries.end(), country1);
    auto it2 = std::find(countries.begin(), countries.end(), country2);

    if (it1 == countries.end() || it2 == countries.end()) {
        std::cerr << "Oshibka: Odna ili obe vvedennye strany ne naideny v spiske!" << std::endl;
        return 1;
    }

    int country1Idx = std::distance(countries.begin(), it1);
    int country2Idx = std::distance(countries.begin(), it2);

    // Удаление связи из матрицы
    removeConnection(matrix, country1Idx, country2Idx);

    // Проверка доступности и построение маршрутов для обеих стран
    std::cout << "Proverka dostupnosti i marshrutov dlya " << country1 << ":\n";
    checkConnectionsToContinents(matrix, country1Idx, continents, countries);

    std::cout << "\nProverka dostupnosti i marshrutov dlya " << country2 << ":\n";
    checkConnectionsToContinents(matrix, country2Idx, continents, countries);

    return 0;
}

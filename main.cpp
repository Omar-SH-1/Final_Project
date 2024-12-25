#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

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
        std::vector<int> row;
        size_t start = 0, end;
        while ((end = line.find(',', start)) != std::string::npos) {
            row.push_back(std::stoi(line.substr(start, end - start)));
            start = end + 1;
        }
        row.push_back(std::stoi(line.substr(start)));
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

// BFS для проверки доступности
bool bfs(const std::vector<std::vector<int>>& matrix, int startIdx, const std::unordered_set<int>& targetIdxs) {
    std::queue<int> q;
    std::vector<bool> visited(matrix.size(), false);

    q.push(startIdx);
    visited[startIdx] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        if (targetIdxs.count(current)) {
            return true; // Найдена связь с одной из целевых стран
        }

        for (size_t i = 0; i < matrix.size(); ++i) {
            if (matrix[current][i] == 1 && !visited[i]) {
                visited[i] = true;
                q.push(i);
            }
        }
    }
    return false;
}

// Проверка доступности до континентов
void checkConnectionsToContinents(const std::vector<std::vector<int>>& matrix, int countryIdx,
                                  const std::unordered_map<std::string, std::vector<int>>& continents,
                                  const std::vector<std::string>& countries) {
    for (const auto& [continent, countryIdxs] : continents) {
        std::unordered_set<int> targetIdxs(countryIdxs.begin(), countryIdxs.end());

        bool connected = bfs(matrix, countryIdx, targetIdxs);

        if (connected) {
            std::cout << "Страна " << countries[countryIdx] << " имеет доступ к континенту " << continent << std::endl;
        } else {
            std::cout << "Страна " << countries[countryIdx] << " НЕ имеет доступа к континенту " << continent << std::endl;
        }
    }
}

int main() {
    // Загрузка списка стран
    std::vector<std::string> countries = loadCountries("195_countries.txt");

    // Загрузка матрицы связей
    std::vector<std::vector<int>> matrix = loadMatrix("connections_matrix.txt");

    // Задание континентов
    std::unordered_map<std::string, std::vector<int>> continents = {
            {"Africa", {2, 56, 78}},  // Индексы стран в Африке
            {"Asia", {34, 70, 102}},  // Индексы стран в Азии
            {"Europe", {10, 50, 150}}, // Индексы стран в Европе
            {"North America", {25, 30, 100}}, // Индексы стран в Северной Америке
            {"South America", {20, 60, 110}}, // Индексы стран в Южной Америке
            {"Australia", {8}} // Индекс Австралии
    };

    // Ввод двух стран, связь между которыми оборвалась
    std::string country1, country2;
    std::cout << "Введите две страны, между которыми оборвалась связь: ";
    std::cin >> country1 >> country2;

    // Получение индексов стран
    auto it1 = std::find(countries.begin(), countries.end(), country1);
    auto it2 = std::find(countries.begin(), countries.end(), country2);

    if (it1 == countries.end() || it2 == countries.end()) {
        std::cerr << "Одна из введенных стран не найдена в списке!" << std::endl;
        return 1;
    }

    int country1Idx = std::distance(countries.begin(), it1);
    int country2Idx = std::distance(countries.begin(), it2);

    // Удаление связи из матрицы
    removeConnection(matrix, country1Idx, country2Idx);

    // Проверка доступности
    std::cout << "Проверка доступности для " << country1 << ":\n";
    checkConnectionsToContinents(matrix, country1Idx, continents, countries);

    std::cout << "\nПроверка доступности для " << country2 << ":\n";
    checkConnectionsToContinents(matrix, country2Idx, continents, countries);

    return 0;
}
